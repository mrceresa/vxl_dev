#include <vil/vil_image.h>
#include <vipl/accessors/vipl_accessors_vil_image.h>
#include <vipl/vipl_threshold.txx>
#include <vcl/vcl_compiler.h> // for definition of VCL_DFL_TMPL_ARG

template class vipl_threshold<vil_image, vil_image, unsigned short, unsigned short VCL_DFL_TMPL_ARG(vipl_trivial_pixeliter)>;
