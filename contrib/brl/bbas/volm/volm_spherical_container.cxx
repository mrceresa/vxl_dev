#include "volm_spherical_container.h"
//:
// \file
#include <vcl_cmath.h> // for std::floor() and std::fabs()

double RoundUp(double x, double unit)
{
  return vcl_ceil(x/unit)*unit;
}

double RoundUp2Vmin(double x, double vmin)
{
  if (x < 0 || vmin < 0) {
    vcl_cout << "ERROR: the voxel size has to be positive" << vcl_endl;
    return vmin;
  }
  if (x < 2*vmin)
    return vmin;
  else {
    unsigned int k = (unsigned int)vcl_floor(x/vmin);
    unsigned int count = 0;
    while (k != 1) {
      k /= 2;
      count++;
    }
    k = 1<<count;
    return k*vmin;
  }
}

volm_spherical_container::volm_spherical_container(float d_solid_ang, float voxel_min, float max_dist)
  : depth_offset_(0), ds_(d_solid_ang), vmin_(voxel_min)
 {
  dmax_ = (float)RoundUp(max_dist,vmin_);
  double vmin = vmin_;
  double d = 0;
  double dtor = vnl_math::pi_over_180;
  double tan_ds = vcl_tan(ds_ * dtor);
  //tan_ds = 1; // for debug purpose
  double vc;
  bool is_even = false; // the first layer has 4 voxels and hence half of it is an odd
  while (d <= dmax_) {
  depth_offset_map_[d] = depth_offset_;
    vc = d * tan_ds;
  if (vc >= 2*vmin && is_even) {
    vc = 2*vmin;
    vmin = vc;
  }
  else {
    vc = vmin;
  }
  // use current voxel size to mesh the space
  this->meshcurrentlayer(d, vc);
  // update and check current layer has even number of voxel along half
  d = d + vc;
  unsigned int N = (unsigned int)(d/vc);
  is_even = !(N%2);
  } // end of while
  
  //: create the depth_interval_map_  [0,d1) --> 1, [d1,d2) --> 2, ... [dn,dmax) --> n , n can be 253 max
  unsigned int i = 0;
  for (vcl_map<double, unsigned int>::iterator iter = depth_offset_map_.begin(); iter != depth_offset_map_.end(); iter++) {
    depth_interval_map_[iter->first] = (unsigned char)i;
    i++;
    if (i > 252) {
      vcl_cerr << "In volm_spherical_container::volm_spherical_container() -- number of depth intervals is larger than 253! Current indexing scheme does not support this container extent of " << dmax_ << "!\n";
      break;
    }
  }
}

//: find the interval of the given depth value, i.e. return interval such that d1 <= value < d2, interval ids are 1 greater than index in depth_offset_map_
unsigned char volm_spherical_container::get_depth_interval(double value)
{
  vcl_map<double, unsigned char>::iterator iter = depth_interval_map_.upper_bound(value);
  if (iter == depth_interval_map_.end()) {
    iter--;
    return iter->second + 1;
  }
  return iter->second;  
}

