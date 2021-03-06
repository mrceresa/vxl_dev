// This is brl/bseg/boxm2/pro/processes/boxm2_dem_to_xyz_process.cxx
#include <bprb/bprb_func_process.h>
//:
// \file
// \brief  A process to prepare x y z images from a geotiff dem, the resolutions are adjusted with respect to scene resolution.
// E.g. if DEM is a 10 m resolution and scene is of 1m resolution, then the output images are resampled versions of DEM
// given x,y,z images use ingest_dem process to initialize model
//
//  If a camera is passed as input (e.g. given by a previous process that reads it from tfw file then use it,
//  otherwise try reading it from geotiff header
//
// \author Ozge C. Ozcanli
// \date May 02, 2012

#include <vcl_fstream.h>
#include <boxm2/boxm2_scene.h>

#include <vpgl/file_formats/vpgl_geo_camera.h>
#include <vpgl/vpgl_generic_camera.h>
#include <vil/vil_image_view.h>
#include <vil/vil_image_resource.h>
#include <vil/vil_load.h>
#include <vcl_algorithm.h>
//#include <vil/vil_resample_bilin.h>
#include <vil/vil_convert.h>
#include <vil/vil_new.h>

#include <vsol/vsol_box_2d_sptr.h>
#include <vsol/vsol_box_2d.h>

#include <brip/brip_roi.h>


namespace boxm2_dem_to_xyz_process_globals
{
  const unsigned n_inputs_ = 5;
  const unsigned n_outputs_ = 3;
}

bool boxm2_dem_to_xyz_process_cons(bprb_func_process& pro)
{
  using namespace boxm2_dem_to_xyz_process_globals;

  //process takes 1 input
  vcl_vector<vcl_string> input_types_(n_inputs_);
  input_types_[0] = "boxm2_scene_sptr";
  input_types_[1] = "vcl_string";  // geotiff image of DEM
  input_types_[2] = "double"; // lvcs is using wgs84 so wrt ellipsoid, however some DEMs are using geoid,
                              // in that case pass the distance between ellipsoid and geoid in the region
                              // to convert DEM heights to heights wrt to ellipsoid
  input_types_[3] = "vpgl_camera_double_sptr";  // geocam if available, otherwise pass 0, camera will be constructed using info in geotiff header
  input_types_[4] = "float";  // some DEMs have gaps or invalid regions, pass the value in the DEM imagery that is used to fill those areas.

  // process has 1 outputs:
  vcl_vector<vcl_string>  output_types_(n_outputs_);
  output_types_[0] = "vil_image_view_base_sptr";  // x image
  output_types_[1] = "vil_image_view_base_sptr";  // y image
  output_types_[2] = "vil_image_view_base_sptr";  // z image

  return pro.set_input_types(input_types_) && pro.set_output_types(output_types_);
}

bool upsample_dem(vil_image_resource_sptr& out_img_res, unsigned ni, unsigned nj, vil_image_view<float>* dem_view, unsigned dem_ni, unsigned dem_nj)
{
  if (dem_ni == ni && dem_nj == nj) {
    out_img_res->put_view(*dem_view, 0,0);
    return true;
  }
  else if (dem_ni > ni || dem_nj > nj) {
    vcl_cerr << "in upsample_dem() -- cannot downsample image, image sizes are incompatible!\n";
    return false;
  }
  int block_ni = ni/dem_ni;
  int block_nj = nj/dem_nj;
  vcl_cout << "ni: " << ni << " dem_ni: " << dem_ni << " block_ni: " << block_ni << '\n'
           << "nj: " << nj << " dem_nj: " << dem_nj << " block_nj: " << block_nj << vcl_endl;
  for (unsigned i = 0; i < dem_ni; i++) {
    for (unsigned j = 0; j < dem_nj; j++) {
      vil_image_view<float> block(block_ni, block_nj, 1);
      block.fill((*dem_view)(i,j));
      out_img_res->put_view(block, i*block_ni, j*block_nj);
    }
  }
  return true;
}

