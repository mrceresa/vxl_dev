#ifndef BUGL_UNCERTAINTY_POINT_3D_H_
#define BUGL_UNCERTAINTY_POINT_3D_H_
//:
// \file
// \author Kongbin Kang
// \brief An abstract class for 3d point with uncertainty / probability distribution

#include <vgl/vgl_point_3d.h>

template<class T>
class bugl_uncertainty_point_3d : public vgl_point_3d<T>
{
 public:
  bugl_uncertainty_point_3d(T x, T y, T z) : vgl_point_3d<T>(x,y,z){}
  bugl_uncertainty_point_3d(vgl_point_3d<T> &p) : vgl_point_3d<T>(p) {}
  bugl_uncertainty_point_3d() : vgl_point_3d<T>(){}
 ~bugl_uncertainty_point_3d(){}

#if 0
  virtual double probability(vgl_point_3d<T> p)=0;
#endif
};

#endif
