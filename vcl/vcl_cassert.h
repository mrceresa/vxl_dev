// The <cassert> header does not
// have inclusion guards. The
// C and C++ standards say so.

// .NAME vcl_cassert
// .INCLUDE vcl_cassert.h
// .FILE vcl_cassert.cxx
// @author fsm@robots.ox.ac.uk

#include "vcl_compiler.h"

// This header file should do something like this:
//   #undef assert
//   #ifdef NDEBUG
//   # define assert(x) ((void) 0)
//   #else
//   extern void vcl_cassert_failure(char const *, int, char const *);
//   # define assert(x) do { if (!(x)) vcl_cassert_failure(__FILE__, __LINE__, #x); } while (false)
//   #endif
// If the system/compiler version works, use that instead.

#if defined(VCL_SGI_CC_720)
# include <assert.h>
#else
# include "iso/vcl_cassert.h"
#endif

// fsm: There should not be a vcl_assert macro as there is no
// std::assert symbol. If your assert macro is broken, fix it
// here using #undef and #define.
//#define vcl_assert(x) assert(x)
