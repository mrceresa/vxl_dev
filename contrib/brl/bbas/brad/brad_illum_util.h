#ifndef brad_illum_util_h_
#define brad_illum_util_h_
//:
// \file
// \brief Various utility methods and classes for modeling illumination
// \author J.L. Mundy
// \date November 29, 2009
//
// \verbatim
//  Modifications
//   <none yet>
// \endverbatim

#include <vcl_string.h>
#include <vcl_vector.h>
#include <vcl_iostream.h>
#include <vcl_cmath.h>
#include <vnl/vnl_double_3.h>
#include <vnl/vnl_double_4.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_vector.h>

class brad_illum_util
{
 public:
  static bool
    load_surface_nhbds(vcl_string const& path,
                       vcl_vector<vcl_vector<vnl_matrix<float> > >& nhds);
  static bool
    load_illumination_dirs(vcl_string const& path,
                           vcl_vector<vnl_double_3>& ill_dirs);
static bool
  load_norm_intensities(vcl_string const& path,
                        vcl_vector<double>& norm_ints);


  //:
  // Given a set of illumination directions and image intensities,
  // find the surface normal and ambient illumination factor, where
  // both are weighted by the surface albedo. A minimum of 4 directions
  // are required.
  static bool solve_lambertian_model(vcl_vector<vnl_double_3> const& ill_dirs,
                                     vcl_vector<double> const& intensities,
                                     vnl_double_4& model_params,
                                     vcl_vector<double>& fitting_error);

  //:given an illumination direction and Lambertian model, predict the resulting image intensity
  static double expected_intensity(vnl_double_3 const& illum_dir,
                                   vnl_double_4 const& model_params);

  //:
  // Test the error in adding a new image observation to the model
  // based on a full solution with the new point. Leave one out method
  static void solution_error(vcl_vector<vnl_double_3> const& ill_dirs,
                             vcl_vector<double> const& intensities,
                             vcl_vector<double>& fitting_error);

  //: Find the nearest illumination orientation
  static double nearest_ill_dir(vcl_vector<vnl_double_3> const& ill_dirs,
                                vnl_double_3 const& dir);

// new model requires scene irradiance for each image
  static void solve_atmospheric_model(vnl_matrix<double> illum_dirs,
                                      vnl_matrix<double> intensities,
                                      vnl_vector<double> airlight,
                                      unsigned max_iterations,
                                      double max_fitting_error,
                                      vnl_vector<double>& scene_irrad,
                                      vnl_matrix<double>& surf_normals,
                                      vnl_vector<double>& reflectances);


  static void solve_atmospheric_model(vnl_matrix<double> illum_dirs,
                                      vnl_matrix<double> corr_intens,
                                      unsigned max_iterations,
                                      double max_fitting_error,
                                      vnl_matrix<double>& surf_normals,
                                      vnl_vector<double>& reflectances);

  static void solution_error(vnl_matrix<double> illum_dirs,
                             vnl_matrix<double> intensities,
                             vnl_vector<double> airlight,
                             vnl_vector<double> scene_irrad,
                             vnl_matrix<double> surf_normals,
                             vnl_vector<double> reflectances,
                             vnl_matrix<double>& fit_errors,
                             vnl_matrix<double>& pred_intensities);

  static void solution_error(vnl_matrix<double> illum_dirs,
                             vnl_matrix<double> corr_intens,
                             vnl_matrix<double> surf_normals,
                             vnl_vector<double> reflectances,
                             vnl_matrix<double>& fit_errors);


  static double atmos_prediction(vnl_double_3 const& ill_dir,
                                 double airlight,
                                 double scene_irrad,
                                 vnl_double_3 const& surface_norm,
                                 double reflectance
                                );

  static void display_illumination_space_vrml(vnl_matrix<double> illum_dirs,
                                              vcl_string const& path,
                                              vnl_double_3 degenerate_dir =
                                              vnl_double_3(0,0,0)
                                             );
};

#endif // brad_illum_util_h_