// <begin copyright notice>
// ---------------------------------------------------------------------------
//
//                   Copyright (c) 1997 TargetJr Consortium
//               GE Corporate Research and Development (GE CRD)
//                             1 Research Circle
//                            Niskayuna, NY 12309
//                            All Rights Reserved
//              Reproduction rights limited as described below.
//                               
//      Permission to use, copy, modify, distribute, and sell this software
//      and its documentation for any purpose is hereby granted without fee,
//      provided that (i) the above copyright notice and this permission
//      notice appear in all copies of the software and related documentation,
//      (ii) the name TargetJr Consortium (represented by GE CRD), may not be
//      used in any advertising or publicity relating to the software without
//      the specific, prior written permission of GE CRD, and (iii) any
//      modifications are clearly marked and summarized in a change history
//      log.
//       
//      THE SOFTWARE IS PROVIDED "AS IS" AND WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
//      WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//      IN NO EVENT SHALL THE TARGETJR CONSORTIUM BE LIABLE FOR ANY SPECIAL,
//      INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND OR ANY
//      DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
//      WHETHER OR NOT ADVISED OF THE POSSIBILITY OF SUCH DAMAGES, OR ON
//      ANY THEORY OF LIABILITY ARISING OUT OF OR IN CONNECTION WITH THE
//      USE OR PERFORMANCE OF THIS SOFTWARE.
//
// ---------------------------------------------------------------------------
// <end copyright notice>
#ifndef vnl_svd_h_
#define vnl_svd_h_
#ifdef __GNUC__
#pragma interface
#endif
// .NAME        vnl_svd - Compute and cache the SVD of a Matrix.
// .LIBRARY     vnl/algo
// .HEADER	Numeircs Package
// .INCLUDE     vnl/algo/vnl_svd.h
// .FILE        vnl/algo/vnl_svd.cxx
// .EXAMPLE     vnl/examples/svd.cxx
//
// .SECTION Description
// @{ vnl_svd holds the singular value decomposition of an vnl\_matrix.
//
//    The class holds three matrices U, W, V such that the original matrix $M =
//    U W V^\top$.  The DiagMatrix W stores the singular values in decreasing
//    order.  The columns of U which correspond to the nonzero singular values
//    form a basis for range of M, while the columns of V corresponding to the
//    zero singular values are the nullspace.
//    
//    The SVD is computed at construction time, and enquiries may then be made
//    of the SVD.  In particular, this allows easy access to multiple
//    right-hand-side solves without the bother of putting all the RHS's into a
//    Matrix.
//
//    This class is supplied even though there is an existing vnl\_matrix method
//    for several reasons:
// \begin{itemize}
// \item It is more convenient to use as it manages all the storage for
//      the U,S,V matrices, allowing repeated queries of the same SVD
//      results.
//
// \item It avoids namespace clutter in the Matrix class.   While svd()
//      is a perfectly reasonable method for a Matrix, there are many other
//      decompositions that might be of interest, and adding them all would
//      make for a very large Matrix class.
//
// \item It demonstrates the holder model of compute class, implementing an
//      algorithm on an object without adding a member that may not be of
//      general interest.  A similar pattern can be used for other
//      decompositions which are not defined as members of the library Matrix
//      class.
//
// \item It extends readily to n-ary operations, such as generalized
//      eigensystems, which cannot be members of just one matrix.
//  \end{itemize}
//  @}
//
// .SECTION Author
//     Andrew W. Fitzgibbon, Oxford IERG, 15 Jul 96
//
// .SECTION Modifications
// F. Schaffalitzky, Oxford IESRG, 26 Mar 1999
//     1. The singular values are now stored as reals (not complexes) when T is
//        complex.
//     2. Fixed bug : for complex T, matrices have to be conjugated as well as 
//        transposed.
//

#include <vnl/vnl_numeric_traits.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_diag_matrix.h>
#include <vcl/vcl_iosfwd.h>

template <class T> class vnl_svd;
// templated friends must be declared to be templated :
template <class T> ostream& operator<<(ostream&, const vnl_svd<T>& svd);

template <class T>
class vnl_svd {
  // fsm : For an svd of vnl_complex<T>, the singular values are of type T, not
  // vnl_complex<T> (here T must be of real type : float,double or long double).
  // Must use 'typename' here for gcc 2.7.2.3 :
  typedef typename vnl_numeric_traits<T>::abs_t singval_t;

