//-*- c++ -*-------------------------------------------------------------------
#ifndef vcl_compiler_h_
#define vcl_compiler_h_

// It's much better to determine the compiler automatically here than to depend
// on command-line flags being set.

#if defined(__sgi) && !defined(__GNUC__)
# ifndef _COMPILER_VERSION
#  define VCL_SGI_CC_6
# else
#  if (_COMPILER_VERSION >= 700)
#   define VCL_SGI_CC_7
#  else
#   define VCL_SGI_CC_6
#  endif
#  if   (_COMPILER_VERSION >= 730)
#   define VCL_SGI_CC_730
#  elif (_COMPILER_VERSION >= 720)
#   define VCL_SGI_CC_720
#  endif
#  define VCL_SGI_CC
# endif
#endif

#if defined(__SUNPRO_CC)
# define VCL_SUNPRO_CC
# if (__SUNPRO_CC>=0x500)
#  define VCL_SUNPRO_CC_50
# else
#  undef VCL_SUNPRO_CC_50
# endif
# ifdef INSTANTIATE_TEMPLATES
#  define _RWSTD_COMPILE_INSTANTIATE
# endif
#endif

#if defined(__GNUC__)
# define VCL_GCC
# if (__GNUC__<=1)
#  error "forget it."
# elif (__GNUC__==2)
#  if (__GNUC_MINOR__>=100)
#   error "I need some help here."
#  elif (__GNUC_MINOR__>=95)
#   define VCL_GCC_295
#  elif (__GNUC_MINOR__>8)
#   define VCL_EGCS
#  elif (__GNUC_MINOR__>7)
#   define VCL_GCC_28
#  elif (__GNUC_MINOR__>6)
#   define VCL_GCC_27
#  endif
#  if (__GNUC_MINOR__>7)
#   define VCL_GCC_EGCS // so this is the union of EGCS, GCC_28 and GCC_295
#  endif
# else
#  define VCL_GCC_30
# endif
#endif

#if defined(_WIN32) || defined(WIN32)
# define VCL_WIN32
# if defined(_MSC_VER)
#  define VCL_VC
#  if _MSC_VER >= 1200
#   define VCL_VC60 1
#  else
#   define VCL_VC50 1
#  endif
# endif
#endif

// win32 or vc++ ?
// awf hack alert:
#ifdef VCL_WIN32
#pragma warning(disable:4786 4660 4661)
#pragma warning(disable:4786 4660 4355 4390)
#endif

#if defined(__KCC)
# define VCL_KAI
#endif

// include header files generated by configure.
#include <vcl/vcl_config_manual.h>
#include <vcl/vcl_config_compiler.h>
#include <vcl/vcl_config_headers.h>

// This *needs* to come after vcl_config_headers.h
#ifdef __GNUC__
// One difference between v2 and v3 is that the former has
// no <istream> header file whereas v3 has the lot.
# if !defined(GNU_LIBSTDCXX_V3) && defined(VCL_GCC_295) && VCL_CXX_HAS_HEADER_ISTREAM
#  define GNU_LIBSTDCXX_V3 1
# endif
# if defined (GNU_LIBSTDCXX_V3)
#  define VCL_GCC_WITH_GNU_LIBSTDCXX_V3
# else
#  define VCL_GCC_WITH_GNU_LIBSTDCXX_V2
# endif
#endif

// -------------------- default template parameters
#if VCL_CAN_DO_COMPLETE_DEFAULT_TYPE_PARAMETER
# define VCL_DFL_TYPE_PARAM_STLDECL(A, a) class A = a
#else
# define VCL_DFL_TYPE_PARAM_STLDECL(A, a) class A
#endif

#if VCL_CAN_DO_TEMPLATE_DEFAULT_TYPE_PARAMETER
# define VCL_DFL_TMPL_PARAM_STLDECL(A, a) class A = a
#else
# define VCL_DFL_TMPL_PARAM_STLDECL(A, a) class A
#endif

#define VCL_DFL_TMPL_ARG(classname) , classname

#if VCL_USE_NATIVE_STL
# define VCL_SUNPRO_ALLOCATOR_HACK(T) T VCL_SUNPRO_CLASS_SCOPE_HACK(std::allocator<T >)
#else
# define VCL_SUNPRO_ALLOCATOR_HACK(T) T // FIXIT
#endif

// -------------------- template instantiation

#if VCL_NEEDS_INLINE_INSTANTIATION
# define VCL_INSTANTIATE_INLINE(symbol) template symbol;
#else
# define VCL_INSTANTIATE_INLINE(symbol) /* */
#endif

// work-around to get template instantiation to work correctly with SunPro
// check flag to turn on inlining
#undef IUEi_STL_INLINE
#if defined(INLINE_EXPLICIT_FLAG) && defined(VCL_SUNPRO_CC) && defined(INSTANTIATE_TEMPLATES)
# define IUEi_STL_INLINE
#else
# define IUEi_STL_INLINE inline
#endif

//--------------------------------------------------------------------------------

// Decide at configuration time whether you want to use double instead
// of long double. On most machines it's too slow, but the default is
// to use it because turning it off by default would be weird.
#if VCL_USE_LONG_DOUBLE
typedef long double VCL_long_double;
#else
typedef double VCL_long_double;
#endif

#if VCL_FOR_SCOPE_HACK
# undef for
# define for if (false) { } else for
typedef int saw_VCL_FOR_SCOPE_HACK;
#endif

// fix to instantiate template functions
#define VCL_INSTANTIATE_NONINLINE(fn_decl) template fn_decl ;

// -------------------- handy macros

// -- VCL_COMMA
//
// Handy for passing things with commas in them to CPP macros.  e.g. 
// DO_MACRO(pair<A,B>) can be replaced by DO_MACRO(pair<A VCL_COMMA B>).
#define VCL_COMMA ,


// -- VCL_VOID_RETURN
//
// VCL_VOID_RETURN is used as a return type where void is expected,
// as in return VCL_VOID_RETURN ;
#define VCL_VOID_RETURN /*empty*/

#endif
