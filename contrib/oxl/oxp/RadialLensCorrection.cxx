//-*- c++ -*-------------------------------------------------------------------
#ifdef __GNUC__
#pragma implementation "RadialLensCorrection.h"
#endif
//
// Class: RadialLensCorrection
// Author: Andrew W. Fitzgibbon, Oxford RRG
// Created: 13 Jul 98
// Modifications:
//   980713 AWF Initial version.
//
//-----------------------------------------------------------------------------


#include "RadialLensCorrection.h"

#include <vcl/vcl_cassert.h>
#include <vcl/vcl_iostream.h>
#include <vnl/vnl_math.h>

RadialLensCorrection::RadialLensCorrection(double cx, double cy, double sx, double sy, double k2, double k4)
{
  init(cx, cy, sx, sy, k2, k4);
}

RadialLensCorrection::RadialLensCorrection(double k2)
{
  init(0, 0, 1, 1, k2, 0);
}

RadialLensCorrection::RadialLensCorrection(int w, int h, double k2)
{
  assert(0);
  init(w/2, h/2, 2.0, 1.0, 4 * k2 / (w*w + h*h), 0);
}

void RadialLensCorrection::init(double cx, double cy, double sx, double sy, double k2, double k4)
{
  _cx = cx;
  _cy = cy;
  _sx = sx;
  _sy = sy;
  _k2 = k2;
  _k4 = k4;
  // vbl_printf(cerr,"RDC params: c(%g,%g) scale %g x %g, K=[%g %g].\n",  cx, cy, sx, sy, k2, k4);

  _invsx = 1.0 / _sx;
  _invsy = 1.0 / _sy;

  // Approximately scale to maintain image size
  _invsx /= (1 + _k2);
  _invsy /= (1 + _k2);

}

void RadialLensCorrection::implement_map(double x1, double y1, double* x2, double* y2)
{
  // center
  double dx = _sx*(x1 - _cx);
  double dy = _sy*(y1 - _cy);

  // warp
  double r2 = dx*dx + dy*dy;
  double correction = 1 + _k2 * r2 + _k4 * r2*r2;
  // so now r2c = r2 * correction

  double dxc = dx * correction;
  double dyc = dy * correction;

  // decenter
  *x2 = dxc * _invsx + _cx;
  *y2 = dyc * _invsy + _cy;
}

void RadialLensCorrection::implement_inverse_map(double x2, double y2, double* x1, double* y1)
{
  // center
  double dxc = _sx * (x2 - _cx);
  double dyc = _sy * (y2 - _cy);

  // dewarp
  double r2c = dxc*dxc + dyc*dyc;
  if (r2c == 0 || _k2 == 0) {
    *x1 = x2;
    *y1 = y2;
    return;
  }
  double rc = sqrt(r2c);
  // So rc = r (1 + k2 r^2)
  // Thf r = fsolve(k2 r^3 + r - rc = 0)
  // from Devernay and Faugeras:
  assert (_k4 == 0);
  double c =  1 / _k2;
  double d = -rc * c;
  
  // r^3 + c * r + d = 0;
  double Q = -c / 3;
  double R = d / 2;

  double DELTA = R*R - Q*Q*Q;
  double r;
  if (DELTA < 0)  {
    double S = vnl_math_cuberoot(sqrt(R*R-DELTA));
    double T = atan(sqrt(-DELTA)/R) / 3.0;
    r = -S * cos(T) + S * sqrt(3.0) * sin(T);
    r = -r;
    double res = (r*(1 + _k2 * r * r) - rc);
    if (fabs(res) > 1e-11) {
      cerr << "RES = " << res << " -- call awf!\n";
    }
  } else {
    double t = -vnl_math_sgn(R) * vnl_math_cuberoot(fabs(R) + sqrt(DELTA));
    
    if (t == 0)
      r = 0;
    else
      r = t + Q / t;
    //cerr <<  "+RES = " << (r*(1 + _k2 * r * r) - rc) << " " << (fabs(t*t*t) - (fabs(R)  + sqrt(DELTA))) << endl;
  } 
  // end D & F


  double correction = r / rc;
  if (correction < 0) correction = -correction;

  double dx = dxc * correction;
  double dy = dyc * correction;

  // decenter
  *x1 = dx * _invsx + _cx;
  *y1 = dy * _invsy + _cy;
}