  // fsm : this is needed to return values of type singval_t, which is
  // from the traits class. gcc can only do that if the method is inline.
  // see determinant_magnitude() below.
  singval_t temp_value_for_gcc_hack;
  void            determinant_magnitude_aux () const;

public:
// -- @{
// Construct an vnl_svd<T> object from $m \times n$ matrix $M$.  The 
// vnl_svd<T> object contains matrices $U, W, V$ such that $U W V^\top = M$.
// \par
// Uses linpack routine DSVDC to calculate an ``economy-size'' SVD
// where the returned $U$ is the same size as $M$, while $W$ and $V$
// are both $n \times n$.  This is efficient for
// large rectangular solves where $m > n$, typical in least squares.
// @}
// The optional argument zero_out_tol is used to mark the zero singular
// values: If nonnegative, any s.v. smaller than zero_out_tol in
// absolute value is set to zero.  If zero_out_tol is negative, the
// zeroing is relative to |zero_out_tol| * sigma_max();
//
  vnl_svd(vnl_matrix<T> const &M, double zero_out_tol = 0.0);
 ~vnl_svd() {}

  // Data Access---------------------------------------------------------------
  void            zero_out_absolute(double tol = 1e-8); // sqrt(machine epsilon)
  void            zero_out_relative(double tol = 1e-8); // sqrt(machine epsilon)
  int             singularities () const { return W_.n() - rank(); }
  int             rank () const { return rank_; }
  singval_t       well_condition () const { return sigma_min()/sigma_max(); }
  singval_t       determinant_magnitude () const 
    { determinant_magnitude_aux(); return temp_value_for_gcc_hack; }
  singval_t       norm() const { return vnl_math::abs(sigma_max()); }

// -- Return the matrix U and the (i,j)th entry (to avoid svd.U()(i,j); ).
  vnl_matrix<T>      & U()       { return U_; }
  vnl_matrix<T> const& U() const { return U_; }
  T U(int i, int j) { return U_(i,j); }

// -- Get at the DiagMatrix (q.v.) of singular values, sorted from largest to smallest.
  vnl_diag_matrix<singval_t>       & W()             { return W_; }
  vnl_diag_matrix<singval_t> const & W() const       { return W_; }
  singval_t                   & W(int i, int j) { return W_(i,j); }
  singval_t                   & W(int i)        { return W_(i,i); }
  singval_t     sigma_max() const { return W_(0,0); }       // largest
  singval_t     sigma_min() const { return W_(n_-1,n_-1); } // smallest

// -- Return the matrix V.
  vnl_matrix<T>      & V()       { return V_; }
  vnl_matrix<T> const& V() const { return V_; }
  T V(int i, int j) { return V_(i,j); }
  
  //
  vnl_matrix<T> inverse () const;
  vnl_matrix<T> pinverse () const; // pseudo-inverse (for non-square matrix).
  vnl_matrix<T> tinverse () const;
  vnl_matrix<T> recompose () const;

  //
  vnl_matrix<T> solve (vnl_matrix<T> const& rhs) const;
  vnl_vector<T> solve (vnl_vector<T> const& rhs) const;
  void solve_preinverted(vnl_vector<T> const& rhs, vnl_vector<T>* out) const;

  //
  vnl_matrix<T> nullspace() const;
  vnl_matrix<T> left_nullspace() const;

  vnl_matrix<T> nullspace(int required_nullspace_dimension) const;
  vnl_matrix<T> left_nullspace(int required_nullspace_dimension) const;

  vnl_vector<T> nullvector() const;
  vnl_vector<T> left_nullvector() const;

  friend ostream& operator<<(ostream&, const vnl_svd<T>& svd);

private:

  // Data Members--------------------------------------------------------------

  int m_, n_;              // Size of M, local cache.
  vnl_matrix<T> U_;        // Columns Ui are basis for range of M for Wi != 0
  vnl_diag_matrix<singval_t> W_;// Singular values, sorted in decreasing order
  vnl_diag_matrix<singval_t> Winverse_;
  vnl_matrix<T> V_;       // Columns Vi are basis for nullspace of M for Wi = 0
  unsigned rank_;
  bool have_max_; 
  singval_t max_;
  bool have_min_;
  singval_t min_;
  double last_tol_;

protected:

  // Constructors/Destructors--------------------------------------------------

  // Disallow assignment until awf decides whether its a good idea
  vnl_svd(vnl_svd<T> const & that);
  vnl_svd<T>& operator=(vnl_svd<T> const & that);
};

#endif   // DO NOT ADD CODE AFTER THIS LINE! END OF DEFINITION FOR CLASS BaseSVD.
