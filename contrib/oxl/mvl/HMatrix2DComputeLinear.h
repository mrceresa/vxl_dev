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
// Class : HMatrix2DComputeLinear
//
// .SECTION Description:
//
// @{ HMatrix2DComputeLinear contains a linear method to calculate
// the plane projectivity which relates four 2D point correspondences.
// The returned $H$ is such that
// \[
//   H ~ [p_1 ~ p_2 ~ p_3 ~ p_4 ] \sim [p'_1 ~ p'_2 ~ p'_3 ~ p'_4 ]
// \]
// Where the $p_i$ are the homogeneous points in the first view, and the
// $p'_i$ their images.
// @}
//
// .NAME HMatrix2DComputeLinear
// .LIBRARY MViewCompute
// .HEADER MultiView package
// .INCLUDE mvl/HMatrix2DComputeLinear.h
// .FILE HMatrix2DComputeLinear.C
//
// .SECTION Modifications:
//     200598 FSM added checks for degenerate or coincident points.
//

#ifndef _HMatrix2DComputeLinear_h
#define _HMatrix2DComputeLinear_h

#include "HMatrix2DCompute.h"

class HMatrix2DComputeLinear : public HMatrix2DCompute {
//--------------------------------------------------------------------------------=
private:
  bool _allow_ideal_points;
//--------------------------------------------------------------------------------=
protected:
  bool compute_p(const PointArray&, 
		 const PointArray&, 
		 HMatrix2D* H);
//--------------------------------------------------------------------------------=
public:
  HMatrix2DComputeLinear(bool allow_ideal_points = false);
};
#endif
