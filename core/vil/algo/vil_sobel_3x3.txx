// This is mul/vil2/algo/vil2_sobel_3x3.txx
#ifndef vil2_sobel_3x3_txx_
#define vil2_sobel_3x3_txx_
//:
//  \file
//  \brief Apply sobel gradient filter to an image
//  \author Tim Cootes

#include "vil2_sobel_3x3.h"

//: Apply Sobel 3x3 gradient filter to image.
//  dest has twice as many planes as src, with dest plane (2i) being the i-gradient
//  of source plane i and dest plane (2i+1) being the j-gradient.
template<class srcT, class destT>
void vil2_sobel_3x3(const vil2_image_view<srcT>& src,
                    vil2_image_view<destT>& grad_ij)
{
  int np = src.nplanes();
  int ni = src.ni();
  int nj = src.nj();
  grad_ij.set_size(ni,nj,2*np);
  for (int p=0;p<np;++p)
  {
    vil2_sobel_3x3_1plane(src.top_left_ptr()+p*src.planestep(),
                          src.istep(),src.jstep(),
                          grad_ij.top_left_ptr()+2*p*grad_ij.planestep(),
                          grad_ij.istep(),grad_ij.jstep(),
                          grad_ij.top_left_ptr()+(2*p+1)*grad_ij.planestep(),
                          grad_ij.istep(),grad_ij.jstep(), ni,nj);
  }
}

//: Apply Sobel 3x3 gradient filter to 2D image
template<class srcT, class destT>
void vil2_sobel_3x3(const vil2_image_view<srcT>& src,
                    vil2_image_view<destT>& grad_i,
                    vil2_image_view<destT>& grad_j)
{
  int np = src.nplanes();
  int ni = src.ni();
  int nj = src.nj();
  grad_i.set_size(ni,nj,np);
  grad_j.set_size(ni,nj,np);
  for (int p=0;p<np;++p)
  {
    vil2_sobel_3x3_1plane(src.top_left_ptr()+p*src.planestep(),
                          src.istep(),src.jstep(),
                          grad_i.top_left_ptr()+p*grad_i.planestep(),
                          grad_i.istep(),grad_i.jstep(),
                          grad_j.top_left_ptr()+p*grad_j.planestep(),
                          grad_j.istep(),grad_j.jstep(), ni,nj);
  }
}


#undef VIL2_SOBEL_3X3_INSTANTIATE
#define VIL2_SOBEL_3X3_INSTANTIATE(srcT, destT) \
template void vil2_sobel_3x3(const vil2_image_view< srcT >& src, \
                                   vil2_image_view<destT >& grad_ij); \
template void vil2_sobel_3x3(const vil2_image_view< srcT >& src, \
                                   vil2_image_view<destT >& grad_i, \
                                   vil2_image_view<destT >& grad_j)

#endif // vil2_sobel_3x3_txx_
