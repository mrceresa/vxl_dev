#include <vcl_cmath.h>
#include <vcl_iostream.h>

#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_math.h>
#include <vnl/algo/vnl_svd.h>

#include <rrel/rrel_linear_regression.h>
#include <rrel/rrel_lms_obj.h>
#include <rrel/rrel_ran_sam_search.h>

#include <rrel/tests/similarity_from_matches.h>

#include <mbl/mbl_mz_random.h>

#include <vbl/vbl_test.h>

bool close( double x, double y ) { return vnl_math_abs(x-y) < 1.0e-6; }

double noise( double sigma )
{
  static mbl_mz_random rand;
  return rand.normal() * sigma;
}

bool
sample_ok( const vcl_vector<int>& indices, int num_pts )
{
  bool ok=true;
  for ( unsigned int i=0; i<indices.size() && ok; ++i ) {
    ok = 0<=indices[i] && indices[i]<num_pts;
    for ( unsigned int j=i+1; j<indices.size() && ok; ++ j )
      ok = (indices[i] != indices[j]);
  }
  return ok;
}


struct null_problem : public rrel_estimation_problem
{
  null_problem( unsigned int num_samples, unsigned int dof, unsigned int min_samples )
    : rrel_estimation_problem( dof, min_samples ), ns(num_samples) { }
  unsigned int num_samples() const { return ns; }
  void compute_residuals( const vnl_vector<double>& params,
                          vcl_vector<double>& residuals ) const { }
  unsigned int ns;
};


