#include <vil/vil_image.h>
#include <vipl/accessors/vipl_accessors_vil_image.h>
#include <vipl/vipl_erode_disk.txx>
#include <vcl_compiler.h> // for definition of VCL_DFL_TMPL_ARG

template class vipl_erode_disk<vil_image, vil_image, unsigned short, unsigned short VCL_DFL_TMPL_ARG(vipl_trivial_pixeliter)>;
