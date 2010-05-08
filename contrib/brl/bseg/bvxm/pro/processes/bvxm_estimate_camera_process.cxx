// This is brl/bseg/bvxm/pro/processes/bvxm_estimate_camera_process.cxx
#include "bvxm_estimate_camera_process.h"
//:
// \file
#include <bprb/bprb_func_process.h>
#include <bprb/bprb_parameters.h>

#include <brdb/brdb_value.h>

#include <bvxm/bvxm_voxel_world.h>
#include <bvxm/bvxm_image_metadata.h>

#include <brip/brip_vil_float_ops.h>

#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_vector_3d.h>
#include <vgl/vgl_plane_3d.h>
#include <bvxm/bvxm_edge_ray_processor.h>

#include <vcl_cstdio.h>
#include <vcl_cassert.h>

#include <vul/vul_timer.h>

#include <vgl/vgl_distance.h>

#include "bvxm_estimate_camera_utils.h"

//: set input and output types
bool bvxm_estimate_camera_process_cons(bprb_func_process& pro)
{
  using namespace bvxm_estimate_camera_process_globals;

  // process takes 4 inputs:
  //input[0]: The voxel world
  //input[1]: Initial camera
  //input[2]: Edge image
  //input[3]: Scale of the image

  vcl_vector<vcl_string> input_types_(n_inputs_);
  unsigned i = 0;
  input_types_[i++] = "bvxm_voxel_world_sptr";
  input_types_[i++] = "vpgl_camera_double_sptr";
  input_types_[i++] = "vil_image_view_base_sptr";
  input_types_[i++] = "unsigned";
  if (!pro.set_input_types(input_types_))
    return false;

  // process has 3 outputs:
  // output[0]: The optimized camera
  // output[1]: Expected edge image (after camera correction)
  // output[2]: Expected edge image (before camera correction)

  vcl_vector<vcl_string> output_types_(n_outputs_);
  unsigned j = 0;
  output_types_[j++] = "vpgl_camera_double_sptr";
  output_types_[j++] = "vil_image_view_base_sptr";
  output_types_[j++] = "vil_image_view_base_sptr";
  return pro.set_output_types(output_types_);
}