bool volm_spherical_container::meshcurrentlayer(double d, double v)
 {
// construct voxel structure at current depth d with current resolution v
  double hv = 0.5*v;
  unsigned int blk_num = 0;
  unsigned int upper_num = 0;
  unsigned int lower_num = 0;
  unsigned int left_num = 0;
  unsigned int right_num = 0;
  unsigned int back_num = 0;
  // construct the upper layer
  vcl_vector<vgl_point_3d<double> > inners_left;
  vcl_vector<vgl_point_3d<double> > uppers;
  vcl_vector<vgl_point_3d<double> > inners_back;
  double z = d + hv;
  double xleft = 0.0;
  double xcr = hv;
  double xcl = -hv;
  double ycr = hv;
  double ycl = -hv;
  double hw = d + v;
  double bdry = d + hv;
  while (xleft < hw) {
    double yleft = 0.0;
    while (yleft < hw) {
      voxels_.push_back(volm_voxel(v,vgl_point_3d<double>(xcr,ycr,z)));
      voxels_.push_back(volm_voxel(v,vgl_point_3d<double>(xcl,ycr,z)));
      voxels_.push_back(volm_voxel(v,vgl_point_3d<double>(xcl,ycl,z)));
      voxels_.push_back(volm_voxel(v,vgl_point_3d<double>(xcr,ycl,z)));
      upper_num += 4;
      blk_num += 4;
      uppers.push_back(vgl_point_3d<double>(xcr,ycr,z));
      uppers.push_back(vgl_point_3d<double>(xcl,ycr,z));
      uppers.push_back(vgl_point_3d<double>(xcl,ycl,z));
      uppers.push_back(vgl_point_3d<double>(xcr,ycl,z));
      if (xcr < bdry) {
        inners_left.push_back(vgl_point_3d<double>(xcr,ycr,z));
        inners_left.push_back(vgl_point_3d<double>(xcl,ycr,z));
        inners_left.push_back(vgl_point_3d<double>(xcl,ycl,z));
        inners_left.push_back(vgl_point_3d<double>(xcr,ycl,z));
      }
      if (ycr < bdry && xcr < bdry) {
        inners_back.push_back(vgl_point_3d<double>(xcr,ycr,z));
        inners_back.push_back(vgl_point_3d<double>(xcl,ycr,z));
        inners_back.push_back(vgl_point_3d<double>(xcl,ycl,z));
        inners_back.push_back(vgl_point_3d<double>(xcr,ycl,z));
      }
      ycr += v; ycl -= v;  yleft += v;
    }
  ycr = hv;  ycl = -hv;
  xcr += v; xcl -= v;  xleft += v;
  }
  // construct lower layer
  for (vcl_vector<vgl_point_3d<double> >::iterator upit = uppers.begin(); upit != uppers.end(); ++upit) {
    voxels_.push_back(volm_voxel(v,vgl_point_3d<double>(upit->x(),upit->y(),-1*upit->z())));
  lower_num++;
  blk_num++;
  }
  // construct left layer
  vnl_matrix_fixed<double,3,3> Q;
  Q[0][0] = 0.0;  Q[0][1] = 0.0;  Q[0][2] = -1.0;
  Q[1][0] = 0.0;  Q[1][1] = 1.0;  Q[1][2] =  0.0;
  Q[2][0] = 1.0;  Q[2][1] = 0.0;  Q[2][2] =  0.0;
  vnl_vector_fixed<double,3> O;
  O[0] = -d-hv;  O[1] = 0.0;  O[2] = d+hv;
  vnl_vector_fixed<double,3> x;
  vnl_vector_fixed<double,3> X;
  vnl_vector_fixed<double,3> xp;
  vnl_vector_fixed<double,3> Xp;
  vcl_vector<vgl_point_3d<double> > left;
  for (vcl_vector<vgl_point_3d<double> >::iterator it = inners_left.begin(); it != inners_left.end(); ++it) {
    X[0] = it->x();  X[1] = it->y();  X[2] = it->z();
  x = Q*(X-O);
  xp[0] = x[2];  xp[1] = x[1];  xp[2] = x[0];
  Xp = Q.transpose()*xp + O;
  voxels_.push_back(volm_voxel(v, vgl_point_3d<double>(Xp[0],Xp[1],Xp[2])));
  left_num++;
  blk_num++;
  left.push_back(vgl_point_3d<double>(Xp[0],Xp[1],Xp[2]));
  }
  // construct right layer
  for (vcl_vector<vgl_point_3d<double> >::iterator it = left.begin(); it != left.end(); ++it) {
    voxels_.push_back(volm_voxel(v,vgl_point_3d<double>(-1*it->x(),it->y(),it->z())));
    right_num++;
    blk_num++;
  }
  // construct back layer
  Q[0][0] = 1.0;  Q[0][1] = 0.0;  Q[0][2] =  0.0;
  Q[1][0] = 0.0;  Q[1][1] = 0.0;  Q[1][2] = -1.0;
  Q[2][0] = 0.0;  Q[2][1] = 1.0;  Q[2][2] =  0.0;
  O[0] = 0.0;  O[1] = -d-hv;  O[2] = d+hv;
  vcl_vector<vgl_point_3d<double> > back;
  for (vcl_vector<vgl_point_3d<double> >::iterator it = inners_back.begin(); it != inners_back.end(); ++it) {
    X[0] = it->x();  X[1] = it->y();  X[2] = it->z();
    x = Q*(X-O);
    xp[0] = x[0];  xp[1] = x[2];  xp[2] = x[1];
    Xp = Q.transpose()*xp + O;
    voxels_.push_back(volm_voxel(v, vgl_point_3d<double>(Xp[0],Xp[1],Xp[2])));
    back_num++;
    blk_num++;
    back.push_back(vgl_point_3d<double>(Xp[0],Xp[1],Xp[2]));
  }
  // use back layer to mirror front layer
  for (vcl_vector<vgl_point_3d<double> >::iterator it = back.begin(); it != back.end(); ++it) {
    voxels_.push_back(volm_voxel(v,vgl_point_3d<double>(it->x(),-1*it->y(),it->z())));
    blk_num++;
  }
  // update current offset
  vnl_vector_fixed<unsigned int,5> surface_index;
  surface_index[0] = upper_num;
  surface_index[1] = lower_num;
  surface_index[2] = left_num;
  surface_index[3] = right_num;
  surface_index[4] = back_num;
  depth_blk_num_map_[d] = surface_index;
  depth_offset_ += blk_num;
  return true;
}

