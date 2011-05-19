//:
// \file
// \brief Parses the configuration file for bwm tool.
//
#include "boxm2_scene_parser.h"

#include <vcl_sstream.h>
#include <vcl_cstring.h>
#include <vcl_iostream.h>
#include <vcl_cstdio.h>

// --------------
// --- PARSER ---
// --------------
template <typename T>
void convert(const char* t, T& d)
{
  vcl_stringstream strm(t);
  strm >> d;
}

boxm2_scene_parser::boxm2_scene_parser()
{
  init_params();
}

bool boxm2_scene_parser::lvcs(bgeo_lvcs& lvcs)
{
  bgeo_lvcs::cs_names cs_name = bgeo_lvcs::str_to_enum(lvcs_cs_name_.data());
  bgeo_lvcs::LenUnits len_unit;
  bgeo_lvcs::AngUnits geo_unit;
   if (vcl_strcmp(lvcs_XYZ_unit_.data(), "feet")==0)
     len_unit = bgeo_lvcs::FEET;
   if (vcl_strcmp(lvcs_XYZ_unit_.data(), "meters")==0)
     len_unit = bgeo_lvcs::METERS;
   else {
     vcl_cout << "LVCS Length Unit " << lvcs_XYZ_unit_ << " is not valid\n";
     return false;
   }

   if (vcl_strcmp(lvcs_geo_angle_unit_.data(), "radians")==0)
     geo_unit = bgeo_lvcs::RADIANS;
   if (vcl_strcmp(lvcs_geo_angle_unit_.data(), "degrees")==0)
     geo_unit = bgeo_lvcs::DEG;
   else {
     vcl_cout << "LVCS Geo Angle Unit " << lvcs_geo_angle_unit_ << " is not valid\n";
     return false;
   }

   lvcs = bgeo_lvcs(lvcs_origin_lat_,lvcs_origin_lon_,lvcs_origin_elev_, cs_name,
                    lvcs_lat_scale_, lvcs_lon_scale_, geo_unit, len_unit,
                    lvcs_local_origin_x_, lvcs_local_origin_y_, lvcs_theta_);
  return true;
}

void boxm2_scene_parser::init_params()
{
  lvcs_cs_name_="";
  lvcs_origin_lon_=0;
  lvcs_origin_lat_=0;
  lvcs_origin_elev_=0;
  lvcs_lon_scale_=0;
  lvcs_lat_scale_=0;
  lvcs_XYZ_unit_="";
  lvcs_geo_angle_unit_="";
  lvcs_local_origin_x_=0;
  lvcs_local_origin_y_=0;
  lvcs_theta_=0;
  num_illum_bins_ = -1;//indicates not defined
  // world origin
  origin_ = vgl_point_3d<double>(0,0,0);
  path_="";
  name_="";
}