//:  optimizes camera parameters based on edges
bool bvxm_estimate_camera_process(bprb_func_process& pro)
{
  using namespace bvxm_estimate_camera_process_globals;

  //check number of inputs
  if ( pro.n_inputs() < n_inputs_ ) {
    vcl_cout << pro.name() << " The input number should be " << n_inputs_<< vcl_endl;
    return false;
  }

  // get the inputs
  unsigned i = 0;

  // voxel world
  bvxm_voxel_world_sptr vox_world = pro.get_input<bvxm_voxel_world_sptr>(i++);
  bvxm_edge_ray_processor edge_proc(vox_world);

  // camera
  vpgl_camera_double_sptr cam_inp = pro.get_input<vpgl_camera_double_sptr>(i++);
  vpgl_perspective_camera<double> *cam_init = dynamic_cast<vpgl_perspective_camera<double>*>(cam_inp.ptr());

  // image
  vil_image_view_base_sptr img_e_sptr = pro.get_input<vil_image_view_base_sptr>(i++);
  vil_image_view<vxl_byte> img_e_vb(img_e_sptr);

  vil_image_view<float> img_e;
  bvxm_camera_estimator::normalize_to_interval<vxl_byte,float>(img_e_vb,img_e,0.0f,1.0f);

  unsigned ni = img_e.ni();
  unsigned nj = img_e.nj();

  // scale of image
  unsigned scale = pro.get_input<unsigned>(i++);

  double deg2rad = vnl_math::pi/180.0;

  double theta_range = 12.0*deg2rad, theta_step = 0.05*deg2rad, phi_range = 12.0*deg2rad, phi_step = 0.05*deg2rad, rot_range = 10.0*deg2rad, rot_step = 1.0*deg2rad;
  int max_iter_rot_angle = 5, max_iter_cam_center = 50;

  pro.parameters()->get_value(theta_range_, theta_range);
  pro.parameters()->get_value(theta_step_, theta_step);
  pro.parameters()->get_value(phi_range_, phi_range);
  pro.parameters()->get_value(phi_step_, phi_step);
  pro.parameters()->get_value(rot_range_, rot_range);
  pro.parameters()->get_value(rot_step_, rot_step);
  pro.parameters()->get_value(max_iter_rot_angle_, max_iter_rot_angle);
  pro.parameters()->get_value(max_iter_cam_center_, max_iter_cam_center);

#if 0
  vcl_cout << "printing bvxm_estimate_camera_process parameters:\n"
           << "theta_range: " << theta_range << '\n'
           << "theta_step: " << theta_step << '\n'
           << "phi_range: " << phi_range << '\n'
           << "phi_step: " << phi_step << '\n'
           << "rot_range: " << rot_range << '\n'
           << "rot_step: " << rot_step << '\n'
           << "max_iter_rot_angle: " << max_iter_rot_angle << '\n'
           << "max_iter_cam_center: " << max_iter_cam_center << '\n';
#endif

  //float n_normal = vox_world->get_params()->edges_n_normal();

  int num_obs = vox_world->num_observations<EDGES>(0,scale);
  vcl_cout << "Number of observations in the voxel world: " << num_obs << '\n';

  vil_image_view<float> *img_eei = new vil_image_view<float>(ni,nj,1);
  img_eei->fill(0.0f);

  typedef bvxm_voxel_traits<EDGES>::voxel_datatype edges_datatype;
  bvxm_voxel_grid_base_sptr edges_grid_base = vox_world->get_grid<EDGES>(0,scale);
  bvxm_voxel_grid<edges_datatype> *edges_grid  = static_cast<bvxm_voxel_grid<edges_datatype>*>(edges_grid_base.ptr());

  int nx = (int)edges_grid->grid_size().x();
  int ny = (int)edges_grid->grid_size().y();
  int nz = (int)edges_grid->grid_size().z();

  bvxm_voxel_slab<float> data(nx,ny,nz);

  vgl_point_3d<float> pt_0 = vox_world->voxel_index_to_xyz(0,0,0);
  vgl_point_3d<float> pt_1 = vox_world->voxel_index_to_xyz(1,1,1);

  double sx = (double)pt_0.x();
  double sy = (double)pt_0.y();
  double sz = (double)pt_0.z();

  double dx = (double)(pt_1.x() - pt_0.x());
  double dy = (double)(pt_1.y() - pt_0.y());
  double dz = (double)(pt_1.z() - pt_0.z());

  bvxm_voxel_grid<edges_datatype>::iterator edges_grid_it(edges_grid->begin());

  for (int z=0; z<nz; z++, ++edges_grid_it) {
    for (int x=0; x<nx; x++) {
      for (int y=0; y<ny; y++) {
        data(x,y,z) = (*edges_grid_it)(x,y);
      }
    }
  }

  vpgl_perspective_camera<double> *cam_est = new vpgl_perspective_camera<double>(*cam_init);

  bvxm_camera_estimator cam_estimator;
  cam_estimator.set_world_params(nx,ny,nz,sx,sy,sz,dx,dy,dz);
  cam_estimator.set_estimation_params(theta_range,theta_step,phi_range,phi_step,rot_range,rot_step,max_iter_rot_angle);

  cam_estimator.get_expected_edge_image(data,cam_est,img_eei);

  vil_image_view<vxl_byte> *img_eei_before_correction = new vil_image_view<vxl_byte>(ni,nj,1);
  bvxm_camera_estimator::normalize_to_interval<float,vxl_byte>(*img_eei,*img_eei_before_correction,0.0f,255.0f);

  //vcl_cout << "Estimating correct camera parameters\n";
  bvxm_camera_estimator_amoeba cost_ftn(data,cam_estimator,img_e,cam_est);
  vnl_vector<double> x(2,1.0);
  vnl_amoeba amoeba(cost_ftn);
  amoeba.set_relative_diameter(3.0);
  amoeba.set_max_iterations(max_iter_cam_center);
  amoeba.minimize(x);
  cost_ftn.get_result(x,cam_est);

  cam_estimator.get_expected_edge_image(data,cam_est,img_eei);

  vil_image_view<vxl_byte> *img_eei_vb = new vil_image_view<vxl_byte>(ni,nj,1);
  bvxm_camera_estimator::normalize_to_interval<float,vxl_byte>(*img_eei,*img_eei_vb,0.0f,255.0f);

  // output
  unsigned j = 0;
  // update the camera and store
  pro.set_output_val<vpgl_camera_double_sptr>(j++, cam_est);
  // update the edge image after update and store
  pro.set_output_val<vil_image_view_base_sptr>(j++, img_eei_vb);
  // update the edge image before update and store
  pro.set_output_val<vil_image_view_base_sptr>(j++, img_eei_before_correction);

  return true;
}