#if 0
double volm_spherical_container::center_depth(volm_voxel voxel)
{
  // given voxel center (x,y,z), return its depth = max(x,y,z)
  double fab_x = vcl_fabs(voxel.center_.x());
  double fab_y = vcl_fabs(voxel.center_.y());
  double fab_z = vcl_fabs(voxel.center_.z());
  double depth = fab_x;
  if (depth < fab_y)
    depth = fab_y;
  if (depth < fab_z)
    depth = fab_z;
  depth -= 0.5*voxel.resolution_;
  return depth;
}

unsigned volm_spherical_container::fetch_voxel(double dist, double theta, double phi)
{
  return 0;
}
#endif

void volm_spherical_container::draw_helper(vcl_ofstream& ofs, double dmin)
{
  // write a world center and world axis
  double rad = 0.5*vmin_;
  vcl_vector<volm_voxel>::iterator it = voxels_.end();
  --it;
  double axis_len = (dmax_ + it->resolution_)*1.2;
  vgl_point_3d<float> cent(0.0,0.0,0.0);
  vgl_point_3d<double> cent_ray(0.0,0.0,0.0);
  vgl_vector_3d<double> axis_x(1.0, 0.0, 0.0);
  vgl_vector_3d<double> axis_y(0.0, 1.0, 0.0);
  vgl_vector_3d<double> axis_z(0.0, 0.0, 1.0);
  vgl_sphere_3d<float> sp((float)cent.x(), (float)cent.y(), (float)cent.z(), (float)rad);
  bvrml_write::write_vrml_sphere(ofs, sp, 1.0f, 0.0f, 0.0f, 0.0f);
  bvrml_write::write_vrml_line(ofs, cent_ray, axis_x, (float)axis_len, 1.0f, 0.0f, 0.0f);
  bvrml_write::write_vrml_line(ofs, cent_ray, axis_y, (float)axis_len, 0.0f, 1.0f, 0.0f);
  bvrml_write::write_vrml_line(ofs, cent_ray, axis_z, (float)axis_len, 1.0f, 1.0f, 1.0f);
  // write the voxel structure
  for (vcl_vector<volm_voxel>::iterator it = voxels_.begin(); it != voxels_.end(); ++it) {
    double x = vcl_fabs(it->center_.x());
    double y = vcl_fabs(it->center_.y());
    double z = vcl_fabs(it->center_.z());
    double d = x;
    if (d < y) d = y;
    if (d < z) d = z;
    if (d > dmin) {
      double v_len = it->resolution_;
      vgl_point_3d<double> vc(it->center_.x(), it->center_.y(), it->center_.z());
      vgl_box_3d<double> box(vc, v_len, v_len, v_len, vgl_box_3d<double>::centre);
      bvrml_write::write_vrml_box(ofs, box, 1.0f, 1.0f, 0.0f, (float)(d/dmax_+0.2));
      bvrml_write::write_vrml_wireframe_box(ofs, box, 0.0f, 0.0f, 1.0f, 1);
    }
  }
}

void volm_spherical_container::draw_template(vcl_string vrml_file_name, double dmin)
{
  vcl_ofstream ofs(vrml_file_name.c_str());
  // write the header
  bvrml_write::write_vrml_header(ofs);
  draw_helper(ofs, dmin);
  ofs.close();
}