bool boxm2_dem_to_xyz_process(bprb_func_process& pro)
{
  using namespace boxm2_dem_to_xyz_process_globals;

  if ( pro.n_inputs() < n_inputs_ ){
    vcl_cout << pro.name() << ": The input number should be " << n_inputs_<< vcl_endl;
    return false;
  }
  //get the inputs
  boxm2_scene_sptr scene = pro.get_input<boxm2_scene_sptr>(0);
  vpgl_lvcs_sptr lvcs = new vpgl_lvcs(scene->lvcs());
  vcl_string geotiff_fname = pro.get_input<vcl_string>(1);
  double geoid_height = pro.get_input<double>(2);
  vpgl_camera_double_sptr cam = pro.get_input<vpgl_camera_double_sptr>(3);
  float fill_in_value = pro.get_input<float>(4);

  vil_image_resource_sptr dem_res = vil_load_image_resource(geotiff_fname.c_str());

  vpgl_geo_camera* geocam = 0;
  if (cam) {
    vcl_cout << "Using the loaded camera!\n";
    geocam = dynamic_cast<vpgl_geo_camera*> (cam.ptr());
  }
  else
    vpgl_geo_camera::init_geo_camera(dem_res, lvcs, geocam);

  if (!geocam) {
    vcl_cerr << "In boxm2_dem_to_xyz_process() - the geocam could not be initialized!\n";
    return false;
  }

  vgl_box_3d<double> scene_bbox = scene->bounding_box();
  vgl_vector_3d<unsigned> dims = scene->scene_dimensions();
  vcl_vector<boxm2_block_id> blks = scene->get_block_ids();
  if (blks.size() < 1)
    return false;

  unsigned orig_dem_ni = dem_res->ni(); unsigned orig_dem_nj = dem_res->nj();
  vcl_cout << "original dem resolution: " << orig_dem_ni << ' ' << orig_dem_nj << vcl_endl;
  brip_roi broi(orig_dem_ni, orig_dem_nj);
  vsol_box_2d_sptr bb = new vsol_box_2d();

  double u,v;
  geocam->project(scene_bbox.min_x(), scene_bbox.min_y(), scene_bbox.min_z(), u, v);
  bb->add_point(u,v);
  geocam->project(scene_bbox.max_x(), scene_bbox.max_y(), scene_bbox.max_z(), u, v);
  bb->add_point(u,v);
  bb = broi.clip_to_image_bounds(bb);
  if (bb->width() <= 0 || bb->height() <= 0) {
    vcl_cout << "In boxm2_dem_to_xyz_process() -- " << geotiff_fname << " does not overlap the scene!\n";
    return false;
  }

  vil_image_view_base_sptr dem_view_base = dem_res->get_view(0, orig_dem_ni, 0, orig_dem_nj);
  vil_image_view<float>* dem_view = dynamic_cast<vil_image_view<float>*>(dem_view_base.ptr());
  if (!dem_view) {
    vil_image_view<float> temp(dem_view_base->ni(), dem_view_base->nj(), 1);

    vil_image_view<vxl_int_16>* dem_view_int = dynamic_cast<vil_image_view<vxl_int_16>*>(dem_view_base.ptr());
    if (!dem_view_int) {
      vil_image_view<vxl_byte>* dem_view_byte = dynamic_cast<vil_image_view<vxl_byte>*>(dem_view_base.ptr());
      if (!dem_view_byte) {
        vcl_cerr << "Error: boxm2_dem_to_xyz_process: The image pixel format: " << dem_view_base->pixel_format() << " is not supported!\n";
        return false;
      }
      else
        vil_convert_cast(*dem_view_byte, temp);
    }
    else
      vil_convert_cast(*dem_view_int, temp);
    dem_view = new vil_image_view<float>(temp);
  }

  boxm2_scene_info* info = scene->get_blk_metadata(blks[0]);
  float sb_length = info->block_len;
  float vox_length = sb_length/8.0f;

  // prepare an image for the finest resolution
  int ni = (int)vcl_ceil((scene_bbox.max_x()-scene_bbox.min_x())/vox_length);
  int nj = (int)vcl_ceil((scene_bbox.max_y()-scene_bbox.min_y())/vox_length);

  vcl_cout <<"ni: " << ni << " nj: " << nj << vcl_endl;
  vcl_cout.flush();

  // create x y z images
  vil_image_view<float>* out_img_x = new vil_image_view<float>(ni, nj, 1);
  vil_image_view<float>* out_img_y = new vil_image_view<float>(ni, nj, 1);
  vil_image_view<float>* out_img_z = new vil_image_view<float>(ni, nj, 1);
  out_img_x->fill((float)(scene_bbox.min_x())-10.0f);  // local coord system min z
  out_img_y->fill((float)(scene_bbox.min_y())-10.0f);  // local coord system min z
  out_img_z->fill((float)(scene_bbox.min_z())-1.0f);  // local coord system min z
  vcl_cout << "out img x(0,0): " << ((*out_img_x)(0,0)) << vcl_endl; vcl_cout.flush();
  vcl_cout << "out img y(0,0): " << ((*out_img_y)(0,0)) << vcl_endl; vcl_cout.flush();
  vcl_cout << "out img z(0,0): " << ((*out_img_z)(0,0)) << vcl_endl; vcl_cout.flush();

  double lon,lat,gz;
  lvcs->local_to_global(0,0,0,vpgl_lvcs::wgs84,lon, lat, gz);
  vcl_cout << "lvcs origin height: " << gz << vcl_endl;
  gz += geoid_height;  // correct for the difference to geoid if necessary, geoid_height should have been passed 0 if that is not necessary
  gz += scene_bbox.min_z();
  
  vcl_cout << " scene min z: " << scene_bbox.min_z() << " gz: " << gz << vcl_endl; 
  vcl_cout.flush();
  if (fill_in_value <= 0)
    fill_in_value = vcl_numeric_limits<float>::max();

  for (int i = 0; i < ni; ++i)
    for (int j = 0; j < nj; ++j) {
      float local_x = (float)(i*vox_length+scene_bbox.min_x()+vox_length/2.0);
      float local_y = (float)(scene_bbox.max_y()-j*vox_length+vox_length/2.0);
      (*out_img_x)(i,j) = local_x;
      (*out_img_y)(i,j) = local_y;

      double u,v;
      geocam->project(local_x, local_y, scene_bbox.min_z(), u, v);
      // for now just cast to the nearest pixel in DEM, might want to sample bilinearly
      int uu = (int)vcl_floor(u+0.5);
      int vv = (int)vcl_floor(v+0.5);
      if (uu >= 0 && vv >= 0 && uu < (int)orig_dem_ni && vv < (int)orig_dem_ni) {
        if ((*dem_view)(uu,vv) < fill_in_value)  // otherwise it remains at local min z
          (*out_img_z)(i,j) = (*dem_view)(uu,vv)-(float)gz;  // we need local height
      }
    }

  pro.set_output_val<vil_image_view_base_sptr>(0, out_img_x);
  pro.set_output_val<vil_image_view_base_sptr>(1, out_img_y);
  pro.set_output_val<vil_image_view_base_sptr>(2, out_img_z);

  return true;
}

