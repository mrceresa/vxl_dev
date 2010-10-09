// This is gel/mrc/vpgl/icam/icam_cost_func.h
#ifndef icam_cost_func_h_
#define icam_cost_func_h_
//:
// \file
// \brief A cost function for registering video cameras by minimizing square difference in intensities
// \author J.L. Mundy
// \date Sept 6, 2010
//
// \verbatim
//  Modifications
//   None
// \endverbatim

#include <vnl/vnl_vector.h>
#include <vnl/vnl_least_squares_function.h>
#include <vil/vil_image_view.h>
#include <icam/icam_depth_transform.h>

//: A cost function for registering video frames by minimizing square difference in intensities
class icam_cost_func : public vnl_least_squares_function
{
 public:
  //: Constructor
  icam_cost_func( const vil_image_view<float>& source_img,
                  const vil_image_view<float>& dest_img,
                  const icam_depth_transform& dt);


  //: The main function.
  //: The main function.
  //  Given the parameter vector x, compute the vector of residuals fx.
  //  Fx has been sized appropriately before the call.
  virtual void f(vnl_vector<double> const& x, vnl_vector<double>& fx);

  //debug purposes
  double error(vnl_vector_fixed<double, 3> rodrigues,
               vgl_vector_3d<double> trans,
               double min_allowed_overlap = 0.01);

  vcl_vector<double> error(vnl_vector<double> const& x,
                           unsigned param_index, double pmin,
                           double pmax, double pinc);
  //: the fraction of potential number of samples
  double frac_samples()
    {return (1.0*n_samples_)/(source_image_.ni()*source_image_.nj());}

  void samples(vnl_vector_fixed<double, 3> rodrigues,
               vgl_vector_3d<double> trans,
               vnl_vector<double>& trans_source,
               vnl_vector<double>& mask,
               vnl_vector<double>& dest);
               
 protected:
  vil_image_view<float> source_image_;
  vil_image_view<float> dest_image_;
  vnl_vector<double> dest_samples_;
  icam_depth_transform dt_;
  unsigned n_samples_;
};

#endif // icam_cost_func_h_
