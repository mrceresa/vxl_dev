//-*- c++ -*-------------------------------------------------------------------
//
// Class: BaseSVD
// Author: Andrew W. Fitzgibbon, Oxford RRG
// Created: 09 May 97
// Modifications:
//   970509 AWF Initial version from SVD.C
//
//-----------------------------------------------------------------------------

#include <vnl/vnl_complex.h> // "system header" must come first.
#include "vnl_svd.h"
#include <vnl/vnl_fortran_copy.h>
#include <vnl/algo/linpack_svd.h>

#include <fstream.h>
#include <stdlib.h> // for abort();

//--------------------------------------------------------------------------------

static bool test_heavily = false;

// awf : REMEMBER TO CHANGE DOC COMMENT IN vnl_svd.h if you change anything
// fsm : errh... which one of them ?
template <class T>
vnl_svd<T>::vnl_svd(const vnl_matrix<T>& M, double zero_out_tol):
  m_(M.rows()),
  n_(M.columns()),
  U_(m_, n_),
  W_(n_),
  Winverse_(n_),
  V_(n_, n_)
{
  // fsm added this sanity check
  if (m_==0 || n_==0) {
    cerr << __FILE__ " : matrix M has size " << M.rows() << 'x' << M.columns() << endl
	 << "  That makes no sense!" << endl
	 << "  &M = " << (void*)&M << endl;
    abort();
  }

  {
    int n = M.rows();
    int p = M.columns();
    int mm = vnl_math::min(n+1,p);

    // Copy source matrix into fortran storage
    // SVD is slow, don't worry about the cost of this transpose.
    vnl_fortran_copy<T> X(M);

    // Make workspace vectors.
    vnl_vector<T> work(n, T(0));
    vnl_vector<T> uspace(n*p, T(0));
    vnl_vector<T> vspace(p*p, T(0));
    vnl_vector<T> wspace(mm, T(0)); // complex fortran routine actually _wants_ complex W!
    vnl_vector<T> espace(p, T(0));

    // Call Linpack SVD
    int info = 0;
    linpack_svd((T*)X, n, n, p,
	   wspace.data_block(),
	   espace.data_block(),
	   uspace.data_block(), n,
	   vspace.data_block(), p,
	   work.data_block(),
	   21, &info);

    // Error return?
    if (info != 0) {
      M.assert_finite();
//       *** this warning is bogus! the singular values are OK!
//       and the matrix is close to identity, most of the times.
//       cerr << "*** Warning vnl_svd<T>::vnl_svd<T>() ***\n";
//       cerr << " About " << info << " singular values are wrong\n";
//       MatOps::matlab_print(cerr, M, "M");
    }

    // Copy fortran outputs into our storage
    {
      const T *d = uspace.data_block();
      for(int j = 0; j < p; ++j)
	for(int i = 0; i < n; ++i)
	  U_(i,j) = *d++;
    }

    for(int j = 0; j < mm; ++j)
      W_(j, j) = vnl_math::abs(wspace(j)); // we get rid of complexness here.
    
    for(int j = mm; j < n_; ++j)
      W_(j, j) = 0;

    {
      const T *d = vspace.data_block();
      for(int j = 0; j < p; ++j)
	for(int i = 0; i < p; ++i)
	  V_(i,j) = *d++;
    }
  }

  if (test_heavily) {
    // Test that recomposed matrix == M
    double recomposition_residual = vnl_math::abs((recompose() - M).fro_norm());
    double n = vnl_math::abs(M.fro_norm());
    double thresh = m_ * vnl_math::eps * n;
    if (recomposition_residual > thresh) {
      cerr << "vnl_svd<T>::vnl_svd<T>() -- Warning, recomposition_residual = "
	   << recomposition_residual << endl;
      cerr << "fro_norm(M) = " << n << endl; 
      cerr << "eps*fro_norm(M) = " << thresh << endl;
      cerr << "Press return to continue\n";
      char x[1];
      cin.get(x, 1, '\n');
    }
  }

  if (zero_out_tol >= 0)
    // Zero out small sv's and update rank count.
    zero_out_absolute(zero_out_tol);
  else
    // negative tolerance implies relative to max elt.
    zero_out_relative(-zero_out_tol);
}




// Copy ctor
template <class T>
vnl_svd<T>::vnl_svd(const vnl_svd<T>& that)
{
  operator= (that);
}




// Assignment
template <class T>
vnl_svd<T>& vnl_svd<T>::operator=(const vnl_svd<T>& that)
{
  U_ = that.U_;
  W_ = that.W_;
  Winverse_ = that.Winverse_;
  V_ = that.V_;
  rank_ = that.rank_;
  return *this;
}




#ifdef xWIN32
#warning "I think I may have fixed this, remove if so -awf"