//: Craete x y z images from the shadow height map of an ortho aerial image

namespace boxm2_shadow_heights_to_xyz_process_globals
{
  const unsigned n_inputs_ = 5;
  const unsigned n_outputs_ = 3;
}

bool boxm2_shadow_heights_to_xyz_process_cons(bprb_func_process& pro)
{
  using namespace boxm2_shadow_heights_to_xyz_process_globals;

  //process takes 5 inputs
  vcl_vector<vcl_string> input_types_(n_inputs_);
  input_types_[0] = "boxm2_scene_sptr";
  input_types_[1] = "vil_image_view_base_sptr";  // shadow height map image of the cropped ortho aerial image, height values are in pixels with respect to a horizontal surface at the base of the object
  input_types_[2] = "vpgl_camera_double_sptr"; // generic cam of cropped ortho aerial image (to be given by roi_init_geotiff_process)
  input_types_[3] = "vcl_string"; // dem_fname, height image values will be added to these values
  input_types_[4] = "double"; // pixel scaling

  // process has 3 outputs:
  vcl_vector<vcl_string>  output_types_(n_outputs_);
  output_types_[0] = "vil_image_view_base_sptr";  // x image
  output_types_[1] = "vil_image_view_base_sptr";  // y image
  output_types_[2] = "vil_image_view_base_sptr";  // z image

  return pro.set_input_types(input_types_) && pro.set_output_types(output_types_);
}

