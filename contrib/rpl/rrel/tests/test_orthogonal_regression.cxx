#include <vcl_iostream.h>

// #include <vnl/vnl_double_3.h>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_math.h>
#include <vnl/algo/vnl_svd.h>

#include <vbl/vbl_test.h>

#include <rrel/rrel_orthogonal_regression.h>


bool close( double x, double y ) { return vnl_math_abs(x-y) < 1.0e-6; }

int
main()
{
  vbl_test_start( "orthogonal regression" );
  vnl_vector<double> true_params(4);
  //  z = 0.02 x - 0.1 y + 10.0
  double a0 = 0.02, a1 = -0.1, a2 = 10.0;
  true_params[0] =  a0;
  true_params[1] =  a1;
  true_params[2] =  -1;
  true_params[3] =  a2;
  vnl_vector<double> par(4); 
  vnl_vector<double> norm_vect(3);
  norm_vect[0] = true_params[0];
  norm_vect[1] = true_params[1];
  norm_vect[2] = true_params[2];
  true_params /= norm_vect.two_norm();  //  4 component vector
  norm_vect /= norm_vect.two_norm();    //  3 component normal only
  const int num_pts=7;

  //  Build LinearRegression objects exercising both constructors and
  //  the two different options for the first constructor.
  vcl_vector<double> error( num_pts );
  error[0]=-0.001;
  error[1]=0;
  error[2]=0;
  error[3]=-0.0025;
  error[4]=0.007;
  error[5]=0;
  error[6]=-0.004;


  vcl_vector< vnl_vector<double> > pts(num_pts);
  vnl_vector<double> p(3), homog_p(4);
  homog_p[3] = 1.0;

  p.x() = 1.0;  p.y()=-0.5; p.z() = a0 * p.x() + a1* p.y() + a2;
  p += error[0] * norm_vect;
  homog_p[0] = p[0];   homog_p[1] = p[1];   homog_p[2] = p[2];
  pts[0] = homog_p;

  p.x() = 2.0;  p.y()=4.0;  p.z() = a0 * p.x() + a1* p.y() + a2;
  p += error[1] * norm_vect;
  homog_p[0] = p[0];   homog_p[1] = p[1];   homog_p[2] = p[2];
  pts[1] = homog_p;

  p.x()= 3.0;   p.y()=1.0;  p.z() = a0 * p.x() + a1* p.y() + a2;
  p += error[2] * norm_vect;
  homog_p[0] = p[0];   homog_p[1] = p[1];   homog_p[2] = p[2];
  pts[2] = homog_p;

  p.x()= -2.0;  p.y()=3.0;  p.z() = a0 * p.x() + a1* p.y() + a2;
  p += error[3] * norm_vect;
  homog_p[0] = p[0];   homog_p[1] = p[1];   homog_p[2] = p[2];
  pts[3] = homog_p;

  p.x()= 2.0;   p.y()=4.0;  p.z() = a0 * p.x() + a1* p.y() + a2;
  p += error[4] * norm_vect;
  homog_p[0] = p[0];   homog_p[1] = p[1];   homog_p[2] = p[2];
  pts[4] = homog_p;

  p.x()= 5.0;   p.y()=-4.0; p.z() = a0 * p.x() + a1* p.y() + a2;
  p += error[5] * norm_vect;
  homog_p[0] = p[0];   homog_p[1] = p[1];   homog_p[2] = p[2];
  pts[5] = homog_p;

  p.x()= 3.0;   p.y()=-2.0; p.z() = a0 * p.x() + a1* p.y() + a2;
  p += error[6] * norm_vect;
  homog_p[0] = p[0];   homog_p[1] = p[1];   homog_p[2] = p[2];
  pts[6] = homog_p;

  //
  //  The first set of tests are for the constructor, and parameter access methods.
  //
  vbl_test_begin( "ctor 1" );
  rrel_estimation_problem * lr1 = new rrel_orthogonal_regression( pts );
  vbl_test_perform( lr1 != 0 );

  
  vbl_test_begin( "num_points_to_instantiate (1)" );
  vbl_test_perform( lr1->num_samples_to_instantiate() == 3 );
  vbl_test_begin( "num_points_to_instantiate (3)" );
  vbl_test_perform( (int)lr1->num_samples() == num_pts );
  vbl_test_begin( "dtor (1)" );
  delete lr1;
  vbl_test_perform( true );
 
 
  //
  //  Test the residuals function.
  //
  vcl_vector<double> residuals;
  lr1 = new rrel_orthogonal_regression( pts );

  vbl_test_begin( "residuals" );
  lr1->compute_residuals( true_params, residuals );
  bool ok = (int(residuals.size()) == num_pts);
  for ( unsigned int i=0; i<residuals.size() && ok; ++ i )  ok = close( residuals[i], error[i] );
  vbl_test_perform( ok );

  //
  //  Test the weighted least squares function.
  //
  vcl_vector<double> wgts( num_pts );
  vnl_matrix<double> cofact;

  // Make weights so that the estimation is singular.
  wgts[0] = 0;   wgts[1] = 1;   wgts[2] = 0;    wgts[3] = 0;  
  wgts[4] = 0;   wgts[5] = 0;   wgts[6] = 0;
  vbl_test_begin( "weighted_least_squares_fit (singular)" );
  vbl_test_perform( !lr1->weighted_least_squares_fit( par, cofact, &wgts ) );

  // Ok.  This one should work.
  ok = lr1->weighted_least_squares_fit( par, cofact );
  // vcl_cout << "true_params = " << true_params
  //          << "\nest_params = " << par << vcl_endl;

  vnl_vector<double> diff1( par - true_params );
  vnl_vector<double> diff2( par + true_params );
  double err;
  if ( diff1.two_norm() < diff2.two_norm() )
    err = diff1.two_norm();
  else
    err = diff2.two_norm();

  
  vbl_test_begin( "weighted_least_squares_fit (ok) ");
  vbl_test_perform( ok && err <1e-2 ); 
  vcl_cout << "estimated params: " << par
           << ";  true params: " << true_params << vcl_endl
           << " error : " << err << vcl_endl;
 
  delete lr1;
  vbl_test_summary();
  return 0;
}