// this is a cool hack for the microsoft compiler
// if it does not see these things being cout'ed 
// there is an error in operator<<!  It claims that
// there is no way to do that for matrix and diagmatrix
static void foo()
{ 
  DiagMatrix<double> DD;
  DiagMatrix<float> FF;
  cout << DD  << FF;
  vnl_matrix<float> U;//  =  svd.U_;
  vnl_matrix<double> dU;//  = svd.U_;
  cout << U << dU;
}
#endif




template <class T>
ostream& operator<<(ostream& s, const vnl_svd<T>& svd)
{
  s << "vnl_svd<T>:\n";
  //s << "M = [\n" << M << "]\n";
  s << "U = [\n" << svd.U() << "]\n";
  s << "W = " << svd.W() << "\n";
  s << "V = [\n" << svd.V() << "]\n";
  s << "rank = " << svd.rank() << endl;
  return s;
}

//-----------------------------------------------------------------------------
// Chunky bits.

// -- find weights below threshold tol, zero them out, and update W_ and
// Winverse_
template <class T> 
void
vnl_svd<T>::zero_out_absolute(double tol)
{
  last_tol_ = tol;
  rank_ = W_.n();
  for (unsigned k = 0; k < W_.n(); k++) {
    singval_t& weight = W_(k, k);
    if (vnl_math::abs(weight) <= tol) {
      Winverse_(k,k) = 0;
      weight = 0;
      --rank_;
    } else {
      Winverse_(k,k) = singval_t(1.0)/weight;
    }
  }
}

// -- find weights below tol*max(w) and zero them out
template <class T> void vnl_svd<T>::zero_out_relative(double tol) // sqrt(machine epsilon)
{
  zero_out_absolute(tol * vnl_math::abs(sigma_max()));
}


/*
// -- return the largest singular value
template <class T>
vnl_svd<T>::singval_t vnl_svd<T>::sigma_max() const
{
  return W_(0,0);
}



// -- return the smallest singular value
template <class T>
vnl_svd<T>::singval_t vnl_svd<T>::sigma_min() const
{
  return W_(n_-1,n_-1);
}



// -- Return ratio of smallest/largest singular value
template <class T>
vnl_svd<T>::singval_t vnl_svd<T>::well_condition () const
{
  return sigma_min() / sigma_max();
}

*/

// -- Calculate determinant as product of diagonals in W.
template <class T>
void vnl_svd<T>::determinant_magnitude_aux () const
{
  singval_t product = W_(0, 0);
  for (unsigned long k = 1; k < W_.columns(); k++)
    product *= W_(k, k);

  ((vnl_svd<T>*)this)->temp_value_for_gcc_hack=product; 
  //return vnl_math::abs(product);
}

// -- Recompose SVD to U*W*V'
template <class T>
vnl_matrix<T> vnl_svd<T>::recompose() const
{
  vnl_matrix<T> W(W_.rows(),W_.columns());
  W.fill(0);
  for (unsigned i=0;i<rank_;i++)
    W(i,i)=W_(i,i);

  return U_*W*V_.conjugate_transpose();
}


template <class T>
vnl_matrix<T> vnl_svd<T>::inverse() const
{
  return pinverse();
}


// -- Calculate pseudo-inverse.
template <class T>
vnl_matrix<T> vnl_svd<T>::pinverse()  const
{
  vnl_matrix<T> Winverse(Winverse_.rows(),Winverse_.columns());
  Winverse.fill(0);
  for (unsigned i=0;i<rank_;i++)
    Winverse(i,i)=Winverse_(i,i);

  return V_ * Winverse * U_.conjugate_transpose();
}



// -- Calculate inverse of transpose.
template <class T>
vnl_matrix<T> vnl_svd<T>::tinverse()  const
{
  vnl_matrix<T> Winverse(Winverse_.rows(),Winverse_.columns());
  Winverse.fill(0);
  for (unsigned i=0;i<rank_;i++)
    Winverse(i,i)=Winverse_(i,i);

  return U_ * Winverse * V_.conjugate_transpose();
}



// -- Solve the matrix equation M X = B, returning X
template <class T>
vnl_matrix<T> vnl_svd<T>::solve(const vnl_matrix<T>& B)  const
{
  vnl_matrix<T> x;                                      // solution matrix
  if (U_.rows() < U_.columns()) {                       // augment y with extra rows of
    vnl_matrix<T> yy(U_.rows(), B.columns(), 0);        // zeros, so that it matches
    yy.update(B);                                       // cols of u.transpose. ???
    x = U_.conjugate_transpose() * yy;
  } else
    x = U_.conjugate_transpose() * B;
  unsigned long i, j;
  for (i = 0; i < x.rows(); i++) {                      // multiply with diagonal 1/W
    T weight = W_(i, i);
    if (weight != vnl_numeric_traits<T>::zero)
      weight = T(1.0) / weight;
    for (j = 0; j < x.columns(); j++)
      x(i, j) *= weight;
  }
  x = V_ * x;                                           // premultiply with v.
  return x;
}

