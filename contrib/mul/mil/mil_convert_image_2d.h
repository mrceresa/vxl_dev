#ifndef mil_convert_image_2d_h_
#define mil_convert_image_2d_h_

//: \file
//  \brief Functions to convert images from one type to another
//  \author Tim Cootes

#include <mil/mil_image_2d_of.h>
#include <vil/vil_byte.h>

//: Copys src_im (of bytes) into dest_im (of float)
void mil_convert_image_2d(mil_image_2d_of<float>& dest_im,
                       const mil_image_2d_of<vil_byte>& src_im);

//: Copys src_im (of float) into dest_im (of byte)
void mil_convert_image_2d(mil_image_2d_of<vil_byte>& dest_im,
                       const mil_image_2d_of<float>& src_im);

//: Copys src_im (of float) into dest_im (of byte) after linear transform
//  Applies scale and offset to pixels of src_im and puts results in dest_im
//  Useful to compress range of data for display
void mil_convert_image_2d(mil_image_2d_of<vil_byte>& dest_im,
                          const mil_image_2d_of<float>& src_im,
                          double scale, double offset);

#endif // mil_convert_image_2d_h_