bool boxm2_shadow_heights_to_xyz_process(bprb_func_process& pro)
{
  using namespace boxm2_shadow_heights_to_xyz_process_globals;

  if ( pro.n_inputs() < n_inputs_ ){
    vcl_cout << pro.name() << ": The input number should be " << n_inputs_<< vcl_endl;
    return false;
  }
  //get the inputs
  boxm2_scene_sptr scene = pro.get_input<boxm2_scene_sptr>(0);
  vpgl_lvcs_sptr lvcs = new vpgl_lvcs(scene->lvcs());
  vil_image_view_base_sptr img = pro.get_input<vil_image_view_base_sptr>(1);
  vpgl_camera_double_sptr cam = pro.get_input<vpgl_camera_double_sptr>(2);
  vcl_string geotiff_fname = pro.get_input<vcl_string>(3);
  double scale = pro.get_input<double>(4);

  vpgl_generic_camera<double>* gcam=dynamic_cast<vpgl_generic_camera<double>*> (cam.ptr());

  vil_image_view<float> height_img(img);
  unsigned ni = height_img.ni();
  unsigned nj = height_img.nj();

  if (gcam->rows() != nj || gcam->cols() !=ni) {
    vcl_cerr << "In boxm2_shadow_heights_to_xyz_process() - generic cam and height image sizes are not compatible!\n"
             << "gcam rows: " << gcam->rows() << " cols: " << gcam->cols() << " ni: " << ni << " nj: " << nj << '\n';
    return false;
  }

  vil_image_resource_sptr dem_res = vil_load_image_resource(geotiff_fname.c_str());
  vpgl_geo_camera* geocam = 0;
  vpgl_geo_camera::init_geo_camera(dem_res, lvcs, geocam);

  vgl_box_3d<double> scene_bbox = scene->bounding_box();
  vgl_vector_3d<unsigned> dims = scene->scene_dimensions();
  vcl_vector<boxm2_block_id> blks = scene->get_block_ids();
  if (blks.size() < 1)
    return false;

  // crop the image from the DEM
  vgl_box_2d<double> proj_bbox;
  double u,v;
  geocam->project(scene_bbox.min_x(), scene_bbox.min_y(), scene_bbox.min_z(), u, v);
  proj_bbox.add(vgl_point_2d<double>(u,v));
  geocam->project(scene_bbox.max_x(), scene_bbox.max_y(), scene_bbox.max_z(), u, v);
  proj_bbox.add(vgl_point_2d<double>(u,v));
  int min_i = int(vcl_max(0.0, vcl_floor(proj_bbox.min_x())));
  int min_j = int(vcl_max(0.0, vcl_floor(proj_bbox.min_y())));
  int max_i = int(vcl_min(dem_res->ni()-1.0, vcl_ceil(proj_bbox.max_x())));
  int max_j = int(vcl_min(dem_res->nj()-1.0, vcl_ceil(proj_bbox.max_y())));
  vcl_cout << "scene projected in the image mini: " << min_i << " minj: " << min_j << " maxi: " << max_i << " maxj: " << max_j << vcl_endl;
  unsigned int dem_ni = max_i - min_i + 1;
  unsigned int dem_nj = max_j - min_j + 1;
  vil_image_view_base_sptr dem_view_base = dem_res->get_view((unsigned int)min_i, dem_ni, (unsigned int)min_j, dem_nj);
  vil_image_view<float>* dem_view = dynamic_cast<vil_image_view<float>*>(dem_view_base.ptr());
  if (!dem_view) {
      vcl_cerr << "Error: boxm2_dem_to_xyz_process: could not cast first return image to a vil_image_view<float>\n";
      return false;
  }
  vcl_cout << "dem resolution is: " << dem_ni << " by " << dem_nj << vcl_endl;

  vil_image_view<float>* out_img = new vil_image_view<float>(ni, nj, 1);
  vil_image_resource_sptr out_img_res = vil_new_image_resource_of_view(*out_img);

  if (!upsample_dem(out_img_res, ni, nj, dem_view, dem_ni, dem_nj))
    return false;

  // create x y z images
  vil_image_view<float>* out_img_x = new vil_image_view<float>(ni, nj, 1);
  vil_image_view<float>* out_img_y = new vil_image_view<float>(ni, nj, 1);
  vil_image_view<float>* out_img_z = new vil_image_view<float>(ni, nj, 1);

  double lon,lat,gz;
  lvcs->local_to_global(0,0,0,vpgl_lvcs::wgs84,lon, lat, gz);

  for (unsigned i = 0; i < ni; i++)
    for (unsigned j = 0; j < nj; j++) {
      vgl_ray_3d<double> ray = gcam->ray(i,j);
      vgl_point_3d<double> o = ray.origin();
      (*out_img_x)(i,j) = (float)o.x();
      (*out_img_y)(i,j) = (float)o.y();
      if (height_img(i,j) >= 0) {
        (*out_img_z)(i,j) = (float)(height_img(i,j)*scale+(*out_img)(i,j)-gz);  // we need local height
      }
      else
        (*out_img_z)(i,j) = (*out_img)(i,j)-(float)gz;  // we need local height
    }

  pro.set_output_val<vil_image_view_base_sptr>(0, out_img_x);
  pro.set_output_val<vil_image_view_base_sptr>(1, out_img_y);
  pro.set_output_val<vil_image_view_base_sptr>(2, out_img_z);
  return true;
}