//: paint the wireframe of the voxels with the given ids with the given color
void volm_spherical_container::draw_template_painted(vcl_string vrml_file_name, double dmin, vcl_vector<unsigned int>& ids, float r, float g, float b, float trans)
{
  vcl_ofstream ofs(vrml_file_name.c_str());
  // write the header
  bvrml_write::write_vrml_header(ofs);
  //draw_helper(ofs, dmin);
  // draw the extras
  
  for (unsigned i = 0; i < ids.size(); i++) {
    
    double x = vcl_fabs(voxels_[ids[i]].center_.x());
    double y = vcl_fabs(voxels_[ids[i]].center_.y());
    double z = vcl_fabs(voxels_[ids[i]].center_.z());
    double d = x;
    if (d < y) d = y;
    if (d < z) d = z;
    if (d > dmin) {
      double v_len = voxels_[ids[i]].resolution_;
      vgl_point_3d<double> vc(voxels_[ids[i]].center_.x(), voxels_[ids[i]].center_.y(), voxels_[ids[i]].center_.z());
      vgl_box_3d<double> box(vc, v_len, v_len, v_len, vgl_box_3d<double>::centre);
      bvrml_write::write_vrml_box(ofs, box, r, g, b, trans);
      bvrml_write::write_vrml_wireframe_box(ofs, box, r, g, b, 1);
    }
  }
  
  ofs.close();
}

//: paint the wireframe of the voxels with the given ids with the given color
void volm_spherical_container::draw_template_vis_prob(vcl_string vrml_file_name, double dmin, vcl_vector<char>& ids)
{
  vcl_ofstream ofs(vrml_file_name.c_str());
  // write the header
  bvrml_write::write_vrml_header(ofs);
  //draw_helper(ofs, dmin);
  // draw the extras
  if (voxels_.size() != ids.size()) {
    vcl_cerr << "In volm_spherical_container::draw_template_vis_prob() -- passed vector is not of the same size with voxels vector!\n";
    return;
  }
  for (unsigned i = 0; i < voxels_.size(); i++) {
    
    double x = vcl_fabs(voxels_[i].center_.x());
    double y = vcl_fabs(voxels_[i].center_.y());
    double z = vcl_fabs(voxels_[i].center_.z());
    double d = x;
    if (d < y) d = y;
    if (d < z) d = z;
    if (d > dmin) {
      double v_len = voxels_[i].resolution_;
      vgl_point_3d<double> vc(voxels_[i].center_.x(), voxels_[i].center_.y(), voxels_[i].center_.z());
      vgl_box_3d<double> box(vc, v_len, v_len, v_len, vgl_box_3d<double>::centre);
      // VIS_OCC = 0, VIS_UNOCC = 1, NONVIS_UNKNOWN = 2 
      if (ids[i] == 0) {
        bvrml_write::write_vrml_box(ofs, box, 0.0f, 0.0f, 0.0f, 0.8f);  // 1 is completely transparent, 0 is completely opaque
        //bvrml_write::write_vrml_wireframe_box(ofs, box, 0.0f, 0.0f, 0.0f, 1.0f); // blacks will be visible and occupied
      } else if (ids[i] == 1) {
        //bvrml_write::write_vrml_box(ofs, box, 0.0f, 0.0f, 1.0f, 0.9f);  
        //bvrml_write::write_vrml_wireframe_box(ofs, box, 0.0f, 0.0f, 1.0f, 1.0f); // blues will be visible and unoccupied
      } else if (ids[i] == 2) {
        //bvrml_write::write_vrml_box(ofs, box, 0.0f, 0.0f, 0.0f, 1.0f);
        //bvrml_write::write_vrml_wireframe_box(ofs, box, 0.0f, 1.0f, 0.0f, 0.2f); // greens will be non visible
      }
    }
  }
  
  ofs.close();
}



//: return the offset and depth of the last layer with given resolution
void volm_spherical_container::last_res(double res, unsigned int& offset, unsigned int& end_offset, double& depth)
{
  // number of depth layers in the container:
  offset = 0;
  depth = 0.0;
  for (vcl_map<double, unsigned int>::iterator iter = depth_offset_map_.begin(); iter != depth_offset_map_.end(); iter++) {
    if (voxels_[iter->second].resolution_ > res) {
      end_offset = iter->second;
      break;
    }
#ifdef DEBUG
    vcl_cout << iter->first << ' ' << iter->second << vcl_endl;
#endif
    offset = iter->second;
    depth = iter->first;
  }
}

//: return the offset and depth of the first layer with the given resolution
void volm_spherical_container::first_res(double res, unsigned int& offset, unsigned int& end_offset, double& depth)
{
  // number of depth layers in the container:
  offset = 0;
  depth = 0.0;
  for (vcl_map<double, unsigned int>::iterator iter = depth_offset_map_.begin(); iter != depth_offset_map_.end(); iter++) {
    if (voxels_[iter->second].resolution_ < res)
      continue;
    offset = iter->second;
    depth = iter->first;
    iter++;
    if (iter != depth_offset_map_.end())
      end_offset = iter->second;
    else
      end_offset = (unsigned int)voxels_.size();
    break;
  }
}