// -- Solve the matrix-vector system M x = y, returning x.
template <class T>
vnl_vector<T> vnl_svd<T>::solve(const vnl_vector<T>& y)  const
{
  // fsm sanity check :
  if (! (y.size() == U_.rows()) ) {
    cerr << "y =" << y << endl;
    cerr << "m_=" << m_ << endl;
    cerr << "n_=" << n_ << endl;
    cerr << "U_=" << endl << U_;
    cerr << "V_=" << endl << V_;
    cerr << "W_=" << endl << W_;
  }
  vnl_vector<T> x(V_.rows());                   // Solution matrix.
  if (U_.rows() < U_.columns()) {               // Augment y with extra rows of
    vnl_vector<T> yy(U_.rows(), 0);             // zeros, so that it matches
    if (yy.size()<y.size()) { // fsm
      cerr << "yy=" << yy << endl;
      cerr << "y =" << y  << endl;
      // the update() call on the next line will abort...
    }
    yy.update(y);                               // cols of u.transpose.
    x = U_.conjugate_transpose() * yy;
  }
  else
    x = U_.conjugate_transpose() * y;

  for (int i = 0; i < x.size(); i++) {        // multiply with diagonal 1/W
    T weight = W_(i, i), zero_ = 0.0;
    if (! (weight == zero_))
      weight = T(1.0) / weight;
    x(i) *= weight;
  }
  return V_ * x;                                // premultiply with v.
}

// -- Solve the matrix-vector system M x = y, assuming that
// the singular values W have been preinverted by the caller.
template <class T>
void vnl_svd<T>::solve_preinverted(const vnl_vector<T>& y, vnl_vector<T>* x_out)  const
{
  vnl_vector<T> x;              // solution matrix
  if (U_.rows() < U_.columns()) {               // augment y with extra rows of
    cout << "vnl_svd<T>::solve_preinverted() -- Augmenting y\n";
    vnl_vector<T> yy(U_.rows(), 0);     // zeros, so that it match
    yy.update(y);                               // cols of u.transpose. ??
    x = U_.conjugate_transpose() * yy;
  } else
    x = U_.conjugate_transpose() * y;
  for (int i = 0; i < x.size(); i++)  // multiply with diagonal W, assumed inverted
    x[i] *= W_(i, i);
  
  *x_out = V_ * x;                                      // premultiply with v.
}

//-----------------------------------------------------------------------------
// -- Return N s.t. M * N = 0
template <class T>
vnl_matrix <T> vnl_svd<T>::nullspace()  const
{
  int k = rank();
  if (k == n_)
    cerr << "vnl_svd<T>::nullspace() -- Matrix is full rank." << last_tol_ << endl;
  return nullspace(n_-k);
}

//-----------------------------------------------------------------------------
// -- Return N s.t. M * N = 0
template <class T>
vnl_matrix <T> vnl_svd<T>::nullspace(int required_nullspace_dimension)  const
{
  return V_.extract(V_.rows(), required_nullspace_dimension, 0, n_ - required_nullspace_dimension);
}

//-----------------------------------------------------------------------------
// -- Return N s.t. M' * N = 0
template <class T>
vnl_matrix <T> vnl_svd<T>::left_nullspace()  const
{
  int k = rank();
  if (k == n_)
    cerr << "vnl_svd<T>::left_nullspace() -- Matrix is full rank." << last_tol_ << endl;
  return U_.extract(U_.rows(), n_-k, 0, k);
}

// -- Implementation to be done yet; currently returns left_nullspace(). - PVR.
template <class T>
vnl_matrix<T> vnl_svd<T>::left_nullspace(int /*required_nullspace_dimension*/) const
{
  return left_nullspace();
}


//-----------------------------------------------------------------------------
// -- Return the rightmost column of V.  Does not check to see whether or
// not the matrix actually was rank-deficient - the caller is assumed to have
// examined W and decided that to their satisfaction.
template <class T>
vnl_vector <T> vnl_svd<T>::nullvector()  const
{
  vnl_vector<T> ret(n_);
  for(int i = 0; i < n_; ++i)
    ret(i) = V_(i, n_-1);
  return ret;
}

//-----------------------------------------------------------------------------
// -- Return the rightmost column of U.  Does not check to see whether or
// not the matrix actually was rank-deficient.
template <class T>
vnl_vector <T> vnl_svd<T>::left_nullvector()  const
{
  vnl_vector<T> ret(m_);
  int col = vnl_math::min(m_, n_) - 1;
  for(int i = 0; i < m_; ++i)
    ret(i) = U_(i, col);
  return ret;
}

//--------------------------------------------------------------------------------

#undef VNL_SVD_INSTANTIATE
#define VNL_SVD_INSTANTIATE(T) \
template class vnl_svd<T >; \
template ostream& operator<<(ostream &, vnl_svd<T > const &);
