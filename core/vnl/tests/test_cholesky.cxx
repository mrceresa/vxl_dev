// This is vxl/vnl/tests/test_cholesky.cxx
#include <testlib/testlib_test.h>
#include <vcl_iostream.h>
#include <vnl/vnl_matrix.h>
#include <vnl/vnl_matlab_print.h>
#include <vnl/algo/vnl_cholesky.h>
#include <vnl/algo/vnl_svd.h>

#include "test_util.h"

void test_cholesky()
{
  vnl_matrix<double> A(3,3);
  test_util_fill_random(A.begin(), A.end());
  A = A * A.transpose();

  vnl_matrix<double> I(3,3);
  I.set_identity();

  {
    vnl_cholesky chol(A);
    vnl_svd<double> svd(A);
    MATLABPRINT(chol.inverse());
    MATLABPRINT(svd.inverse());
    testlib_test_assert_near("svd.inverse() ~= cholesky.inverse()",
                             (chol.inverse() - svd.inverse()).fro_norm());
  }
  {
    vnl_cholesky chol(A);
    testlib_test_assert_near("Ai * A - I", (chol.inverse() * A - I).fro_norm());
    testlib_test_assert_near("Ai * A - I", (A * chol.inverse() - I).fro_norm());
  }
  {
    vnl_cholesky chol(A, vnl_cholesky::estimate_condition);
    testlib_test_assert_near("Ai * A - I", (chol.inverse() * A - I).fro_norm());
    testlib_test_assert_near("Ai * A - I", (A * chol.inverse() - I).fro_norm());
  }
}

TESTMAIN(test_cholesky);