namespace boxm2_dem_to_xyz_process2_globals
{
  const unsigned n_inputs_ = 5;
  const unsigned n_outputs_ = 3;
}

bool boxm2_dem_to_xyz_process2_cons(bprb_func_process& pro)
{
  using namespace boxm2_dem_to_xyz_process2_globals;

  //process takes 1 input
  vcl_vector<vcl_string> input_types_(n_inputs_);
  input_types_[0] = "boxm2_scene_sptr";
  input_types_[1] = "vcl_string";  // geotiff image of DEM
  input_types_[2] = "double"; // lvcs is using wgs84 so wrt ellipsoid, however some DEMs are using geoid,
                              // in that case pass the distance between ellipsoid and geoid in the region
                              // to convert DEM heights to heights wrt to ellipsoid
  input_types_[3] = "vpgl_camera_double_sptr";  // geocam if available, otherwise pass 0, camera will be constructed using info in geotiff header
  input_types_[4] = "float";  // some DEMs have gaps or invalid regions, pass the value in the DEM imagery that is used to fill those areas.

  // process has 1 outputs:
  vcl_vector<vcl_string>  output_types_(n_outputs_);
  output_types_[0] = "vil_image_view_base_sptr";  // x image
  output_types_[1] = "vil_image_view_base_sptr";  // y image
  output_types_[2] = "vil_image_view_base_sptr";  // z image

  return pro.set_input_types(input_types_) && pro.set_output_types(output_types_);
}

