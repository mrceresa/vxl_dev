// This is mul/vil2/vil2_image_view_functions.txx
#ifndef vil2_image_view_functions_txx_
#define vil2_image_view_functions_txx_
//:
// \file
// \brief Various functions for manipulating image views
// \author Tim Cootes - Manchester

#include <vil2/vil2_image_view_functions.h>

//: Return a 3-plane view of an RGB image
// \return an empty view if it can't do the conversion 
// (because it is already a multiplane image.)
template<class T>
vil2_image_view<T> vil2_view_as_planes(const vil2_image_view<vil_rgb<T> >& v)
{
  if (v.nplanes()!=1) return vil2_image_view<T>();

  // Image is RGBRGBRGB so x step = 3*v.xstep(), ystep=3*v.ystep()
  return vil2_image_view<T>(v.memory_chunk(),(T*) v.top_left_ptr(),
                            v.nx(),v.ny(),3,
                            v.xstep()*3,v.ystep()*3,1);
}

//: Return an RGB component view of a 3-plane image
// \return an empty view if it can't do the conversion 
// (because the data isn't arranged with triples of consecutive components)
template<class T>
vil2_image_view<vil_rgb<T> > vil2_view_as_rgb(const vil2_image_view<T>& v)
{
  if ((v.nplanes()!=3) || (v.planestep()!=1) || (v.xstep()!=3 && v.ystep()!=3))
    return vil2_image_view<vil_rgb<T> >();

  return vil2_image_view<vil_rgb<vil_byte> >(v.memory_chunk(),
                                             (vil_rgb<vil_byte>*) v.top_left_ptr(),
                                             v.nx(),v.ny(),1,
                                             v.xstep()/3,v.ystep()/3,1);
}

//: Create a view which appears as the transpose of this view.
//  i.e transpose(x,y,p) = view(y,x,p)
template<class T>
vil2_image_view<T> vil2_transpose(const vil2_image_view<T>& v)
{
  // Create view with x and y switched
  return vil2_image_view<T>(v.memory_chunk(),v.top_left_ptr(),
                                   v.ny(),v.nx(),v.nplanes(),
                                   v.ystep(),v.xstep(),v.planestep());
}

//: Create a reflected view in which x -> (nx-1)-x
//  i.e vil2_reflect_x(view)(x,y,p) = view(nx-1-x,y,p)
template<class T>
vil2_image_view<T> vil2_reflect_x(const vil2_image_view<T>& v)
{
  return vil2_image_view<T>(v.memory_chunk(),
                            v.top_left_ptr()+(v.nx()-1)*v.xstep(),
                            v.nx(),v.ny(),v.nplanes(),
                            -v.xstep(),v.ystep(),v.planestep());
}

//: Create a reflected view in which y -> (ny-1)-y
//  i.e vil2_reflect_y(view)(x,y,p) = view(x,ny-1-y,p)
template<class T>
vil2_image_view<T> vil2_reflect_y(const vil2_image_view<T>& v)
{
  return vil2_image_view<T>(v.memory_chunk(),
                            v.top_left_ptr()+(v.ny()-1)*v.ystep(),
                            v.nx(),v.ny(),v.nplanes(),
                            v.xstep(),-v.ystep(),v.planestep());
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

  int nx = view.nx(), xstep=view.xstep();
  int ny = view.ny(), ystep=view.ystep();
  int np = view.nplanes(), pstep = view.planestep();

  const T* plane = view.top_left_ptr();
  for (int p=0;p<np;++p,plane += pstep)
  {
    const T* row = plane;
    for (int y=0;y<ny;++y,row += ystep)
    {
      const T* pixel = row;
      for (int x=0;x<nx;++x,pixel+=xstep)
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
  int nx = view.nx(), xstep=view.xstep();
  int ny = view.ny(), ystep=view.ystep();
  int np = view.nplanes(), pstep = view.planestep();

  T* plane = view.top_left_ptr();
  for (int p=0;p<np;++p,plane += pstep)
  {
    T* row = plane;
    for (int y=0;y<ny;++y,row += ystep)
    {
      T* pixel = row;
      for (int x=0;x<nx;++x,pixel+=xstep) *pixel=value;
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

//: Fill row y in view with given value
template<class T>
void vil2_fill_row(vil2_image_view<T>& view, unsigned y, T value)
{
  int nx = view.nx(), xstep=view.xstep();
  int ny = view.ny(), ystep=view.ystep();
  int np = view.nplanes(), pstep = view.planestep();

  assert(y<ny);

  T* row = view.top_left_ptr() + y*ystep;
  for (int p=0;p<np;++p,row += pstep)
    vil2_fill_line(row,nx,xstep,value);
}

//: Fill column x in view with given value
template<class T>
void vil2_fill_col(vil2_image_view<T>& view, unsigned x, T value)
{
  int nx = view.nx(), xstep=view.xstep();
  int ny = view.ny(), ystep=view.ystep();
  int np = view.nplanes(), pstep = view.planestep();

  assert(x<nx);
  T* col_top = view.top_left_ptr() + x*xstep;
  for (int p=0;p<np;++p,col_top += pstep)
    vil2_fill_line(col_top,ny,ystep,value);
}


//: How to print value in vil2_print_all(image_view)
template<class T>
void vil2_print_value(vcl_ostream& os, const T& value)
{
  os.width(3);
  os<<value;
}


//: print all image data to os in a grid (rounds output to int)
template<class T>
void vil2_print_all(vcl_ostream& os,const vil2_image_view<T>& view)
{
  os<<view.is_a()<<" "<<view.nplanes()<<" planes, each "<<view.nx()<<" x "<<view.ny();
  os<<" xstep: "<<view.xstep()<<" ";
  os<<" ystep: "<<view.ystep()<<" ";
  os<<" planestep: "<<view.planestep()<<vcl_endl;
  for (int i=0;i<view.nplanes();++i)
  {
    if (view.nplanes()>1) os<<"Plane "<<i<<":"<<vcl_endl;
    for (int y=0;y<view.ny();++y)
    {
      for (int x=0;x<view.nx();++x)
      {
        vil2_print_value(os,view(x,y,i));
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
template void vil2_value_range(T& min_value, T& max_value,const vil2_image_view<T>& view); \
template void vil2_print_value(vcl_ostream& os, const T& value)

// For everything else
#define VIL2_IMAGE_VIEW_FUNCTIONS_INSTANTIATE(T) \
template vil2_image_view<T > vil2_transpose(const vil2_image_view<T >& v); \
template vil2_image_view<T > vil2_reflect_x(const vil2_image_view<T >& view); \
template vil2_image_view<T > vil2_reflect_y(const vil2_image_view<T >& view); \
template void vil2_print_all(vcl_ostream& os,const vil2_image_view<T >& view); \
template void vil2_fill(vil2_image_view<T >& view, T value); \
template void vil2_fill_line(T * data, unsigned n, int step, T value); \
template void vil2_fill_row(vil2_image_view<T >& view, unsigned y, T value); \
template void vil2_fill_col(vil2_image_view<T >& view, unsigned x, T value)

#endif
