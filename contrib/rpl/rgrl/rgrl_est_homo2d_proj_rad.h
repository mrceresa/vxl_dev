#ifndef rgrl_est_homo2d_proj_rad_h_
#define rgrl_est_homo2d_proj_rad_h_

//:
// \file
// \author Gehua Yang
// \date   March 2007

#include <rgrl/rgrl_estimator.h>
#include <vnl/vnl_double_2.h>

//: homography2D transform estimator using the new proj function
//  defined in rgrl_est_proj_func
//
class rgrl_est_homo2d_proj_rad
  : public rgrl_nonlinear_estimator
{
public:
  //: Default constructor
  //
  rgrl_est_homo2d_proj_rad( vnl_vector_fixed<double, 2> const& to_camera_centre,
                            bool with_grad = true );

  //: Estimates homography transformation
  //
  // \sa rgrl_estimator::estimate
  //
  rgrl_transformation_sptr
  estimate( rgrl_set_of<rgrl_match_set_sptr> const& matches,
            rgrl_transformation const& cur_transform ) const;

  // import base class estimate function
  using rgrl_nonlinear_estimator::estimate;

  //: Type of transformation estimated by this estimator.
  const vcl_type_info& transformation_type() const;

  // Defines type-related functions
  rgrl_type_macro( rgrl_est_homo2d_proj_rad, rgrl_nonlinear_estimator );

private:

  vnl_double_2 to_camera_centre_;
  bool with_grad_;
};

#endif
