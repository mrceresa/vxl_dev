// This is core/vil2/vil2_view_as.h
#ifndef vil2_view_as_h_
#define vil2_view_as_h_
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif
//:
// \file
// \brief Various view conversion functions.
//
// vil2_image_view<T>::operator=() can automatically perform these
// conversions for you.
// \author Tim Cootes, Ian Scott - Manchester

#include <vcl_complex.h>
#include <vil2/vil2_image_view.h>
#include <vil2/vil2_rgb.h>
#include <vil2/vil2_rgba.h>

//: Return a 3-plane view of an RGB image, or a 4-plane view of an RGBA, or a 2-plane view of a complex image.
// Class T must be a compound pixel type.
// \return an empty view if it can't do the conversion.
// O(1).
// \relates vil2_image_view
template<class T>
inline vil2_image_view<typename T::value_type> vil2_view_as_planes(const vil2_image_view<T >& v)
{
  if (v.nplanes()!=1) return vil2_image_view<T>();
  const unsigned ncomponents = sizeof(T) / sizeof(T::value_type);

  // An RGB image is RGBRGBRGB, an RGBA image is RGBARGBARGBA, and a
  // complex image is RCRCRC, so istep = ncomponents*v.istep(), and
  // jstep = ncomponents*v.jstep().

#if VCL_VC60 || !VCL_HAS_TYPENAME
  return vil2_image_view<T::value_type>(
    v.memory_chunk(),reinterpret_cast<T::value_type const*>(v.top_left_ptr()),
#else
  return vil2_image_view<typename T::value_type>(
    v.memory_chunk(),reinterpret_cast<typename T::value_type const*>( v.top_left_ptr()),
#endif
    v.ni(),v.nj(),ncomponents,
    v.istep()*ncomponents,v.jstep()*ncomponents,1);
}

//: Return an RGB component view of a 3-plane image.
// \return an empty view if it can't do the conversion (e.g. planestep != 1)
// O(1).
// \relates vil2_image_view
template<class T>
inline vil2_image_view<vil2_rgb<T> > vil2_view_as_rgb(const vil2_image_view<T>& v)
{
  if ((v.nplanes()!=3) || (v.planestep()!=1) || (v.istep()!=3 && v.jstep()!=3))
    return vil2_image_view<vil2_rgb<T> >();

  return vil2_image_view<vil2_rgb<T> >(v.memory_chunk(),
                                      reinterpret_cast<vil2_rgb<T> const*>( v.top_left_ptr()),
                                      v.ni(),v.nj(),1,
                                      v.istep()/3,v.jstep()/3,1);
}

//: Return an RGBA component view of a 4-plane image.
// \return an empty view if it can't do the conversion (e.g. planestep != 1)
// O(1).
// \relates vil2_image_view
template<class T>
inline vil2_image_view<vil2_rgba<T> > vil2_view_as_rgba(const vil2_image_view<T>& v)
{
  if ((v.nplanes()!=4) || (v.planestep()!=1) || (v.istep()!=4 && v.jstep()!=4))
    return vil2_image_view<vil2_rgba<T> >();

  return vil2_image_view<vil2_rgba<T> >(v.memory_chunk(),
                                       static_cast<vil2_rgba<T> const*>( v.top_left_ptr()),
                                       v.ni(),v.nj(),1,
                                       v.istep()/3,v.jstep()/3,1);
}

//: Return an complex component view of a 2N-plane image.
// \return an empty view if it can't do the conversion (e.g. planestep != 1)
// O(1).
// \relates vil2_image_view
template<class T>
inline vil2_image_view<vcl_complex<T> >
vil2_view_as_complex (const vil2_image_view<T> & v)
{
  if ((v.nplanes()%2!=0) || (v.planestep()!=1) || (v.istep()!=2 && v.jstep()!=2))
      return vil2_image_view<vil2_rgb<T> >();

  return vil2_image_view<vcl_complex<T> > (
      v.memory_chunk(),
      reinterpret_cast<vcl_complex<T> const *> (v.top_left_ptr()),
      v.ni(), v.nj(), v.nplanes()/2,
      v.istep()/2, v.jstep()/2, 1);
}

//: Base function to do the work for both vil2_view_real/imag_part
// O(1).
// \relates vil2_image_view
template <class T>
inline vil2_image_view<T>
vil2_view_part (vil2_image_view<vcl_complex<T> > img, int pt)
{
  return vil2_image_view<T> (
      img.memory_chunk(),
      reinterpret_cast<T *>(img.top_left_ptr()) + pt,
      img.ni(), img.nj(), img.nplanes(),
      2*img.istep(), 2*img.jstep(), 2*img.planestep());
}

//: Return a view of the real part of a complex image.
// O(1).
// \relates vil2_image_view
template <class T>
inline vil2_image_view<T>
vil2_view_real_part (vil2_image_view<vcl_complex<T> > img)
{
  return vil2_view_part (img, 0);
}

//: Return a view of the imaginary part of a complex image.
// O(1).
// \relates vil2_image_view
template <class T>
inline vil2_image_view<T>
vil2_view_imag_part (vil2_image_view<vcl_complex<T> > img)
{
  return vil2_view_part (img, 1);
}

#endif // vil2_view_as_h_