//-----------------------------------------------------------------------------
//: Start Element needs to parse the following tags
// - scene level metadata
//   * LVCS_TAG "lvcs"
//   * LOCAL_ORIGIN_TAG "local_origin"
//   * SCENE_PATHS_TAG "scene_paths"
// - block level metadata
//   * BLOCK_TAG "block"
//   * BLOCK_ID_TAG "block_id"
//   * BLOCK_ORIGIN_TAG "block_origin"
//   * SUB_BLOCK_DIMENSIONS_TAG "sub_block_dimensions"
//   * TREE_INIT_LEVEL_TAG "tree_init_level"
//   * TREE_MAX_LEVEL_TAG "tree_max_level"
//   * P_INIT_TAG "p_init"
//   * MAX_MB_TAG "max_mb"
void
boxm2_scene_parser::startElement(const char* name, const char** atts)
{
#if 0
  vcl_cout<< "element=" << name << '\n'
          << "  Attr=" << atts[i] << "->" << atts[i+1] << vcl_endl;
#endif

  //LVCS tag
  if (vcl_strcmp(name, LVCS_TAG) == 0) {
    for (int i=0; atts[i]; i+=2) {
      if (vcl_strcmp(atts[i], "cs_name") == 0)
        convert(atts[i+1], lvcs_cs_name_);
      else if (vcl_strcmp(atts[i], "origin_lon") == 0)
        convert(atts[i+1], lvcs_origin_lon_);
      else if (vcl_strcmp(atts[i], "origin_lat") == 0)
        convert(atts[i+1], lvcs_origin_lat_);
      else if (vcl_strcmp(atts[i], "origin_elev") == 0)
        convert(atts[i+1], lvcs_origin_elev_);
      else if (vcl_strcmp(atts[i], "lon_scale") == 0)
        convert(atts[i+1], lvcs_lon_scale_);
      else if (vcl_strcmp(atts[i], "lat_scale") == 0)
        convert(atts[i+1], lvcs_lat_scale_);
      else if (vcl_strcmp(atts[i], "local_XYZ_unit") == 0)
        convert(atts[i+1], lvcs_XYZ_unit_);
      else if (vcl_strcmp(atts[i], "geo_angle_unit") == 0)
        convert(atts[i+1], lvcs_geo_angle_unit_);
      else if (vcl_strcmp(atts[i], "local_origin_x") == 0)
        convert(atts[i+1], lvcs_local_origin_x_);
      else if (vcl_strcmp(atts[i], "local_origin_y") == 0)
        convert(atts[i+1], lvcs_local_origin_y_);
      else if (vcl_strcmp(atts[i], "theta_") == 0)
        convert(atts[i+1], lvcs_theta_);
    }
  }

  //Local Origin Tag
  else if (vcl_strcmp(name,LOCAL_ORIGIN_TAG)== 0) {
    double x,y,z;
    for (int i=0; atts[i]; i+=2) {
      if (vcl_strcmp(atts[i], "x") == 0)
        convert(atts[i+1], x);
      else if (vcl_strcmp(atts[i], "y") == 0)
        convert(atts[i+1], y);
      else if (vcl_strcmp(atts[i], "z") == 0)
        convert(atts[i+1], z);
    }
    origin_ = vgl_point_3d<double>(x,y,z);
  }

  //SCENE PATHS TAG
  else if (vcl_strcmp(name,SCENE_PATHS_TAG)== 0) {
    for (int i=0; atts[i]; i+=2) {
      if (vcl_strcmp(atts[i], "path") == 0)
        convert(atts[i+1], path_);
    }
  }
  
  //----------- APPEARANCE TAG -------------------------------------------------
  else if (vcl_strcmp(name, APM_TAG) == 0) {
    for (int i=0; atts[i]; i+=2) {
      vcl_string buff; 
      if (vcl_strcmp(atts[i], "apm") == 0) {
        convert(atts[i+1], buff);
        appearances_.push_back(buff); 
      }
      if (vcl_strcmp(atts[i], "num_illumination_bins") == 0) {
        convert(atts[i+1], num_illum_bins_);
      }
    }
  }
    
  //---------- BLOCK TAG -------------------------------------------------------
  else if (vcl_strcmp(name, BLOCK_TAG) == 0) {
    boxm2_block_metadata metadata;
    int idi, idj, idk;
    double ox, oy, oz;
    double dim_x, dim_y, dim_z;
    unsigned num_x, num_y, num_z;

    //iterate over attributes...
    for (int i=0; atts[i]; i+=2) {

      if (vcl_strcmp(atts[i], "id_i") == 0)
        convert(atts[i+1], idi);
      else if (vcl_strcmp(atts[i], "id_j") == 0)
        convert(atts[i+1], idj);
      else if (vcl_strcmp(atts[i], "id_k") == 0)
        convert(atts[i+1], idk);
      else if (vcl_strcmp(atts[i], "origin_x") == 0)
        convert(atts[i+1], ox);
      else if (vcl_strcmp(atts[i], "origin_y") == 0)
        convert(atts[i+1], oy);
      else if (vcl_strcmp(atts[i], "origin_z") == 0)
        convert(atts[i+1], oz);
      else if (vcl_strcmp(atts[i], "dim_x") == 0)
        convert(atts[i+1], dim_x);
      else if (vcl_strcmp(atts[i], "dim_y") == 0)
        convert(atts[i+1], dim_y);
      else if (vcl_strcmp(atts[i], "dim_z") == 0)
        convert(atts[i+1], dim_z);
      else if (vcl_strcmp(atts[i], "num_x") == 0)
        convert(atts[i+1], num_x);
      else if (vcl_strcmp(atts[i], "num_y") == 0)
        convert(atts[i+1], num_y);
      else if (vcl_strcmp(atts[i], "num_z") == 0)
        convert(atts[i+1], num_z);
      else if (vcl_strcmp(atts[i], "init_level")==0)
        convert(atts[i+1], metadata.init_level_);
      else if (vcl_strcmp(atts[i], "max_level")==0)
        convert(atts[i+1], metadata.max_level_);
      else if (vcl_strcmp(atts[i], "max_mb")==0)
        convert(atts[i+1], metadata.max_mb_);
      else if (vcl_strcmp(atts[i], "p_init")==0)
        convert(atts[i+1], metadata.p_init_);
    }
    metadata.id_ = boxm2_block_id(idi, idj, idk);
    metadata.local_origin_ = vgl_point_3d<double>(ox, oy, oz);
    metadata.sub_block_dim_ = vgl_vector_3d<double>(dim_x, dim_y, dim_z);
    metadata.sub_block_num_ = vgl_vector_3d<unsigned>(num_x, num_y, num_z);
    blocks_[metadata.id_] = metadata;
  }
}
