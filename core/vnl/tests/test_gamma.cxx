// This is mul/vil2/tests/test_image_view.cxx
#include <vnl/vnl_gamma.h>
#include <vcl_iostream.h>
#include <testlib/testlib_test.h>

void test_gamma()
{
  TEST_NEAR("vnl_log_gamma(1)",      vnl_log_gamma(1),            0,  1e-8);
  TEST_NEAR("vnl_gamma(1))",         vnl_gamma(1),                1,  1e-8);

  // `true' values obtained from http://www.efunda.com/math/gamma/findgamma.cfm
  TEST_NEAR("vnl_gamma(0.5))",       vnl_gamma(0.5),          1.77245385091,  1e-10);
  TEST_NEAR("vnl_gamma(1.5))",       vnl_gamma(1.5),          0.88622692545,  1e-10);
  TEST_NEAR("vnl_gamma(10.7))",      vnl_gamma(10.7),         1799844.07893,  1e-5);

  TEST_NEAR("vnl_gamma_p(2,0)",      vnl_gamma_p(2,0),            0,  1e-8);
  TEST_NEAR("vnl_gamma_p(2,inf)",    vnl_gamma_p(2,1e9),        1.0,  1e-8);

  // Values from MATLAB
  TEST_NEAR("vnl_gamma_p(2.5,1.5)",  vnl_gamma_p(2.5,1.5),   0.30001416412137,  1e-8);
  TEST_NEAR("vnl_gamma_p(2.5,0.5)",  vnl_gamma_p(2.5,0.5),   0.03743422675270,  1e-8);
  // Next one not so accurate?
  TEST_NEAR("vnl_gamma_p(10,10)",    vnl_gamma_p(10,10),     0.54207028552815,  1e-6);

  TEST_NEAR("vnl_gamma_q(2,0)",      vnl_gamma_q(2,0),          1.0,  1e-8);
  TEST_NEAR("vnl_gamma_q(2,inf)",    vnl_gamma_q(2,1e9),          0,  1e-8);
  TEST_NEAR("vnl_erf(0)",            vnl_erf(0),                  0,  1e-8);
  TEST_NEAR("vnl_erf(inf)",          vnl_erf(1e9),              1.0,  1e-8);
  TEST_NEAR("vnl_erf(-inf)",         vnl_erf(-1e9),            -1.0,  1e-8);
}

MAIN( test_gamma )
{
  START( "vnl_gamma functions" );
  test_gamma();

  SUMMARY();
}
