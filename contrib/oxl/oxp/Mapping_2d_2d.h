#ifndef Mapping_2d_2d_h_
#define Mapping_2d_2d_h_
#ifdef __GNUC__
#pragma interface
#endif
//:
//  \file
// \brief Mapping from R^2 -> R^2
//
//    Mapping_2d_2d is a base class for functions from R^2 -> R^2.
//    Operations such as warping can take a Mapping_2d_2d object
//    and therefore be able to perform arbitrary warps.
//
//    The user should subclass from Mapping_2d_2d, and write
//    the virtual functions implement_map, and implement_inverse_map.
//
// \author
//     Andrew W. Fitzgibbon, Oxford RRG, 16 Mar 97
//-----------------------------------------------------------------------------

#include <vnl/vnl_double_2.h>
struct Mapping_2d_2d {
  virtual ~Mapping_2d_2d();

  //: Map (x1, y1) to (x2, y2)
  void map(double x1, double y1, double* x2, double* y2) {
    implement_map(x1, y1, x2, y2);
  }

  //: x2 = map(x1);
  vnl_double_2 map(const vnl_vector<double>& in) {
    vnl_double_2 ret;
    implement_map(in[0], in[1], &ret[0], &ret[1]);
    return ret;
  }

  //: Inverse map (x2, y2) to (x1, y1)
  void inverse_map(double x1, double y1, double* x2, double* y2) {
    implement_inverse_map(x1, y1, x2, y2);
  }

  //: x1 = $map^{-1}(x1)$;
  vnl_double_2 inverse_map(const vnl_vector<double>& in) {
    vnl_double_2 ret;
    implement_inverse_map(in[0], in[1], &ret[0], &ret[1]);
    return ret;
  }

public:
  virtual void implement_map(double x1, double y1, double* x2, double* y2) = 0;
  virtual void implement_inverse_map(double x2, double y2, double* x1, double* y1) = 0;
};

#endif // Mapping_2d_2d_h_
