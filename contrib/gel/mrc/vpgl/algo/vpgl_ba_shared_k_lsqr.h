// This is gel/mrc/vpgl/algo/vpgl_ba_shared_k_lsqr.h
#ifndef vpgl_ba_shared_k_lsqr_h_
#define vpgl_ba_shared_k_lsqr_h_
//:
// \file
// \brief Bundle adjustment sparse least squares class for shared unknown intrinsics
// \author Matt Leotta
// \date March 24, 2010
// \verbatim



#include <vpgl/algo/vpgl_bundle_adjust_lsqr.h>


//: a class for bundle adjustment with shared intrinsic parameters
//  some shared intrinsic parameters can be estimated
//  currently only focal length is estimated
class vpgl_ba_shared_k_lsqr : public vpgl_bundle_adjust_lsqr
{
 public:
  //: Constructor
  // \note image points are not homogeneous because they require finite points
  //       to measure projection error
  vpgl_ba_shared_k_lsqr(const vpgl_calibration_matrix<double>& K,
                        const vcl_vector<vgl_point_2d<double> >& image_points,
                        const vcl_vector<vcl_vector<bool> >& mask);

  //: Constructor
  //  Each image point is assigned an inverse covariance (error projector) matrix
  // \note image points are not homogeneous because they require finite points 
  //       to measure projection error
  vpgl_ba_shared_k_lsqr(const vpgl_calibration_matrix<double>& K,
                        const vcl_vector<vgl_point_2d<double> >& image_points,
                        const vcl_vector<vnl_matrix<double> >& inv_covars,
                        const vcl_vector<vcl_vector<bool> >& mask);

  // Destructor
  virtual ~vpgl_ba_shared_k_lsqr() {}


  //: compute the Jacobian Aij
  virtual void jac_Aij(unsigned int i,
                       unsigned int j,
                       vnl_double_3x4 const& Pi,
                       vnl_vector<double> const& ai,
                       vnl_vector<double> const& bj,
                       vnl_vector<double> const& c,
                       vnl_matrix<double>& Aij);

  //: compute the Jacobian Bij
  virtual void jac_Bij(unsigned int i,
                       unsigned int j,
                       vnl_double_3x4 const& Pi,
                       vnl_vector<double> const& ai,
                       vnl_vector<double> const& bj,
                       vnl_vector<double> const& c,
                       vnl_matrix<double>& Bij);

  //: compute the Jacobian Cij
  virtual void jac_Cij(unsigned int i,
                       unsigned int j,
                       vnl_double_3x4 const& Pi,
                       vnl_vector<double> const& ai,
                       vnl_vector<double> const& bj,
                       vnl_vector<double> const& c,
                       vnl_matrix<double>& Cij);


  //: construct the j-th perspective camera
  //  from a pointer to the j-th parameters of \param b and parameters \param c
  virtual vgl_homg_point_3d<double>
  param_to_point(int j,
                 const double* bj,
                 const vnl_vector<double>& c) const;

  //: construct the j-th perspective camera
  //  from a pointer to the j-th parameters of \param b and parameters \param c
  virtual vnl_vector_fixed<double,4>
  param_to_pt_vector(int j,
                     const double* bj,
                     const vnl_vector<double>& c) const;

  //: construct the ith perspective camera 
  //  from a pointer to the i-th parameters of \param a and parameters \param c
  virtual vpgl_perspective_camera<double>
  param_to_cam(int i,
               const double* ai,
               const vnl_vector<double>& c) const;

  //: compute a 3x4 camera matrix of camera i
  //  from a pointer to the i-th parameters of \param a and parameters \param c
  virtual vnl_double_3x4 
  param_to_cam_matrix(int i,
                      const double* ai,
                      const vnl_vector<double>& c) const;

  //: Create the parameter vectors \p a and \p c from a vector of cameras
  static void
  create_param_vector(const vcl_vector<vpgl_perspective_camera<double> >& cameras,
                      vnl_vector<double>& a,
                      vnl_vector<double>& c);

  //: Create the parameter vector \p b from a vector of 3D points
  static vnl_vector<double>
  create_param_vector(const vcl_vector<vgl_point_3d<double> >& world_points);


 protected:
  //: The shared internal camera calibration
  mutable vpgl_calibration_matrix<double> K_;
  //: The shared internal camera calibration in matrix form
  mutable vnl_double_3x3 Km_;
};


#endif // vpgl_ba_shared_k_lsqr_h_
