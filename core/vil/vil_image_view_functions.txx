// This is mul/vil2/vil2_image_view_functions.txx
#ifndef vil2_image_view_functions_txx_
#define vil2_image_view_functions_txx_
//:
// \file
// \brief Various functions for manipulating image views
// \author Tim Cootes - Manchester

#include <vil2/vil2_image_view_functions.h>
#include <vcl_cassert.h>
#include <vil2/vil2_byte.h>
#include <vil/vil_rgb.h>



//: Create a copy of the data viewed by this, and return a view of copy.
template<class T>
vil2_image_view<T> vil2_deep_copy(const vil2_image_view<T> &rhs)
{
  vil2_image_view<T> cpy;
  cpy.deep_copy(rhs);
  return cpy;
}


//: Copy src to dest, without changing dest's view parameters.
// This is useful if you want to copy on image into a window on another image.
// src and dest must have identical sizes, and types.
template<class T>
void vil2_reformat_copy(const vil2_image_view<T> &src, vil2_image_view<T> &dest)
{
  assert (src.nplanes() == dest.nplanes() &&
    src.nj() == dest.nj() &&
    src.ni() == dest.ni());
  for (unsigned p = 0; p < dest.nplanes(); ++p)
    for (unsigned j = 0; j < dest.nj(); ++j)
      for (unsigned i = 0; i < dest.ni(); ++i)
        dest(i,j,p) = src(i,j,p);
}

//: True if the actual images are identical.
// $\bigwedge_{i,j,p} {\textstyle src}(i,j,p) == {\textstyle dest}(i,j,p)$
// The data may be formatted differently in each memory chunk.
template<class T>
bool vil2_deep_equality(const vil2_image_view<T> &lhs, const vil2_image_view<T> &rhs)
{
  if (lhs.nplanes() != rhs.nplanes() ||
    lhs.nj() != rhs.nj() ||
    lhs.ni() != rhs.ni()) return false;

  for (unsigned p = 0; p < rhs.nplanes(); ++p)
    for (unsigned j = 0; j < rhs.nj(); ++j)
      for (unsigned i = 0; i < rhs.ni(); ++i)
        if (!(rhs(i,j,p) == lhs(i,j,p))) return false;
  return true;
}


//: Copy src to window in dest.
// Size of window is defined by src.
//  O(window.size).
template<class T>
void vil2_copy_to_window(const vil2_image_view<T> &src, vil2_image_view<T> &dest,
                         unsigned i0, unsigned j0)
{
  // check window is within dest's bounds
  assert(i0+src.ni() <= dest.ni() && j0+src.nj() <= dest.nj());
  assert (src.nplanes() == dest.nplanes());

  for (unsigned p = 0; p < dest.nplanes(); ++p)
    for (unsigned j = 0; j < src.nj(); ++j)
      for (unsigned i = 0; i < src.ni(); ++i)
        dest(i+i0,j+j0,p) = src(i,j,p);
}

//: Return an ni x nj window of this data with offset (i0,j0)
template<class T>
vil2_image_view<T> vil2_window(const vil2_image_view<T> &im,
                               unsigned i0, unsigned ni, unsigned j0, unsigned nj)
{
  assert(i0<im.ni()); assert(i0+ni<=im.ni());
  assert(j0<im.nj()); assert(j0+nj<=im.nj());
  return vil2_image_view<T>(im.memory_chunk(),im.top_left_ptr()+ i0*im.istep() + j0*im.jstep(),
    ni,nj,im.nplanes(),im.istep(),im.jstep(),im.planestep());
}

//: Return a view of plane p
template<class T>
vil2_image_view<T> vil2_plane(const vil2_image_view<T> &im, unsigned p)
{
  assert(p<im.nplanes());
  return vil2_image_view<T>(im.memory_chunk(),im.top_left_ptr()+p*im.planestep(),im.ni(),im.nj(),1,
    im.istep(),im.jstep(),im.planestep());
}



