// This is gel/mrc/vpgl/vpgl_affine_camera.h
#ifndef vpgl_affine_camera_h_
#define vpgl_affine_camera_h_
//:
// \file
// \brief A class for the affine camera model.
// \author Thomas Pollard
// \date January 28, 2005
// \author Joseph Mundy, Matt Leotta, Vishal Jain
// \verbatim
//  Modifications
//  March 14, 2010 J.L. Mundy brought in virtual functions of proj_camera
//  that require special treatment for the affine case. Added a default
//  viewing distance to alow these methods to construct finite objects when
//  the camera center is infinity.
//  at infinity.
// \endverbatim

#include <vnl/vnl_fwd.h>
#include <vgl/vgl_fwd.h>
#include "vpgl_proj_camera.h"

template <class T>
class vpgl_affine_camera : public vpgl_proj_camera<T>
{
 public:

  //: Default constructor creates the canonical affine camera.
  vpgl_affine_camera();

  //: Construct from the first two rows.
  vpgl_affine_camera( const vnl_vector_fixed<T,4>& row1,
                      const vnl_vector_fixed<T,4>& row2 );

  //: Construct from a 3x4 matrix, sets the last row to 0001.
  // The bottom right entry had better not be 0.
  vpgl_affine_camera( const vnl_matrix_fixed<T,3,4>& camera_matrix );

  //: Construct from a ray direction, up vector, 3-d stare point: vgl interface
  // p projects to (u0, v0), su and sv calibration scale factors
  vpgl_affine_camera(vgl_vector_3d<T> ray, vgl_vector_3d<T> up, 
                     vgl_point_3d<T> stare_pt, T u0, T v0, T su, T sv);

  //: Construct from a ray direction, up vector, 3-d stare point: vnl interface
  //p projects to (u0, v0), su and sv are calibration scale factors
  vpgl_affine_camera(vnl_vector_fixed<T, 3> ray, vnl_vector_fixed<T, 3> up,
                     vnl_vector_fixed<T, 3> stare_pt, T u0, T v0, T su, T sv){
    vgl_vector_3d<T> ry(ray[0], ray[1], ray[2]), u(up[0], up[1], up[2]);
    vgl_point_3d<T> pt(stare_pt[0], stare_pt[1], stare_pt[2]);
    (*this) = vpgl_affine_camera<T>(ry, u, pt, u0, v0, su, sv);
  }

  virtual vcl_string type_name() const { return "vpgl_affine_camera"; }

  //: Set the top two rows.
  void set_rows( const vnl_vector_fixed<T,4>& row1,
                 const vnl_vector_fixed<T,4>& row2 );

  //: The following virtual functions require special treatment for the affine camera
  //: set a finite viewing distance to allow the methods below to 
  //  return finite objects 
  void set_viewing_distance(T dist){view_distance_ = dist;}

  //: Find the 3d ray that goes through the camera center. The finite point of the ray is at the viewing distance from the origin
virtual  vgl_homg_line_3d_2_points<T> backproject( const vgl_homg_point_2d<T>& image_point ) const;

  //: Find the world plane perpendicular to the camera rays at viewing distance from the origin
virtual  vgl_homg_plane_3d<T> principal_plane() const;

 private: 
 T view_distance_; // distance from origin along rays 
};

#endif // vpgl_affine_camera_h_
