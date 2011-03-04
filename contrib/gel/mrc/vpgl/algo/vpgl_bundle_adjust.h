// This is gel/mrc/vpgl/algo/vpgl_bundle_adjust.h
#ifndef vpgl_bundle_adjust_h_
#define vpgl_bundle_adjust_h_
//:
// \file
// \brief Bundle adjustment sparse least squares functions
// \author Matt Leotta
// \date April 18, 2005
// \verbatim
//  Modifications
//   Mar 23, 2010  MJL - Separate file for least square function class
// \endverbatim



#include <vnl/vnl_vector.h>
#include <vnl/vnl_sparse_lst_sqr_function.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_point_3d.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <vpgl/algo/vpgl_bundle_adjust_lsqr.h>
#include <vcl_vector.h>



//: Static functions for bundle adjustment
class vpgl_bundle_adjust
{
 public:
  //: Constructor
  vpgl_bundle_adjust();
  //: Destructor
  ~vpgl_bundle_adjust();

  void set_use_weights(bool use_weights) { use_weights_ = use_weights; }
  void set_use_gradient(bool use_gradient) { use_gradient_ = use_gradient; }

  //: Return the ending error
  double end_error() const { return end_error_; }
  //: Return the starting error
  double start_error() const { return start_error_; }
  //: Return the number of iterations
  int num_iterations() const { return num_iterations_; }

  //: Return the raw camera parameters
  const vnl_vector<double>& cam_params() const { return a_; }
  //: Return the raw world point parameters
  const vnl_vector<double>& point_params() const { return b_; }

  //: Bundle Adjust
  bool optimize(vcl_vector<vpgl_perspective_camera<double> >& cameras,
                vcl_vector<vgl_point_3d<double> >& world_points,
                const vcl_vector<vgl_point_2d<double> >& image_points,
                const vcl_vector<vcl_vector<bool> >& mask);

  //: Write cameras and points to a file in VRML 2.0 for debugging
  static void write_vrml(const vcl_string& filename,
                         vcl_vector<vpgl_perspective_camera<double> >& cameras,
                         vcl_vector<vgl_point_3d<double> >& world_points);

 private:
  //: The bundle adjustment error function
  vpgl_bundle_adjust_lsqr* ba_func_;
  //: The last camera parameters
  vnl_vector<double> a_;
  //: The last point parameters
  vnl_vector<double> b_;

  bool use_weights_;
  bool use_gradient_;

  double start_error_;
  double end_error_;
  int num_iterations_;
};


#endif // vpgl_bundle_adjust_h_
