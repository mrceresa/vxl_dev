#include <vil/vil_rgb.h>
#include <vipl/accessors/vipl_accessors_vil_image.txx>
#include <vipl/vipl_convert.txx>

template class vipl_convert<vil_image, vil_image, vil_rgb<unsigned char>, float, vipl_trivial_pixeliter>;
