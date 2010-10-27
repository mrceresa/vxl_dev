// This is core/vgl/algo/vgl_h_matrix_1d.h
#ifndef vgl_h_matrix_1d_h_
#define vgl_h_matrix_1d_h_
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif
//:
// \file
// \brief 2x2 line-to-line projectivity
//
// A class to hold a line-to-line projective transformation matrix
// and to perform common operations using it e.g. transfer point.
//
// \verbatim
//  Modifications
//   22 Oct 2002 - Peter Vanroose - added vgl_homg_point_2d interface
//   23 Oct 2002 - Peter Vanroose - using fixed 3x3 matrices throughout
//   22 Mar 2003 - J.L. Mundy - preparing for upgrade to vgl
//   13 Jun 2004 - Peter Vanroose - added set_identity() and projective_basis()
//   31 Jul 2010 - Peter Vanroose - made more similar to 2d and 3d variants
//   24 Oct 2010 - Peter Vanroose - mutators and setters now return *this
//   27 Oct 2010 - Peter Vanroose - moved Doxygen docs from .txx to .h
// \endverbatim

#include <vcl_vector.h>
#include <vnl/vnl_matrix_fixed.h>
#include <vgl/vgl_homg_point_1d.h>
#include <vcl_iosfwd.h>

//:
// A class to hold a line-to-line projective transformation matrix
// and to perform common operations using it e.g. transfer point.
template <class T>
class vgl_h_matrix_1d
{
  // Data Members--------------------------------------------------------------
 protected:
  vnl_matrix_fixed<T,2,2> t12_matrix_;

 public:

  // Constructors/Initializers/Destructors-------------------------------------

  vgl_h_matrix_1d() {}
 ~vgl_h_matrix_1d() {}
  //: Copy constructor
  vgl_h_matrix_1d(vgl_h_matrix_1d<T> const& M);
  //: Constructor from a 2x2 matrix, and implicit cast from vnl_matrix_fixed<T,2,2>
  vgl_h_matrix_1d(vnl_matrix_fixed<T,2,2> const& M);
  // product of two vgl_h_matrix_1ds
  vgl_h_matrix_1d(vgl_h_matrix_1d<T> const&,
                  vgl_h_matrix_1d<T> const&);
  //: Constructor from 2x2 C-array
  explicit vgl_h_matrix_1d(T const* t_matrix);
  //: Constructor from istream
  explicit vgl_h_matrix_1d(vcl_istream& s);
  //: Constructor from file
  explicit vgl_h_matrix_1d(char const* filename);
  //: Constructor - calculate homography between two sets of 1D points (minimum 3)
  vgl_h_matrix_1d(vcl_vector<vgl_homg_point_1d<T> > const& points1,
                  vcl_vector<vgl_homg_point_1d<T> > const& points2);

  // Operations----------------------------------------------------------------

  //: Return the transformed point given by $q = {\tt H} p$
  vgl_homg_point_1d<T> operator()(vgl_homg_point_1d<T> const& p) const;
  //: Return the transformed point given by $q = {\tt H} p$
  vgl_homg_point_1d<T> operator*(vgl_homg_point_1d<T> const& p) const { return (*this)(p); }

  //: Return the transformed point given by $p = {\tt H}^{-1} q$
  vgl_homg_point_1d<T> preimage(vgl_homg_point_1d<T> const& q) const;

  bool operator==(vgl_h_matrix_1d<T> const& M) const { return t12_matrix_ == M.get_matrix(); }

  // Data Access---------------------------------------------------------------

  //: Return the 2x2 homography matrix
  vnl_matrix_fixed<T,2,2> const& get_matrix() const { return t12_matrix_; }
  //: Fill t_matrix with contents of the 2x2 homography matrix
  void get (vnl_matrix_fixed<T,2,2>* t_matrix) const;
  //: Deprecated; use the vnl_matrix_fixed variant instead
  void get (vnl_matrix<T>* t_matrix) const;
  //: Fill t_matrix with contents of the 2x2 homography matrix
  void get (T* t_matrix) const;
  //: Return an element from the 2x2 homography matrix
  T get (unsigned int row_index, unsigned int col_index) const;
  //: Return the inverse homography
  vgl_h_matrix_1d get_inverse() const;

  //: transformation to projective basis (canonical frame)
  // Compute the homography that takes the input set of points to the
  // canonical frame.  The points act as the projective basis for
  // the canonical coordinate system.  In the canonical frame the points
  // have coordinates:
  // \verbatim
  //   p[0]p[1]p[2]
  //     1   0   1
  //     0   1   1
  // \endverbatim
  bool projective_basis(vcl_vector<vgl_homg_point_1d<T> > const& three_points);

  //: initialize the transformation to identity
  vgl_h_matrix_1d& set_identity() { t12_matrix_.set_identity(); return *this; }
  //: Set to 2x2 row-stored matrix
  vgl_h_matrix_1d& set(T const* t_matrix);
  //: Set to given 2x2 matrix
  vgl_h_matrix_1d& set(vnl_matrix_fixed<T,2,2> const& t_matrix);

  //: Read H from file
  bool read(char const* filename);
  //: Read H from vcl_istream
  bool read(vcl_istream& s);
};

//: Print H on vcl_ostream
template <class T> vcl_ostream& operator<<(vcl_ostream& s, vgl_h_matrix_1d<T> const& h);
//: Read H from vcl_istream
template <class T> vcl_istream& operator>>(vcl_istream& s, vgl_h_matrix_1d<T>&       h);


#define VGL_H_MATRIX_1D_INSTANTIATE(T) extern "please include vgl/algo/vgl_h_matrix_1d.txx first"

#endif // vgl_h_matrix_1d_h_
