#ifndef vcl_cstdio_h_
#define vcl_cstdio_h_
/*
  fsm@robots.ox.ac.uk
*/

#include "vcl_compiler.h"

#if !VCL_CXX_HAS_HEADER_CSTDIO
# include <stdio.h>
# define vcl_generic_cstdio_STD /* */
# include "generic/vcl_cstdio.h"
#elif defined(VCL_SUNPRO_CC_50)
# include <iosfwd> // <cstdio> breaks <iosfwd>
# include "iso/vcl_cstdio.h"
#elif defined(VCL_VC)
# include "win32/vcl_cstdio.h"
#elif defined(VCL_METRO_WERKS)
# include <cstdio>
# define vcl_generic_cstdio_STD /* */
# include "generic/vcl_cstdio.h"
#elif defined(VCL_GCC_295)
# include <cstdio>
   /* gcc 2.95 seems implement these with a macro */
# ifdef getchar
#   undef vcl_getchar
    inline int vcl_getchar() { return getchar(); }
#   define vcl_getchar vcl_getchar
# endif
# ifdef getc
#   undef vcl_getc
    inline int vcl_getc(FILE *stream) { return getc(stream); }
#   define vcl_getc vcl_getc
# endif
# define vcl_generic_cstdio_STD /* */
# include "generic/vcl_cstdio.h"
#else
# include "vcl_cstddef.h" // for size_t
# include "iso/vcl_cstdio.h"
#endif

#endif // vcl_cstdio_h_
