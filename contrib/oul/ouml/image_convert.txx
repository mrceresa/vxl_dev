#ifndef ouml_image_convert_txx_
#define ouml_image_convert_txx_

#include "image_convert.h"

//----------------------------------------------------------------------
//:
// \file
// \brief convert image

//----------------------------------------------------------------------
//: Converts from one type of memory image to another
//
// \param const vil_memory_image_of<T1> &from: the image converted from
// \param T2 &var: a dummy variable used so the right function is called.
//
// \ret vil_memory_image_of<T2> *: the resultant image - a new one
//
// \author Brendan McCane
//----------------------------------------------------------------------

template <class T1, class T2>
vil_memory_image_of<T2> *convert_image
(
  const vil_memory_image_of<T1> &from,
  T2 &var
)
{
  vil_memory_image_of<T2> *to;
  to = new vil_memory_image_of<T2>(from.width(), from.height());

  for (int x=0; x<from.width(); x++)
    for (int y=0; y<from.height(); y++)
    {
      (*to)(x,y) = (T2)from(x,y);
    }

  return to;
}


//----------------------------------------------------------------------
//: convert image
//
// Specialization for converting from rgb to greyscale
//
// \param const vil_memory_image_of<T1> &from: the image converted from
// \param T2 &var: a dummy variable used so the right function is called.
//
// \ret vil_memory_image_of<T2> *: the resultant image - a new one
//
// \author Brendan McCane
//----------------------------------------------------------------------

template <> vil_memory_image_of<unsigned char> *
convert_image<vil_rgb<unsigned char>, unsigned char>
( 
  const vil_memory_image_of<vil_rgb<unsigned char> > &from,
  unsigned char &var
)
{
  vil_memory_image_of<unsigned char> *to
    = new vil_memory_image_of<unsigned char>(from.width(), from.height());

  for (int x=0; x<from.width(); x++)
    for (int y=0; y<from.height(); y++)
      (*to)(x,y) = from(x,y).grey();

  return to;
}

template <> vil_memory_image_of<double> *
convert_image<vil_rgb<unsigned char>, double>
( 
  const vil_memory_image_of<vil_rgb<unsigned char> > &from,
  double &var
)
{
  vil_memory_image_of<double> *to
    = new vil_memory_image_of<double>(from.width(), from.height());

  for (int x=0; x<from.width(); x++)
    for (int y=0; y<from.height(); y++)
      (*to)(x,y) = (double)from(x,y).grey();

  return to;
}

#endif // ouml_image_convert_txx_
