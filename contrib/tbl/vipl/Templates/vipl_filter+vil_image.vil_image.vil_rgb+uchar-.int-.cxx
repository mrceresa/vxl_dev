#include <vil/vil_image.h>
#include <vil/vil_rgb.h>
typedef vil_rgb<unsigned char> rgbcell;

// this must be here for filter-2d to work
#include <vipl/section/vipl_filterable_section_container_generator_vil_image.txx>

#include <vipl/filter/vipl_filter.txx>
template class vipl_filter<vil_image, vil_image, rgbcell, int, 2, vipl_trivial_pixeliter>;

#include <vipl/filter/vipl_filter_2d.txx>
template class vipl_filter_2d<vil_image, vil_image, rgbcell, int, vipl_trivial_pixeliter>;