bool boxm2_dem_to_xyz_process2(bprb_func_process& pro)
{
  using namespace boxm2_dem_to_xyz_process2_globals;

  if ( pro.n_inputs() < n_inputs_ ){
    vcl_cout << pro.name() << ": The input number should be " << n_inputs_<< vcl_endl;
    return false;
  }
  //get the inputs
  boxm2_scene_sptr scene = pro.get_input<boxm2_scene_sptr>(0);
  vpgl_lvcs_sptr lvcs = new vpgl_lvcs(scene->lvcs());
  vcl_string geotiff_fname = pro.get_input<vcl_string>(1);
  double geoid_height = pro.get_input<double>(2);
  vpgl_camera_double_sptr cam = pro.get_input<vpgl_camera_double_sptr>(3);
  float fill_in_value = pro.get_input<float>(4);

  vil_image_resource_sptr dem_res = vil_load_image_resource(geotiff_fname.c_str());

  vpgl_geo_camera* geocam = 0;
  if (cam) {
    vcl_cout << "Using the loaded camera!\n";
    geocam = dynamic_cast<vpgl_geo_camera*> (cam.ptr());
  }
  else
    vpgl_geo_camera::init_geo_camera(dem_res, lvcs, geocam);

  if (!geocam) {
    vcl_cerr << "In boxm2_dem_to_xyz_process() - the geocam could not be initialized!\n";
    return false;
  }

  vgl_box_3d<double> scene_bbox = scene->bounding_box();
  vgl_vector_3d<unsigned> dims = scene->scene_dimensions();
  vcl_vector<boxm2_block_id> blks = scene->get_block_ids();
  if (blks.size() < 1)
    return false;

  unsigned orig_dem_ni = dem_res->ni(); unsigned orig_dem_nj = dem_res->nj();
  vcl_cout << "original dem resolution: " << orig_dem_ni << ' ' << orig_dem_nj << vcl_endl;
  brip_roi broi(orig_dem_ni, orig_dem_nj);
  vsol_box_2d_sptr bb = new vsol_box_2d();

  double min_uu, min_vv, max_uu, max_vv;
  geocam->project(scene_bbox.min_x(), scene_bbox.min_y(), scene_bbox.min_z(), min_uu, min_vv);
  bb->add_point(min_uu,min_vv);
  geocam->project(scene_bbox.max_x(), scene_bbox.max_y(), scene_bbox.max_z(), max_uu, max_vv);
  bb->add_point(max_uu,max_vv);
  bb = broi.clip_to_image_bounds(bb);
  if (bb->width() <= 0 || bb->height() <= 0) {
    vcl_cout << "In boxm2_dem_to_xyz_process() -- " << geotiff_fname << " does not overlap the scene!\n";
    return false;
  }
  vcl_cout <<"projected scene bbox: " << *bb << vcl_endl;

  unsigned int min_i = min_uu > 0 ? (unsigned int)min_uu : 0;
  unsigned int min_j = max_vv > 0 ? (unsigned int)max_vv : 0;  // scene box min projects to lower left corner of the scene in the image
  
  int ni = max_uu-min_uu+1 < orig_dem_ni ? (int)(max_uu-min_uu+1) : orig_dem_ni;
  int nj = min_vv-max_vv+1 < orig_dem_nj ? (int)(min_vv-max_vv+1) : orig_dem_nj;
  
  vcl_cout <<"min_uu: " << min_uu << " min_vv: " << min_vv << vcl_endl;
  vcl_cout <<"max_uu: " << max_uu << " max_vv: " << max_vv << vcl_endl;
  vcl_cout <<"min_i: " << min_i << " min_j: " << min_j << " ni: " << ni << " nj: " << nj << vcl_endl;
  vcl_cout.flush();
  
  vil_image_view_base_sptr dem_view_base = dem_res->get_view(min_i, ni, min_j, nj);
  vil_image_view<float>* dem_view = dynamic_cast<vil_image_view<float>*>(dem_view_base.ptr());
  if (!dem_view) {
    vil_image_view<float> temp(dem_view_base->ni(), dem_view_base->nj(), 1);

    vil_image_view<vxl_int_16>* dem_view_int = dynamic_cast<vil_image_view<vxl_int_16>*>(dem_view_base.ptr());
    if (!dem_view_int) {
      vil_image_view<vxl_byte>* dem_view_byte = dynamic_cast<vil_image_view<vxl_byte>*>(dem_view_base.ptr());
      if (!dem_view_byte) {
        vcl_cerr << "Error: boxm2_dem_to_xyz_process: The image pixel format: " << dem_view_base->pixel_format() << " is not supported!\n";
        return false;
      }
      else
        vil_convert_cast(*dem_view_byte, temp);
    }
    else
      vil_convert_cast(*dem_view_int, temp);
    dem_view = new vil_image_view<float>(temp);
  }
  vcl_cout <<"got dem image!" << vcl_endl;
  vcl_cout.flush();

  boxm2_scene_info* info = scene->get_blk_metadata(blks[0]);
  float sb_length = info->block_len;
  vcl_cout <<"sb_length: " << sb_length << "!\n" << vcl_endl;
  vcl_cout.flush();

  vcl_cout <<"ni: " << ni << " nj: " << nj << vcl_endl;
  vcl_cout.flush();

  // create x y z images
  vil_image_view<float>* out_img_x = new vil_image_view<float>(ni, nj, 1);
  vil_image_view<float>* out_img_y = new vil_image_view<float>(ni, nj, 1);
  vil_image_view<float>* out_img_z = new vil_image_view<float>(ni, nj, 1);
  out_img_x->fill((float)(scene_bbox.min_x())-10.0f);  // local coord system min z
  out_img_y->fill((float)(scene_bbox.min_y())-10.0f);  // local coord system min z
  out_img_z->fill((float)(scene_bbox.min_z())-1.0f);  // local coord system min z
  vcl_cout << "out img x(0,0): " << ((*out_img_x)(0,0)) << vcl_endl; vcl_cout.flush();
  vcl_cout << "out img y(0,0): " << ((*out_img_y)(0,0)) << vcl_endl; vcl_cout.flush();
  vcl_cout << "out img z(0,0): " << ((*out_img_z)(0,0)) << vcl_endl; vcl_cout.flush();

  for (int i = 0; i < ni; ++i)
    for (int j = 0; j < nj; ++j) {
      // find the global coord of this pixel
      double lon, lat;
      geocam->img_to_global(i+min_i, j+min_j, lon, lat);
      // find the local coord of this global position
      double lx, ly, lz;
      lvcs->global_to_local(lon, lat, (*dem_view)(i,j), vpgl_lvcs::wgs84, lx, ly, lz);
      vgl_point_3d<double> pt(lx, ly, lz);
      (*out_img_x)(i,j) = (float)lx;
      (*out_img_y)(i,j) = (float)ly;
      (*out_img_z)(i,j) = (float)lz;           
    }
  

  pro.set_output_val<vil_image_view_base_sptr>(0, out_img_x);
  pro.set_output_val<vil_image_view_base_sptr>(1, out_img_y);
  pro.set_output_val<vil_image_view_base_sptr>(2, out_img_z);

  return true;
}

