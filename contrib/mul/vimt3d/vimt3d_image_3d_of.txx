#ifndef vimt3d_image_3d_of_txx_
#define vimt3d_image_3d_of_txx_
//:
//  \file
//  \brief Container for vil_image_view<T> + transform
//  \author Tim Cootes

#include <vcl_string.h>
#include <vsl/vsl_indent.h>
#include <vimt3d/vimt3d_image_3d_of.h>
#include <vil3d/vil3d_print.h>
#include <vsl/vsl_vector_io.h>
#include <vil3d/io/vil3d_io_image_view.h>

//=======================================================================

template<class T>
vimt3d_image_3d_of<T>::vimt3d_image_3d_of()
{
}

template<class T>
vimt3d_image_3d_of<T>::vimt3d_image_3d_of(unsigned ni, unsigned nj, unsigned nk, unsigned nplanes)
: image_(ni,nj,nk,nplanes)
{
}

//: Perform deep copy of src into this image
template<class T>
void vimt3d_image_3d_of<T>::deep_copy(const vimt3d_image_3d_of& src)
{
  world2im_     = src.world2im_;
  image_.deep_copy(src.image_);
}

template<class T> vimt3d_image_3d_of<T>::~vimt3d_image_3d_of()
{
}


//: Define valid data region (including transform).
//  Resizes and sets the transformation so that
//  worldToIm(x,y) is valid for all points in range
template<class T>
void vimt3d_image_3d_of<T>::set_valid_region(int i0, unsigned ni,
                                             int j0, unsigned nj,
                                             int k0, unsigned nk)
{
  image_.set_size(ni,nj,nk);
  world2im_.set_translation(-i0,-j0,-k0);
}

template<class T>
bool vimt3d_image_3d_of<T>::is_class(vcl_string const& s) const
{
  return s==vimt3d_image_3d_of<T>::is_a() || vimt3d_image_3d::is_class(s);
}

//=======================================================================

template<class T>
short vimt3d_image_3d_of<T>::version_no() const
{
  return 1;
}

//=======================================================================
template<class T>
vimt_image* vimt3d_image_3d_of<T>::clone() const
{
  return new vimt3d_image_3d_of(*this);
}

//=======================================================================

template<class T>
void vimt3d_image_3d_of<T>::print_summary(vcl_ostream& os) const
{
  os<<vsl_indent() << "Transform: "<<world2im_
    <<vsl_indent() << image_<<vcl_endl;
}

//=======================================================================
//: print all data to os
template<class T>
void vimt3d_image_3d_of<T>::print_all(vcl_ostream& os) const
{
  os<<vsl_indent();
  vil3d_print_all(os,image_);
  os<<'\n'<<vsl_indent() << "Transform: "<<world2im_<<'\n';
}


//=======================================================================

template<class T>
void vimt3d_image_3d_of<T>::b_write(vsl_b_ostream& bfs) const
{
  vsl_b_write(bfs,version_no());
  vsl_b_write(bfs,image_);
  vsl_b_write(bfs,world2im_);
}

//=======================================================================

template<class T>
void vimt3d_image_3d_of<T>::b_read(vsl_b_istream& bfs)
{
  if (!bfs) return;

  short version;
  vsl_b_read(bfs,version);
  switch (version)
  {
  case (1):
    vsl_b_read(bfs,image_);
    vsl_b_read(bfs,world2im_);
    break;
  default:
    vcl_cerr << "I/O ERROR: vimt3d_image_3d_of<T>::b_read(vsl_b_istream&) \n"
             << "           Unknown version number "<< version << "\n";
    bfs.is().clear(vcl_ios::badbit); // Set an unrecoverable IO error on stream
    return;
  }
}

//: True if transforms, etc. are equal, and they share same image data.
//  This does not do a deep equality on image data. If the images point
//  to different image data objects that contain identical images, then
//  the result will still be false.
template<class T>
bool vimt3d_image_3d_of<T>::operator==(const vimt3d_image_3d_of<T> &other) const
{
  return image_ == other.image_ &&
      world2im_ == other.world2im_;
}

#define VIMT3D_IMAGE_3D_OF_INSTANTIATE(T) \
VCL_DEFINE_SPECIALIZATION vcl_string vimt3d_image_3d_of<T >::is_a() const \
{ return vcl_string("vimt3d_image_3d_of<" #T ">"); }\
template class vimt3d_image_3d_of<T >

#endif // vimt3d_image_3d_of_txx_
