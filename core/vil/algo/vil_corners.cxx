//:
// \file
// \brief Estimate corner positions using Forstner/Harris approach
// \author Tim Cootes

#include "vil_corners.h"
#include <vil/vil_fill.h>
#include <vcl_cassert.h>

//: Compute Forstner/Harris corner strength function given gradient images.
//  grad_i and grad_j are assumed to be the i and j gradient images (single
//  plane), such as produced by vil_sobel_3x3().  At each pixel compute
//  the Forstner/Harris corner function: det(H)-k*sqr(trace(H)), where
//  H is the 2x2 matrix of second derivatives, generated by applying a Sobel
//  operator to the gradient images.
//
//  The local peaks of the output image correspond to corner candidates.
// \relates vil_image_view
void vil_corners(const vil_image_view<float>& grad_i,
                 const vil_image_view<float>& grad_j,
                 vil_image_view<float>& dest, double k)
{
  assert(grad_i.nplanes()==1);
  assert(grad_j.nplanes()==1);
  unsigned ni = grad_i.ni(), nj = grad_i.nj();
  assert(grad_j.ni()==ni && grad_j.nj()==nj );

  dest.set_size(ni,nj);

  // Zero a two pixel border
  for (unsigned i=0;i<2;++i)
  {
    vil_fill_row(dest,i,0.0f);
    vil_fill_row(dest,nj-1-i,0.0f);
    vil_fill_col(dest,i,0.0f);
    vil_fill_col(dest,ni-1-i,0.0f);
  }

  const unsigned ni2 = ni-2;
  const unsigned nj2 = nj-2;

  // Compute relative grid positions
  //  o1 o2 o3
  //  o4    o5
  //  o6 o7 o8
  const vcl_ptrdiff_t oi1 = grad_i.jstep() - grad_i.istep();
  const vcl_ptrdiff_t oi2 = grad_i.jstep();
  const vcl_ptrdiff_t oi3 = grad_i.istep() + grad_i.jstep();
  const vcl_ptrdiff_t oi4 = -grad_i.istep();
  const vcl_ptrdiff_t oi5 = grad_i.istep();
  const vcl_ptrdiff_t oi6 = -grad_i.istep() - grad_i.jstep();
  const vcl_ptrdiff_t oi7 = -grad_i.jstep();
  const vcl_ptrdiff_t oi8 = grad_i.istep() - grad_i.jstep();

  const vcl_ptrdiff_t oj1 = grad_j.jstep() - grad_j.istep();
  const vcl_ptrdiff_t oj2 = grad_j.jstep();
  const vcl_ptrdiff_t oj3 = grad_j.istep() + grad_j.jstep();
  const vcl_ptrdiff_t oj4 = -grad_j.istep();
  const vcl_ptrdiff_t oj5 = grad_j.istep();
  const vcl_ptrdiff_t oj6 = -grad_j.istep() - grad_j.jstep();
  const vcl_ptrdiff_t oj7 = -grad_j.jstep();
  const vcl_ptrdiff_t oj8 = grad_j.istep() - grad_j.jstep();

  float * d_data = &dest(2,2);
  const float * gi_data = &grad_i(2,2);
  const float * gj_data = &grad_j(2,2);

  for (unsigned j=2;j<nj2;++j)
  {
    float* d = d_data;
    const float* pgi = gi_data;
    const float* pgj = gj_data;
    for (unsigned i=2;i<ni2;++i)
    {
    // Compute gradient in i
      float dxdx = 0.125f*(pgi[oi3]+pgi[oi8] - (pgi[oi1]+pgi[oi6])) + 0.25f*(pgi[oi5]-pgi[oi4]);
     // Compute gradient in j
      float dxdy = 0.125f*(pgi[oi1]+pgi[oi3] - (pgi[oi6]+pgi[oi8])) + 0.25f*(pgi[oi2]-pgi[oi7]);
     // Compute gradient in j
      float dydy = 0.125f*(pgj[oj1]+pgj[oj3] - (pgi[oj6]+pgj[oj8])) + 0.25f*(pgj[oj2]-pgj[oj7]);

      float detH = dxdx*dydy + dxdy*dxdy;
      float traceH = dxdx+dydy;

      *d = detH - float(k) * traceH * traceH;

      pgi += grad_i.istep();
      pgj += grad_j.istep();
      d += dest.istep();
    }

    gi_data += grad_i.jstep();
    gj_data += grad_j.jstep();
    d_data  += dest.jstep();
  }
}
