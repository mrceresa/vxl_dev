#include <testlib/testlib_register.h>

DECLARE( test_include );
DECLARE( test_amoeba );
DECLARE( test_bignum );
DECLARE( test_complex );
DECLARE( test_complex_eigensystem );
DECLARE( test_convolve );
DECLARE( test_cpoly_roots );
DECLARE( test_determinant );
DECLARE( test_diag_matrix );
DECLARE( test_fft );
DECLARE( test_fft1d );
DECLARE( test_fft2d );
DECLARE( test_file_matrix );
DECLARE( test_finite );
DECLARE( test_functions );
DECLARE( test_generalized_eigensystem );
DECLARE( test_levenberg_marquardt );
DECLARE( test_math );
DECLARE( test_matlab );
DECLARE( test_matrix );
DECLARE( test_matrix_exp );
DECLARE( test_matrix_fixed );
DECLARE( test_minimizers );
DECLARE( test_numeric_limits );
DECLARE( test_numeric_traits );
DECLARE( test_qr );
DECLARE( test_qsvd );
DECLARE( test_rational );
DECLARE( test_real_eigensystem );
DECLARE( test_real_polynomial );
DECLARE( test_resize );
DECLARE( test_rnpoly_roots );
DECLARE( test_rpoly_roots );
DECLARE( test_sample );
DECLARE( test_sparse_matrix );
DECLARE( test_svd );
DECLARE( test_sym_matrix );
DECLARE( test_symmetric_eigensystem );
DECLARE( test_transpose );
DECLARE( test_vector );

void
register_tests()
{
  REGISTER( test_include );
  REGISTER( test_amoeba );
  REGISTER( test_bignum );
  REGISTER( test_complex );
  REGISTER( test_complex_eigensystem );
  REGISTER( test_convolve );
  REGISTER( test_cpoly_roots );
  REGISTER( test_determinant );
  REGISTER( test_diag_matrix );
  REGISTER( test_fft );
  REGISTER( test_fft1d );
  REGISTER( test_fft2d );
  REGISTER( test_file_matrix );
  REGISTER( test_finite );
  REGISTER( test_functions );
  REGISTER( test_generalized_eigensystem );
  REGISTER( test_levenberg_marquardt );
  REGISTER( test_math );
  REGISTER( test_matlab );
  REGISTER( test_matrix );
  REGISTER( test_matrix_exp );
  REGISTER( test_matrix_fixed );
  REGISTER( test_minimizers );
  REGISTER( test_numeric_limits );
  REGISTER( test_numeric_traits );
  REGISTER( test_qr );
  REGISTER( test_qsvd );
  REGISTER( test_rational );
  REGISTER( test_real_eigensystem );
  REGISTER( test_real_polynomial );
  REGISTER( test_resize );
  REGISTER( test_rnpoly_roots );
  REGISTER( test_rpoly_roots );
  REGISTER( test_sample );
  REGISTER( test_sparse_matrix );
  REGISTER( test_svd );
  REGISTER( test_sym_matrix );
  REGISTER( test_symmetric_eigensystem );
  REGISTER( test_transpose );
  REGISTER( test_vector );
}

DEFINE_MAIN;