//: Return a 3-plane view of an RGB image
// \return an empty view if it can't do the conversion
// (because it is already a multiplane image.)
template<class T>
vil2_image_view<T> vil2_view_as_planes(const vil2_image_view<vil_rgb<T> >& v)
{
  if (v.nplanes()!=1) return vil2_image_view<T>();

  // Image is RGBRGBRGB so i step = 3*v.istep(), jstep=3*v.jstep()
  return vil2_image_view<T>(v.memory_chunk(),(T const*) v.top_left_ptr(),
                            v.ni(),v.nj(),3,
                            v.istep()*3,v.jstep()*3,1);
}

//: Return an RGB component view of a 3-plane image
// \return an empty view if it can't do the conversion
// (because the data isn't arranged with triples of consecutive components)
template<class T>
vil2_image_view<vil_rgb<T> > vil2_view_as_rgb(const vil2_image_view<T>& v)
{
  if ((v.nplanes()!=3) || (v.planestep()!=1) || (v.istep()!=3 && v.jstep()!=3))
    return vil2_image_view<vil_rgb<T> >();

  return vil2_image_view<vil_rgb<T> >(v.memory_chunk(),
                                      (vil_rgb<T> const*) v.top_left_ptr(),
                                      v.ni(),v.nj(),1,
                                      v.istep()/3,v.jstep()/3,1);
}

//: Create a view which appears as the transpose of this view.
//  i.e transpose(i,j,p) = view(j,i,p)
template<class T>
vil2_image_view<T> vil2_transpose(const vil2_image_view<T>& v)
{
  // Create view with i and j switched
  return vil2_image_view<T>(v.memory_chunk(),v.top_left_ptr(),
                                   v.nj(),v.ni(),v.nplanes(),
                                   v.jstep(),v.istep(),v.planestep());
}

//: Create a reflected view in which i -> (ni-1)-i
//  i.e vil2_flip_lr(view)(i,j,p) = view(ni-1-i,j,p)
template<class T>
vil2_image_view<T> vil2_flip_lr(const vil2_image_view<T>& v)
{
  return vil2_image_view<T>(v.memory_chunk(),
                            v.top_left_ptr()+(v.ni()-1)*v.istep(),
                            v.ni(),v.nj(),v.nplanes(),
                            -v.istep(),v.jstep(),v.planestep());
}

//: Create a reflected view in which j -> (nj-1)-j
//  i.e vil2_flip_ud(view)(i,j,p) = view(i,nj-1-j,p)
template<class T>
vil2_image_view<T> vil2_flip_ud(const vil2_image_view<T>& v)
{
  return vil2_image_view<T>(v.memory_chunk(),
                            v.top_left_ptr()+(v.nj()-1)*v.jstep(),
                            v.ni(),v.nj(),v.nplanes(),
                            v.istep(),-v.jstep(),v.planestep());
}

//: Compute minimum and maximum values over view
template<class T>
void vil2_value_range(T& min_value, T& max_value,const vil2_image_view<T>& view)
{
  if (view.size()==0)
  {
    min_value = 0;
    max_value = 0;
    return;
  }

  min_value = *(view.top_left_ptr());
  max_value = min_value;

  int ni = view.ni(), istep=view.istep();
  int nj = view.nj(), jstep=view.jstep();
  int np = view.nplanes(), pstep = view.planestep();

  const T* plane = view.top_left_ptr();
  for (int p=0;p<np;++p,plane += pstep)
  {
    const T* row = plane;
    for (int j=0;j<nj;++j,row += jstep)
    {
      const T* pixel = row;
      for (int i=0;i<ni;++i,pixel+=istep)
      {
        if (*pixel<min_value) min_value=*pixel;
        else if (*pixel>max_value) max_value=*pixel;
      }
    }
  }
}

//: Fill view with given value
template<class T>
void vil2_fill(vil2_image_view<T>& view, T value)
{
  int ni = view.ni(), istep=view.istep();
  int nj = view.nj(), jstep=view.jstep();
  int np = view.nplanes(), pstep = view.planestep();

  T* plane = view.top_left_ptr();
  for (int p=0;p<np;++p,plane += pstep)
  {
    T* row = plane;
    for (int j=0;j<nj;++j,row += jstep)
    {
      T* pixel = row;
      for (int i=0;i<ni;++i,pixel+=istep) *pixel=value;
    }
  }
}

