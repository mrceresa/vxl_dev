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

//--------------------------------------------------------------
//
// Class : PMatrixComputeLinear
//
// Modifications : see PMatrixComputeLinear.h
//
//--------------------------------------------------------------

#include <vcl/vcl_vector.h>
#include <vnl/vnl_matrix.h>
#include <vnl/algo/vnl_svd.h>

#include <mvl/HomgPoint2D.h>
#include <mvl/HomgPoint3D.h>
#include <mvl/PMatrix.h>
#include <mvl/PMatrixComputeLinear.h>

//-----------------------------------------------------------------------------
//
// -- Compute a projection matrix using linear least squares.
// Input is a list of 3D-2D point correspondences.
// 
// Return false if the calculation fails or there are fewer than six point
// matches in the list.
//
// @{ The algorithm finds the nullvector of the following design matrix, where
// the 3d points $\vec X^i$ are projected into 2d points $\vec u^i$.
// \[
// \left(
// \begin{array}{ccc}
// \vec X^1 u^1_3        & 0_{4}             & -\vec X^1 u^1_1 \cr
// 0_4                   & \vec X^1 u^1_3    & -\vec X^1 u^1_2 \cr
// \vdots                & \vdots & \vdots \cr
// \vec X^n u^n_3        & 0_{4}             & -\vec X^n u^n_1 \cr
// 0_4                   & \vec X^n u^n_3    & -\vec X^n u^n_2 \cr
// \end{array}
// \right)
// \]
// @}

bool
PMatrixComputeLinear::compute (vcl_vector<HomgPoint2D>& points1, vcl_vector<HomgPoint3D>& points2, PMatrix *P)
{
  assert(points1.size() >= 6);
  assert(points2.size() == points1.size());

  unsigned npts = points1.size();
  vnl_matrix<double> a_matrix(npts * 2, 12);

  for (unsigned i = 0; i < npts; i++) {
    const HomgPoint2D& u = points1[i];
    const HomgPoint3D& X = points2[i];
    
    unsigned row_index = i * 2;
    a_matrix(row_index, 0) = X.get_x() * u.get_w();
    a_matrix(row_index, 1) = X.get_y() * u.get_w();
    a_matrix(row_index, 2) = X.get_z() * u.get_w();
    a_matrix(row_index, 3) = X.get_w() * u.get_w();
    a_matrix(row_index, 4) = 0;
    a_matrix(row_index, 5) = 0;
    a_matrix(row_index, 6) = 0;
    a_matrix(row_index, 7) = 0;
    a_matrix(row_index, 8) = -X.get_x() * u.get_x();
    a_matrix(row_index, 9) = -X.get_y() * u.get_x();
    a_matrix(row_index, 10) = -X.get_z() * u.get_x();
    a_matrix(row_index, 11) = -X.get_w() * u.get_x();

    row_index = i * 2 + 1;
    a_matrix(row_index, 0) = 0;
    a_matrix(row_index, 1) = 0;
    a_matrix(row_index, 2) = 0;
    a_matrix(row_index, 3) = 0;
    a_matrix(row_index, 4) = X.get_x() * u.get_w();
    a_matrix(row_index, 5) = X.get_y() * u.get_w();
    a_matrix(row_index, 6) = X.get_z() * u.get_w();
    a_matrix(row_index, 7) = X.get_w() * u.get_w();
    a_matrix(row_index, 8) = -X.get_x() * u.get_y();
    a_matrix(row_index, 9) = -X.get_y() * u.get_y();
    a_matrix(row_index, 10) = -X.get_z() * u.get_y();
    a_matrix(row_index, 11) = -X.get_w() * u.get_y();
  }

  a_matrix.normalize_rows();
  vnl_svd<double> svd(a_matrix);

  P->set(svd.nullvector().data_block());

  return true;
}

bool PMatrixComputeLinear::compute (PMatrix *)
{
  assert(!"PMatrixComputeLinear::compute isn't a good interface\n");
  return false;
}
