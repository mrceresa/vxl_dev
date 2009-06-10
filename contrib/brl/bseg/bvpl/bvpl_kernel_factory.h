// This is contrib/brl/bseg/bvpl/bvpl_kernel_factory.h
#ifndef bvpl_kernel_base_h
#define bvpl_kernel_base_h

//:
// \file
// \brief  A base class for 3D kernel. The kernel is fully described by a rotation axis and an angle
//         The children of this classes must create a kernel on a "canonical form" i.e. with a default
//         axis of rotation and a complementary vector here called canonical_paralle_axis_. 
//         If this two vector are seen as points on the unit sphere, then they lie on the same 
//         parallel (constant polar angle). These vectors determine the zero rotation position as followos: 
//         1. The rotation that transforms the canonical_rotation_axis to a new specified rotation axis, 
//            is applied to the kernel (Lets call such rotation R).
//         2. The kernel is rotate around its new rototion such that R*canonical_parallel_axis achieves
//            the same same polar angle as the new rotation axis
//         Usage:
//         The user can create a kernel in its canonical form through children constructors
//         Then, the user call set_rotation_axis(), set_angle() and create() and if one whises to use 
//         another angle with the same axis, it suffies to call set_angle(angle) and create(). This will save computation 
//         time since the rotation_axis has already been set
//         Alternativly, the user can call create(axis, angle). This transforms the canonical kernel according
//         to new parameters
//         
// \author Isabel Restrepo mir@lems.brown.edu
// \date  5/29/09
//
// \verbatim
//  Modifications
//   <none yet>
// \endverbatim

#include "bvpl_kernel_iterator.h"
#include <vcl_string.h>
#include <vcl_map.h>
#include <vcl_utility.h>
#include <vnl/vnl_matrix_fixed.h>
#include <vbl/vbl_ref_count.h>
#include <vgl/algo/vgl_rotation_3d.h>

//: A simple class to hold bvpl_kernel_iterator and its bounding cube
class bvpl_kernel:public vbl_ref_count
{
public:
  //:Constructor
  bvpl_kernel(bvpl_kernel_iterator kernel, vgl_vector_3d<int> dim, vgl_point_3d<int> max, vgl_point_3d<int> min):
  kernel_(kernel),dim_(dim),max_(max),min_(min){};
  bvpl_kernel_iterator iterator(){return kernel_;}
  vgl_vector_3d<int> dim(){return dim_;}
  vgl_point_3d<int> min(){return min_;}
  vgl_point_3d<int> max(){return max_;}
  void print()
  {
    kernel_.begin();
    while(!kernel_.isDone()) {
      vgl_point_3d<int> coord =kernel_.index();
      char c = (char)((*kernel_).c_);
      vcl_cout << coord << "  " << int(c) << vcl_endl;
      ++kernel_;
    }
  }
  
private:
  bvpl_kernel_iterator kernel_;
  vgl_vector_3d<int> dim_;
  vgl_point_3d<int> min_;
  vgl_point_3d<int> max_;
};

typedef vbl_smart_ptr<bvpl_kernel> bvpl_kernel_sptr;

//: A factory of bvpl_kernels
class bvpl_kernel_factory
{
public:
  
  //: Returns a kernel described by class variables rotation_axis_ ,angle_
  //  The user can modified the axis and angle using set_rotation_axis() and set_angle()
  bvpl_kernel create();
  
  //: Returns a kernel described by inputs; rotation_axis and angle
  bvpl_kernel create(vnl_vector_fixed<float,3> rotation_axis, float angle);
  
  //: Sets the toration axis of this kernel
  void set_rotation_axis( vnl_vector_fixed<float,3> rotation_axis);
  
  //: Sets rotation angle of this kernel
  void set_angle(float angle){angle_ =angle;}
  
  //: Returns the current aligments axis
  vnl_vector_fixed<float,3> axis(){return rotation_axis_;}
  
  //: Return current rotation angle around axis()
  float angle(){return angle_;}
  
  //: Returns angular resolution around rotation axis
  unsigned angular_resolution()  {return angular_resolution_;}
  
  //: Return x-y-z maximum coordinate values
  vgl_point_3d<int> max_(){return max3d_;}
  
  //: Return x-y-z minimum coordinate values
  vgl_point_3d<int> min_(){return min3d_;}

  //: returns the rectangular dimensions around the kernel center
  vgl_vector_3d<int> dim();
 
  //: Save kernel as a Drishti raw file
  bool save_raw(vcl_string filename);
  
protected:
  
  typedef vcl_vector<vcl_pair<vgl_point_3d<float>, bvpl_kernel_dispatch> > kernel_type;
  
  //:The map of 3d positions and their symbols. This kernel has an axis of rotation, but it is always on zero-rotation position
  vcl_vector<vcl_pair<vgl_point_3d<float>, bvpl_kernel_dispatch> > kernel_;
  
  //:The map of 3d positions and their symbols in their cacnonical form (As specified by children)
  vcl_vector<vcl_pair<vgl_point_3d<float>, bvpl_kernel_dispatch> > canonical_kernel_;
  
  //:The rotation axis for canonical edge
  vnl_vector_fixed<float,3> canonical_rotation_axis_;
  
  //:Rotation axis of kernel_
  vnl_vector_fixed<float,3> rotation_axis_;
  
  
  // Paralles_axis_ define a coordinate system for each kernel. 
  // The vector (parallel_axis_ - rotation_axis_) defines the direction of the minor axis of the kernel
  // The zero-rotation for any rotation axis is that for which parallel_axis_ and rotation_axis_ have constant polar angle
  vnl_vector_fixed<float,3> canonical_parallel_axis_;

  //: Amounts rotation around rotation_axis_
  float angle_;
  
  //: Dimensions of the 3D grid
  vgl_point_3d<int> max3d_;
  
  //: Dimensions of the 3D grid
  vgl_point_3d<int> min3d_;
  
  //: Angular resolutions
  unsigned angular_resolution_;
  
  //: Creates canonical(default) kernel. It is decribed by a canonical axis of rotation and a canonical parallel axis
  //  This is the main function implemented by the children.
  virtual void create_canonical()=0;
  
  //: Rounds coordinates of kernel to the nearest integer
  bvpl_kernel_iterator interpolate(kernel_type const& kernel);
  
  //: Rotates "class-kernel_" around "class-rotation_axis_"  by an "angle"
  kernel_type rotate(float angle);
  
  //: Rotates "class-kernel_" using the given rotation matrix
  kernel_type rotate(vgl_rotation_3d<float> R);

};

#endif