//: Fill data[i*step] (i=0..n-1) with given value
template<class T>
void vil2_fill_line(T* data, unsigned n, int step, T value)
{
  T* end_data = data + n*step;
  while (data!=end_data) { *data=value; data+=step; }
}

//: Fill row j in view with given value
template<class T>
void vil2_fill_row(vil2_image_view<T>& view, unsigned j, T value)
{
  int ni = view.ni(), istep=view.istep();
  int nj = view.nj(), jstep=view.jstep();
  int np = view.nplanes(), pstep = view.planestep();

  assert(j<nj);

  T* row = view.top_left_ptr() + j*jstep;
  for (int p=0;p<np;++p,row += pstep)
    vil2_fill_line(row,ni,istep,value);
}

//: Fill column i in view with given value
template<class T>
void vil2_fill_col(vil2_image_view<T>& view, unsigned i, T value)
{
  int ni = view.ni(), istep=view.istep();
  int nj = view.nj(), jstep=view.jstep();
  int np = view.nplanes(), pstep = view.planestep();

  assert(i<ni);
  T* col_top = view.top_left_ptr() + i*istep;
  for (int p=0;p<np;++p,col_top += pstep)
    vil2_fill_line(col_top,nj,jstep,value);
}


//: print all image data to os in a grid (rounds output to int)
template<class T>
void vil2_print_all(vcl_ostream& os,const vil2_image_view<T>& view)
{
  os<<view.is_a()<<" "<<view.nplanes()<<" planes, each "<<view.ni()<<" x "<<view.nj();
  os<<" istep: "<<view.istep()<<" ";
  os<<" jstep: "<<view.jstep()<<" ";
  os<<" planestep: "<<view.planestep()<<vcl_endl;
  for (int p=0;p<view.nplanes();++p)
  {
    if (view.nplanes()>1) os<<"Plane "<<p<<":"<<vcl_endl;
    for (int j=0;j<view.nj();++j)
    {
      for (int i=0;i<view.ni();++i)
      {
        vil2_print_value(os,view(i,j,p));
        os<<" ";
      }
      os<<vcl_endl;
    }
  }
}

// For things which must not be composites
#define VIL2_IMAGE_VIEW_FUNCTIONS_INSTANTIATE_FOR_SCALARS(T) \
template vil2_image_view<T > vil2_view_as_planes(const vil2_image_view<vil_rgb<T > >&); \
template vil2_image_view<vil_rgb<T > > vil2_view_as_rgb(const vil2_image_view<T >& plane_view); \
template void vil2_value_range(T& min_value, T& max_value,const vil2_image_view<T >& view)

// For everything else
#define VIL2_IMAGE_VIEW_FUNCTIONS_INSTANTIATE(T) \
template void vil2_copy_to_window(const vil2_image_view<T > &src, vil2_image_view<T > &dest, \
  unsigned i0, unsigned j0); \
template bool vil2_deep_equality(const vil2_image_view<T > &lhs, const vil2_image_view<T > &rhs); \
template void vil2_reformat_copy(const vil2_image_view<T > &src, vil2_image_view<T > &dest); \
template vil2_image_view<T > vil2_deep_copy(const vil2_image_view<T > &rhs); \
template vil2_image_view<T > vil2_window(const vil2_image_view<T > &im, \
  unsigned i0, unsigned ni, unsigned j0, unsigned nj); \
template vil2_image_view<T > vil2_plane(const vil2_image_view<T > &im, unsigned p); \
template vil2_image_view<T > vil2_transpose(const vil2_image_view<T >& v); \
template vil2_image_view<T > vil2_flip_lr(const vil2_image_view<T >& view); \
template vil2_image_view<T > vil2_flip_ud(const vil2_image_view<T >& view); \
template void vil2_print_all(vcl_ostream& os,const vil2_image_view<T >& view); \
template void vil2_fill(vil2_image_view<T >& view, T value); \
template void vil2_fill_line(T * data, unsigned n, int step, T value); \
template void vil2_fill_row(vil2_image_view<T >& view, unsigned j, T value); \
template void vil2_fill_col(vil2_image_view<T >& view, unsigned i, T value)

#endif
