/*
  fsm@robots.ox.ac.uk
*/
#ifdef __GNUC__
#pragma implementation
#endif
#include "vnl_sample.h"

#include <vcl_cmath.h>
#include <vcl_cstdlib.h>
#include <vxl_config.h>
#if VXL_STDLIB_HAS_DRAND48
// On SunPro 5.0, <cstdlib> does not
// declare drand48() but <stdlib.h> does.
# include <stdlib.h>
#endif

// -- return a random number uniformly drawn on [a, b)
double vnl_sample_uniform(double a, double b) 
{
#if VXL_STDLIB_HAS_DRAND48
  // it's your lucky day.
  double u = drand48(); // uniform on [0, 1)
#else
  // unlucky! perhaps your luck will change if you try
  // a different random number generator.

  // rand() is not always a good random number generator,
  // so use the following congruential random number generator - PVr
  static unsigned long seed = 12345;
  seed = (seed*16807)%2147483647L;
  double u = double(seed)/2147483711L;
#endif
  return (1.0-u)*a + u*b;
}

double vnl_sample_normal(double mean, double sigma) 
{
  double u     = vnl_sample_uniform(0, 1);
  double theta = vnl_sample_uniform(0, 2*3.1415926);
  
  double r = vcl_sqrt(-2*vcl_log(u));
  
  double x = r * vcl_cos(theta);
#ifdef fred
  double y = r * vcl_sin(theta);
#endif
  
  return mean + sigma * x;
}