int
main()
{
  vbl_test_start( "rrel_ran_sam_search" );
  double a[] = { 10.0, 0.02, -0.1 };
  vnl_vector<double> true_params(3, 3, a);
  const int num_pts=12;

  //  Build LinearRegression objects.
  vcl_vector< vnl_vector<double> > pts(num_pts);
  vcl_vector< double > error(num_pts);
  vnl_vector<double> p(3);
  double x, y, z;

  x = 1.0; y=-0.5; error[0]=-0.001;  z= a[0] + a[1]*x + a[2]*y + error[0];
  p[0] = x; p[1]=y; p[2]=z;  pts[0]=p;

  x = 2.0;  y=4.0;  error[1]=0; z = a[0] + a[1]*x + a[2]*y + error[1];
  p[0] = x; p[1]=y; p[2]=z;  pts[1]=p;

  x = 3.0;  y=1.0;  error[2]=0; z = a[0] + a[1]*x + a[2]*y + error[2];
  p[0] = x; p[1]=y; p[2]=z;  pts[2]=p;

  x = -2.0;  y=3.0; error[3]=-0.0025;  z = a[0] + a[1]*x + a[2]*y + error[3];
  p[0] = x; p[1]=y; p[2]=z;  pts[3]=p;

  x = 2.0;  y=4.0;  error[4]=0.9;  z = a[0] + a[1]*x + a[2]*y + error[4];
  p[0] = x; p[1]=y; p[2]=z;  pts[4]=p;

  x = 5.0;  y=-4.0;  error[5]=0; z = a[0] + a[1]*x + a[2]*y + error[5];
  p[0] = x; p[1]=y; p[2]=z;  pts[5]=p;

  x = 3.0;  y=-2.0;  error[6]=-0.004; z = a[0] + a[1]*x + a[2]*y + error[6];
  p[0] = x; p[1]=y; p[2]=z;  pts[6]=p;

  x = 2.0;  y=-2.0;  error[7]=-0.8; z = a[0] + a[1]*x + a[2]*y + error[7];
  p[0] = x; p[1]=y; p[2]=z;  pts[7]=p;

  x = 3.0;  y=0.0;  error[8]=0.008; z = a[0] + a[1]*x + a[2]*y + error[8];
  p[0] = x; p[1]=y; p[2]=z;  pts[8]=p;

  x = -1.0;  y=-2.0;  error[9]=0.003; z = a[0] + a[1]*x + a[2]*y + error[9];
  p[0] = x; p[1]=y; p[2]=z;  pts[9]=p;

  x = 0.0;  y=0.0;  error[10]=0.3; z = a[0] + a[1]*x + a[2]*y + error[10];
  p[0] = x; p[1]=y; p[2]=z;  pts[10]=p;

  x = -1.0;  y=2.0;  error[11]=-0.0012; z = a[0] + a[1]*x + a[2]*y + error[11];
  p[0] = x; p[1]=y; p[2]=z;  pts[11]=p;

  //
  //  Create the linear regression problem and an lms objective
  //  function.
  //
  bool use_intercept=true;
  rrel_estimation_problem * lr = new rrel_linear_regression( pts, use_intercept );
  int dof = lr->num_samples_to_instantiate();
  rrel_objective* lms = new rrel_lms_obj( dof );
  vbl_test_begin( "ctor");
  rrel_ran_sam_search * ransam = new rrel_ran_sam_search();
  vbl_test_perform( ransam != 0 );
#if 0
  //  Test sampling by generating all parameters
  //
  vbl_test_begin( "generation of all samples" );
  ransam->set_gen_all_samples();
  //  ransam->print_params();

  int num_points=5;
  vcl_vector<int> indices(dof);
  ransam->calc_num_samples( num_points, dof );
  vbl_test_perform( ransam->samples_tested() == 10 );

  vbl_test_begin( "samples generated in order" );
  ransam->next_sample( 0, num_points, indices, dof );
  bool ok = indices[0]==0 && indices[1]==1 && indices[2]==2;
  ransam->next_sample( 1, num_points, indices, dof );
  ok = ok && (indices[0]==0 && indices[1]==1 && indices[2]==3);
  ransam->next_sample( 2, num_points, indices, dof );
  ok = ok && (indices[0]==0 && indices[1]==1 && indices[2]==4);
  ransam->next_sample( 3, num_points, indices, dof );
  ok = ok && (indices[0]==0 && indices[1]==2 && indices[2]==3);
  ransam->next_sample( 4, num_points, indices, dof );
  ok = ok && (indices[0]==0 && indices[1]==2 && indices[2]==4);
  ransam->next_sample( 5, num_points, indices, dof );
  ok = ok && (indices[0]==0 && indices[1]==3 && indices[2]==4);
  ransam->next_sample( 6, num_points, indices, dof );
  ok = ok && (indices[0]==1 && indices[1]==2 && indices[2]==3);
  ransam->next_sample( 7, num_points, indices, dof );
  ok = ok && (indices[0]==1 && indices[1]==2 && indices[2]==4);
  ransam->next_sample( 8, num_points, indices, dof );
  ok = ok && (indices[0]==1 && indices[1]==3 && indices[2]==4);
  ransam->next_sample( 9, num_points, indices, dof );
  ok = ok && (indices[0]==2 && indices[1]==3 && indices[2]==4);
  vbl_test_perform( ok );
  //  vcl_cout << "Test 2: " << ( ok ? "yes" : "NO" ) << vcl_endl;

  double max_outlier_frac = 0.4;
  double desired_prob_good = 0.99;
  int max_pops = 2;
  //  vcl_cout << "\nNow testing less than complete sampling:\n";
  vbl_test_begin( "1st probabilistic sampling" );
  ransam->set_sampling_params( max_outlier_frac, desired_prob_good,
                               max_pops);
  //  vcl_cout << "Parameters:\n";
  // ransam->print_params();
  ransam->calc_num_samples( num_pts, dof );
  vbl_test_perform(  ransam->samples_tested() == 83 );

  vbl_test_begin( "2nd probabilistic sampling" );
  max_outlier_frac=0.5;
  ransam->set_sampling_params( max_outlier_frac, desired_prob_good );
  // vcl_cout << "Parameters:\n";
  // ransam->print_params();
  ransam->calc_num_samples( num_pts, dof );
  vbl_test_perform( ransam->samples_tested() == 35 );

  vbl_test_begin( "generating samples" );
  ok = true;
  for ( int i=0; i<10 && ok; ++i ) {
    ransam->next_sample( i, num_pts, indices, dof );
    ok = sample_ok( indices, num_pts );
  }
  vbl_test_perform( ok );
#endif
  //
  //  Actually running the random sampler ...
  //
  bool ok;
//int trace_level=0;
  vbl_test_begin( "estimate succeed" );
  vbl_test_perform( ransam->estimate( lr, lms ) );
  vnl_vector<double> est_params = ransam->params();
  //  vcl_cout << "estimate = " << est_params
  //           << ", true model = " << true_params << vcl_endl
  //           << "scale = " << ransam->get_scale() << vcl_endl;
  ok = vnl_math_abs( est_params[0] - true_params[0] ) < 0.2
    && vnl_math_abs( est_params[1] - true_params[1] ) < 0.025
    && vnl_math_abs( est_params[2] - true_params[2] ) < 0.025;
  vbl_test_begin( "accurate estimate" );
  vbl_test_perform( ok );

  delete lr;

  double sigma = 0.1;
  vcl_vector<image_point_match> matches;
  vnl_vector<double> sim_params(4);
  sim_params[0] = 1.4;  sim_params[1] = -0.2;  sim_params[2] = 20;  sim_params[3] = -18;
  generate_similarity_matches( sim_params, sigma, matches );  // 20 matches, 13 points
  rrel_estimation_problem* match_prob = new similarity_from_matches( matches );
#if 0
  ransam->set_sampling_params( 0.5, 0.999, 1 );
  ransam->calc_num_samples( match_prob->num_data_points(), match_prob->num_correspondences_all(),
                            match_prob->num_points_to_instantiate() );
  vbl_test_begin( "num samples for matching problem" );
  vbl_test_perform( ransam->samples_tested() == 12 );
#endif
//trace_level=0;
  vbl_test_begin( "non-unique estimate succeed" );
  vbl_test_perform( ransam->estimate( match_prob, lms ) );
  est_params = ransam->params();
  vcl_cout << "similarity estimate = " << est_params
           << ", true similarity model = " << sim_params << vcl_endl
           << "scale = " << ransam->scale() << vcl_endl;
  ok = vnl_math_abs( est_params[0] - sim_params[0] ) < 0.025
    && vnl_math_abs( est_params[1] - sim_params[1] ) < 0.025
    && vnl_math_abs( est_params[2] - sim_params[2] ) < 1.0
    && vnl_math_abs( est_params[3] - sim_params[3] ) < 1.0;
  vbl_test_begin( "non-unique estimate accurate" );
  vbl_test_perform( ok );

  delete ransam;
  delete lms;

  vbl_test_summary();
  return 0;
}
