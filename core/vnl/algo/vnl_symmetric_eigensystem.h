#ifndef vnl_symmetric_eigensystem_h_
#define vnl_symmetric_eigensystem_h_
#ifdef __GNUC__
#pragma interface
#endif

//:
// \file
// \brief Find eigenvalues of a symmetric matrix
//
//    vnl_symmetric_eigensystem_compute()
//    solves the eigenproblem $A x = \lambda x$, with $A$ symmetric.
//    The resulting eigenvectors and values are sorted in increasing order
//    so <CODE> V.column(0) </CODE> is the eigenvector corresponding to the smallest
//    the smallest eigenvalue.
//
//    As a matrix decomposition, this is $A = V D V^t$
//
//    Uses the EISPACK routine RS, which in turn calls TRED2 to reduce A
//    to tridiagonal form, followed by TQL2, to find the eigensystem.
//    This is summarized in Golub and van Loan, \S8.2.  The following are
//    the original subroutine headers:
//
// \remark TRED2 is a translation of the Algol procedure tred2,
//     Num. Math. 11, 181-195(1968) by Martin, Reinsch, and Wilkinson.
//     Handbook for Auto. Comp., Vol.ii-Linear Algebra, 212-226(1971).
//
// \remark This subroutine reduces a real symmetric matrix to a
//     symmetric tridiagonal matrix using and accumulating
//     orthogonal similarity transformations.
//
// \remark TQL2 is a translation of the Algol procedure tql2,
//     Num. Math. 11, 293-306(1968) by Bowdler, Martin, Reinsch, and Wilkinson.
//     Handbook for Auto. Comp., Vol.ii-Linear Algebra, 227-240(1971).
//
// \remark This subroutine finds the eigenvalues and eigenvectors
//     of a symmetric tridiagonal matrix by the QL method.
//     the eigenvectors of a full symmetric matrix can also
//     be found if  tred2  has been used to reduce this
//     full matrix to tridiagonal form.
//
// \author Andrew W. Fitzgibbon, Oxford RRG
// \date   29 Aug 96
//
// \verbatim
// Modifications
//  fsm@robots, 5 March 2000: templated
//  dac (Manchester) 28/03/2001: tidied up documentation
// \endverbatim

#include <vnl/vnl_matrix.h>
#include <vnl/vnl_diag_matrix.h>

//: Find eigenvalues of a symmetric matrix

bool vnl_symmetric_eigensystem_compute(vnl_matrix<float> const & A,
                                       vnl_matrix<float> & V,
                                       vnl_vector<float> & D);

//: Find eigenvalues of a symmetric matrix

bool vnl_symmetric_eigensystem_compute(vnl_matrix<double> const & A,
                                       vnl_matrix<double> & V,
                                       vnl_vector<double> & D);

//: Computes and stores the eigensystem decomposition of a symmetric matrix.

export template <class T>
class vnl_symmetric_eigensystem {
public:
  //: Solve real symmetric eigensystem $A x = \lambda x$
  vnl_symmetric_eigensystem(vnl_matrix<T> const & M);

protected:
  // need this here to get inits in correct order, but still keep gentex
  // in the right order.
  int n_;

public:
  //: Public eigenvectors.  After construction, the columns of V are the
  // eigenvectors, sorted by increasing eigenvalue, from most negative to
  // most positive.
  vnl_matrix<T> V;

  //: Public eigenvalues.  After construction,  D contains the
  // eigenvalues, sorted as described above.  Note that D is a vnl_diag_matrix,
  // and is therefore stored as a vcl_vector while behaving as a matrix.
  vnl_diag_matrix<T> D;

  //: Recover specified eigenvector after computation.
  vnl_vector<T> get_eigenvector(int i) const;

  //: Recover specified eigenvalue after computation.
  T             get_eigenvalue(int i) const;

  //: Convenience method to get least-squares nullvector.
  // It is deliberate that the signature is the same as on vnl_svd<T>.
  vnl_vector<T> nullvector() const { return get_eigenvector(0); }

  //: Return the matrix $V  D  V^\top$.  This can be useful if you've
  // modified $D$.  So an inverse is obtained using
  // \verbatim
  //   vnl_symmetric_eigensystem} eig(A);
  //   eig.D.invert_in_place}();
  //   vnl_matrix<double> Ainverse = eig.recompose();
  // \endverbatim

  vnl_matrix<T> recompose() const { return V * D * V.transpose(); }

  //: return the pseudoinverse.
  vnl_matrix<T> pinverse() const;

  //: return the square root, if positive semi-definite.
  vnl_matrix<T> square_root() const;

  //: return the inverse of the square root, if positive semi-definite.
  vnl_matrix<T> inverse_square_root() const;

  //: Solve LS problem M x = b
  vnl_vector<T> solve(vnl_vector<T> const & b);

  //: Solve LS problem M x = b
  void solve(vnl_vector<T> const & b, vnl_vector<T> * x);
};

#endif // vnl_symmetric_eigensystem_h_
