/*
  fsm@robots.ox.ac.uk
*/
#include <vcl/vcl_iostream.h>
#include <vnl/vnl_test.h>
#include <vnl/vnl_complex.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_sample.h>

void testvnl_sample()
{
  unsigned const N = 10000;
  double mu = 1.552;
  double sigma = 3.729;
  
  double X[N];
  for (unsigned i=0; i<N; ++i)
    X[i] = vnl_sample_normal(mu, sigma);

  // sample mean
  double X_bar = 0;
  for (unsigned i=0; i<N; ++i)
    X_bar += X[i];
  X_bar /= N;
  cerr << "actual mean : " << mu << endl;
  cerr << "sample mean : " << X_bar << endl;

  // sample standard deviation
  double sigma_bar = 0;
  for (unsigned i=0; i<N; ++i)
    sigma_bar += vnl_math_sqr(X[i] - X_bar);
  sigma_bar = sqrt(sigma_bar / (N-1));
  cerr << "actual standard deviation : " << sigma << endl;
  cerr << "sample standard deviation : " << sigma_bar << endl;
}

TESTMAIN(testvnl_sample);
