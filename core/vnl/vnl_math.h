// This is core/vnl/vnl_math.h
#ifndef vnl_math_h_
#define vnl_math_h_
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif
//:
// \file
// \brief Namespace with standard math functions
//
//  The vnl_math namespace provides a standard set of the simple mathematical
//  functions (min, max, sqr, sgn, rnd, abs), and some predefined constants
//  such as pi and e, which are not defined by the ANSI C++ standard.
//
//  There are complex versions defined in vnl_complex.h
//
//  That's right, M_PI is nonstandard!
//
//  Aside from e, pi and their associates the namespace also defines eps,
//  the IEEE double machine precision.  This is the smallest number
//  eps such that 1+eps != 1.
//
//  The operations are overloaded for int, float and double arguments,
//  which in combination with inlining can make them  more efficient than
//  their counterparts in the standard C library.
//
// \author Andrew W. Fitzgibbon, Oxford RRG
// \date   July 13, 1996
//
// \verbatim
//  Modifications
//   21 May 1998 AWF Removed conditional VCL_IMPLEMENT_STATIC_CONSTS, sometimes gcc needs them.
//   LSB (Modifications) 23 Jan 2001 Documentation tidied
//   Peter Vanroose - 7 Sep 2002 - maxdouble etc. replaced by vnl_numeric_traits<T>::maxval
//   Amitha Perera - 13 Sep 2002 - make constant initialization standards compliant.
//   Peter Vanroose -22 Oct 2012 - was a class, now is a namespace
//                                 also renamed functions vnl_math_isnan etc. to vnl_math::isnan
// \endverbatim

#include <vcl_cmath.h>
#include "dll.h"
#include <vxl_config.h>
#include <vnl/vnl_config.h> // for VNL_CONFIG_ENABLE_SSE2_ROUNDING
#ifdef VNL_CHECK_FPU_ROUNDING_MODE
# include <vcl_cassert.h>
#endif

// Figure out when the fast implementation can be used
#if VNL_CONFIG_ENABLE_SSE2_ROUNDING
# if !VXL_HAS_EMMINTRIN_H
#   error "Required file emmintrin.h for SSE2 not found"
# else
#   include <emmintrin.h> // sse 2 intrinsics
# endif
#endif
// Turn on fast impl when using GCC on Intel-based machines with the following exception:
//   PPC with Mac OS X
#if defined(__GNUC__) && (!defined(__APPLE__)  || !defined(__ppc__) )
# define GCC_USE_FAST_IMPL 1
#else
# define GCC_USE_FAST_IMPL 0
#endif
// Turn on fast impl when using msvc on 32 bits windows
#if defined(VCL_VC) && !defined(_WIN64)
# define VC_USE_FAST_IMPL 1
#else
# define VC_USE_FAST_IMPL 0
#endif


//: Type-accessible infinities for use in templates.
template <class T> T vnl_huge_val(T);
double   vnl_huge_val(double);
float    vnl_huge_val(float);
long int vnl_huge_val(long int);
int      vnl_huge_val(int);
short    vnl_huge_val(short);
char     vnl_huge_val(char);

//: real numerical constants
// Strictly speaking, the static declaration of the constant variables is
// redundant with the implicit behavior in C++ of objects declared const
// as defined at:
//  "C++98 7.1.1/6: ...Objects declared const and not explicitly declared
//   extern have internal linkage."
//
// Explicit use of the static keyword is used to make the code easier to
// understand.
namespace vnl_math
{
  //: pi, e and all that
  static const double e                = 2.71828182845904523536;
  static const double log2e            = 1.44269504088896340736;
  static const double log10e           = 0.43429448190325182765;
  static const double ln2              = 0.69314718055994530942;
  static const double ln10             = 2.30258509299404568402;
  static const double pi               = 3.14159265358979323846;
  static const double twopi            = 6.28318530717958647692;
  static const double pi_over_2        = 1.57079632679489661923;
  static const double pi_over_4        = 0.78539816339744830962;
  static const double pi_over_180      = 0.01745329251994329577;
  static const double one_over_pi      = 0.31830988618379067154;
  static const double two_over_pi      = 0.63661977236758134308;
  static const double deg_per_rad      = 57.2957795130823208772;
  static const double sqrt2pi          = 2.50662827463100024161;
  static const double two_over_sqrtpi  = 1.12837916709551257390;
  static const double one_over_sqrt2pi = 0.39894228040143267794;
  static const double sqrt2            = 1.41421356237309504880;
  static const double sqrt1_2          = 0.70710678118654752440;
  static const double sqrt1_3          = 0.57735026918962573106;
  static const double euler            = 0.57721566490153286061;

  //: IEEE double machine precision
  static const double eps              = 2.2204460492503131e-16;
  static const double sqrteps          = 1.490116119384766e-08;
  //: IEEE single machine precision
  static const float  float_eps        = 1.192092896e-07f;
  static const float  float_sqrteps    = 3.4526698307e-4f;

  //: Convert an angle to [0, 2Pi) range
  double angle_0_to_2pi(double angle);
  //: Convert an angle to [-Pi, Pi) range
  double angle_minuspi_to_pi(double angle);
};

// We do not want to make assumptions about unknown types that happen
// to have conversions to one of the fundamental types.  The templated
// versions of isnan, isinf, and isfinite below serve as catch-alls to
// cause linker errors if these functions are invoked with an unknown
// type.  However, due to compiler bugs, the templates sometimes match
// too often (see documentation of VCL_TEMPLATE_MATCHES_TOO_OFTEN) and
// are selected over reference-binding overloads like those in
// vnl_rational.h.  We add the catch-all templates only if the
// compiler does not have this bug. -- Brad King

// Note that the three template functions below should not be declared "inline"
// since that would override the non-inline specialisations. - PVr.
//

namespace vnl_math
{
 // isnan
 inline bool isnan(char)               { return false; }
 inline bool isnan(short)              { return false; }
 inline bool isnan(int)                { return false; }
 inline bool isnan(long)               { return false; }
 inline bool isnan(signed char)        { return false; }
 inline bool isnan(unsigned char)      { return false; }
 inline bool isnan(unsigned short)     { return false; }
 inline bool isnan(unsigned int)       { return false; }
 inline bool isnan(unsigned long)      { return false; }
#if VCL_HAS_LONG_LONG
 inline bool isnan(long long)          { return false; }
 inline bool isnan(unsigned long long) { return false; }
#endif
 bool isnan(float);
 bool isnan(double);
 bool isnan(long double);
#if !VCL_TEMPLATE_MATCHES_TOO_OFTEN
 template <class T> bool isnan(T);
#endif


 // isinf
 inline bool isinf(char)               { return false; }
 inline bool isinf(short)              { return false; }
 inline bool isinf(int)                { return false; }
 inline bool isinf(long)               { return false; }
 inline bool isinf(signed char)        { return false; }
 inline bool isinf(unsigned char)      { return false; }
 inline bool isinf(unsigned short)     { return false; }
 inline bool isinf(unsigned int)       { return false; }
 inline bool isinf(unsigned long)      { return false; }
#if VCL_HAS_LONG_LONG
 inline bool isinf(long long)          { return false; }
 inline bool isinf(unsigned long long) { return false; }
#endif
 bool isinf(float);
 bool isinf(double);
 bool isinf(long double);
#if !VCL_TEMPLATE_MATCHES_TOO_OFTEN
 template <class T> bool isinf(T);
#endif

 // isfinite
 inline bool isfinite(char)               { return true; }
 inline bool isfinite(short)              { return true; }
 inline bool isfinite(int)                { return true; }
 inline bool isfinite(long)               { return true; }
 inline bool isfinite(signed char)        { return true; }
 inline bool isfinite(unsigned char)      { return true; }
 inline bool isfinite(unsigned short)     { return true; }
 inline bool isfinite(unsigned int)       { return true; }
 inline bool isfinite(unsigned long)      { return true; }
#if VCL_HAS_LONG_LONG
 inline bool isfinite(long long)          { return true; }
 inline bool isfinite(unsigned long long) { return true; }
#endif
 bool isfinite(float);
 bool isfinite(double);
 bool isfinite(long double);
#if !VCL_TEMPLATE_MATCHES_TOO_OFTEN
 template <class T> bool isfinite(T);
#endif

 // rnd_halfinttoeven  -- round towards nearest integer
 //         halfway cases are rounded towards the nearest even integer, e.g.
 //         rnd_halfinttoeven( 1.5) ==  2
 //         rnd_halfinttoeven(-1.5) == -2
 //         rnd_halfinttoeven( 2.5) ==  2
 //         rnd_halfinttoeven( 3.5) ==  4
 //
 // We assume that the rounding mode is not changed from the default
 // one (or at least that it is always restored to the default one).

#if VNL_CONFIG_ENABLE_SSE2_ROUNDING // Fast sse2 implementation

 inline int rnd_halfinttoeven(float  x)
 {
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
   assert(fegetround()==FE_TONEAREST);
# endif
   return _mm_cvtss_si32(_mm_set_ss(x));
 }

 inline int rnd_halfinttoeven(double  x)
 {
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
   assert(fegetround()==FE_TONEAREST);
# endif
   return _mm_cvtsd_si32(_mm_set_sd(x));
 }

#elif GCC_USE_FAST_IMPL // Fast gcc asm implementation

 inline int rnd_halfinttoeven(float  x)
 {
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
   assert(fegetround()==FE_TONEAREST);
# endif
   int r;
   __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
   return r;
 }

 inline int rnd_halfinttoeven(double  x)
 {
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
   assert(fegetround()==FE_TONEAREST);
# endif
   int r;
   __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
   return r;
 }

#elif VC_USE_FAST_IMPL // Fast msvc asm implementation

 inline int rnd_halfinttoeven(float  x)
 {
   int r;
   __asm {
     fld x
     fistp r
   }
   return r;
 }

 inline int rnd_halfinttoeven(double  x)
 {
   int r;
   __asm {
     fld x
     fistp r
   }
   return r;
 }

#else // Vanilla implementation

 inline int rnd_halfinttoeven(float  x)
 {
   if (x>=0.f)
   {
      x+=0.5f;
      const int r = static_cast<int>(x);
      if ( x != static_cast<float>(r) ) return r;
      return 2*(r/2);
   }
   else
   {
      x-=0.5f;
      const int r = static_cast<int>(x);
      if ( x != static_cast<float>(r) ) return r;
      return 2*(r/2);
   }
 }

 inline int rnd_halfinttoeven(double x)
 {
   if (x>=0.)
   {
      x+=0.5;
      const int r = static_cast<int>(x);
      if ( x != static_cast<double>(r) ) return r;
      return 2*(r/2);
   }
   else
   {
      x-=0.5;
      const int r = static_cast<int>(x);
      if ( x != static_cast<double>(r) ) return r;
      return 2*(r/2);
   }
 }

#endif

 // rnd_halfintup  -- round towards nearest integer
 //         halfway cases are rounded upward, e.g.
 //         rnd_halfintup( 1.5) ==  2
 //         rnd_halfintup(-1.5) == -1
 //         rnd_halfintup( 2.5) ==  3
 //
 // Be careful: argument absolute value must be less than INT_MAX/2
 // for rnd_halfintup to be guaranteed to work.
 // We also assume that the rounding mode is not changed from the default
 // one (or at least that it is always restored to the default one).

#if VNL_CONFIG_ENABLE_SSE2_ROUNDING || GCC_USE_FAST_IMPL || VC_USE_FAST_IMPL

 inline int rnd_halfintup(float  x) { return rnd_halfinttoeven(2*x+0.5f)>>1; }
 inline int rnd_halfintup(double  x) { return rnd_halfinttoeven(2*x+0.5)>>1; }

#else // Vanilla implementation

 inline int rnd_halfintup(float  x)
 {
   x+=0.5f;
   return static_cast<int>(x>=0.f?x:(x==static_cast<int>(x)?x:x-1.f));
 }

 inline int rnd_halfintup(double x)
 {
   x+=0.5;
   return static_cast<int>(x>=0.?x:(x==static_cast<int>(x)?x:x-1.));
 }

#endif

 // rnd  -- round towards nearest integer
 //         halfway cases such as 0.5 may be rounded either up or down
 //         so as to maximize the efficiency, e.g.
 //         rnd_halfinttoeven( 1.5) ==  1 or  2
 //         rnd_halfinttoeven(-1.5) == -2 or -1
 //         rnd_halfinttoeven( 2.5) ==  2 or  3
 //         rnd_halfinttoeven( 3.5) ==  3 or  4
 //
 // We assume that the rounding mode is not changed from the default
 // one (or at least that it is always restored to the default one).

#if VNL_CONFIG_ENABLE_SSE2_ROUNDING || GCC_USE_FAST_IMPL || VC_USE_FAST_IMPL

 inline int rnd(float  x) { return rnd_halfinttoeven(x); }
 inline int rnd(double  x) { return rnd_halfinttoeven(x); }

#else // Vanilla implementation

 inline int rnd(float  x) { return x>=0.f?static_cast<int>(x+.5f):static_cast<int>(x-.5f); }
 inline int rnd(double x) { return x>=0.0?static_cast<int>(x+0.5):static_cast<int>(x-0.5); }

#endif

 // floor -- round towards minus infinity
 //
 // Be careful: argument absolute value must be less than INT_MAX/2
 // for floor to be guaranteed to work.
 // We also assume that the rounding mode is not changed from the default
 // one (or at least that it is always restored to the default one).

#if VNL_CONFIG_ENABLE_SSE2_ROUNDING // Fast sse2 implementation

 inline int floor(float  x)
 {
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
   assert(fegetround()==FE_TONEAREST);
# endif
   return _mm_cvtss_si32(_mm_set_ss(2*x-.5f))>>1;
 }

 inline int floor(double  x)
 {
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
   assert(fegetround()==FE_TONEAREST);
# endif
   return _mm_cvtsd_si32(_mm_set_sd(2*x-.5))>>1;
 }

#elif GCC_USE_FAST_IMPL // Fast gcc asm implementation

 inline int floor(float  x)
 {
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
   assert(fegetround()==FE_TONEAREST);
# endif
   int r;
   x = 2*x-.5f;
   __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
   return r>>1;
 }

 inline int floor(double  x)
 {
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
   assert(fegetround()==FE_TONEAREST);
# endif
   int r;
   x = 2*x-.5;
   __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
   return r>>1;
 }

#elif VC_USE_FAST_IMPL // Fast msvc asm implementation

 inline int floor(float  x)
 {
   int r;
   x = 2*x-.5f;
   __asm {
     fld x
     fistp r
   }
   return r>>1;
 }

 inline int floor(double  x)
 {
   int r;
   x = 2*x-.5;
   __asm {
     fld x
     fistp r
   }
   return r>>1;
 }

#else // Vanilla implementation

 inline int floor(float  x)
 {
   return static_cast<int>(x>=0.f?x:(x==static_cast<int>(x)?x:x-1.f));
 }

 inline int floor(double x)
 {
   return static_cast<int>(x>=0.0?x:(x==static_cast<int>(x)?x:x-1.0));
 }

#endif


 // ceil -- round towards plus infinity
 //
 // Be careful: argument absolute value must be less than INT_MAX/2
 // for ceil to be guaranteed to work.
 // We also assume that the rounding mode is not changed from the default
 // one (or at least that it is always restored to the default one).

#if VNL_CONFIG_ENABLE_SSE2_ROUNDING // Fast sse2 implementation

 inline int ceil(float  x)
 {
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
   assert(fegetround()==FE_TONEAREST);
# endif
    return -(_mm_cvtss_si32(_mm_set_ss(-.5f-2*x))>>1);
 }

 inline int ceil(double  x)
 {
# if defined(VNL_CHECK_FPU_ROUNDING_MODE) && defined(__GNUC__)
   assert(fegetround()==FE_TONEAREST);
# endif
    return -(_mm_cvtsd_si32(_mm_set_sd(-.5-2*x))>>1);
 }

#elif GCC_USE_FAST_IMPL // Fast gcc asm implementation

 inline int ceil(float  x)
 {
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
   assert(fegetround()==FE_TONEAREST);
# endif
   int r;
   x = -.5f-2*x;
   __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
   return -(r>>1);
 }

 inline int ceil(double  x)
 {
# ifdef VNL_CHECK_FPU_ROUNDING_MODE
   assert(fegetround()==FE_TONEAREST);
# endif
   int r;
   x = -.5-2*x;
   __asm__ __volatile__ ("fistpl %0" : "=m"(r) : "t"(x) : "st");
   return -(r>>1);
 }

#elif VC_USE_FAST_IMPL // Fast msvc asm implementation

 inline int ceil(float  x)
 {
   int r;
   x = -.5f-2*x;
   __asm {
     fld x
     fistp r
   }
   return -(r>>1);
 }

 inline int ceil(double  x)
 {
   int r;
   x = -.5-2*x;
   __asm {
     fld x
     fistp r
   }
   return -(r>>1);
 }

#else // Vanilla implementation

 inline int ceil(float  x)
 {
   return static_cast<int>(x<0.f?x:(x==static_cast<int>(x)?x:x+1.f));
 }

 inline int ceil(double x)
 {
   return static_cast<int>(x<0.0?x:(x==static_cast<int>(x)?x:x+1.0));
 }

#endif

 // abs
 inline bool               abs(bool x)               { return x; }
 inline unsigned char      abs(unsigned char x)      { return x; }
 inline unsigned char      abs(signed char x)        { return x < 0 ? static_cast<unsigned char>(-x) : x; }
 inline unsigned char      abs(char x)               { return static_cast<unsigned char>(x); }
 inline unsigned short     abs(short x)              { return x < 0 ? static_cast<unsigned short>(-x) : x; }
 inline unsigned short     abs(unsigned short x)     { return x; }
 inline unsigned int       abs(int x)                { return x < 0 ? -x : x; }
 inline unsigned int       abs(unsigned int x)       { return x; }
 inline unsigned long      abs(long x)               { return x < 0L ? -x : x; }
 inline unsigned long      abs(unsigned long x)      { return x; }
#if VCL_HAS_LONG_LONG
 inline unsigned long long abs(long long x)          { return x < 0LL ? -x : x; }
 inline unsigned long long abs(unsigned long long x) { return x; }
#endif
 inline float              abs(float x)              { return x < 0.0f ? -x : x; }
 inline double             abs(double x)             { return x < 0.0 ? -x : x; }
 inline long double        abs(long double x)        { return x < 0.0 ? -x : x; }

 // max
 inline int                max(int x, int y)                               { return (x > y) ? x : y; }
 inline unsigned int       max(unsigned int x, unsigned int y)             { return (x > y) ? x : y; }
 inline long               max(long x, long y)                             { return (x > y) ? x : y; }
 inline unsigned long      max(unsigned long x, unsigned long y)           { return (x > y) ? x : y; }
#if VCL_HAS_LONG_LONG
 inline long long          max(long long x, long long y)                   { return (x > y) ? x : y; }
 inline unsigned long long max(unsigned long long x, unsigned long long y) { return (x > y) ? x : y; }
#endif
 inline float              max(float x, float y)                           { return (x < y) ? y : x; }
 inline double             max(double x, double y)                         { return (x < y) ? y : x; }

 // min
 inline int                min(int x, int y)                               { return (x < y) ? x : y; }
 inline unsigned int       min(unsigned int x, unsigned int y)             { return (x < y) ? x : y; }
 inline long               min(long x, long y)                             { return (x < y) ? x : y; }
 inline unsigned long      min(unsigned long x, unsigned long y)           { return (x < y) ? x : y; }
#if VCL_HAS_LONG_LONG
 inline long long          min(long long x, long long y)                   { return (x < y) ? x : y; }
 inline unsigned long long min(unsigned long long x, unsigned long long y) { return (x < y) ? x : y; }
#endif
 inline float              min(float x, float y)                           { return (x > y) ? y : x; }
 inline double             min(double x, double y)                         { return (x > y) ? y : x; }

 // sqr (square)
 inline bool               sqr(bool x)               { return x; }
 inline int                sqr(int x)                { return x*x; }
 inline unsigned int       sqr(unsigned int x)       { return x*x; }
 inline long               sqr(long x)               { return x*x; }
 inline unsigned long      sqr(unsigned long x)      { return x*x; }
#if VCL_HAS_LONG_LONG
 inline long long          sqr(long long x)          { return x*x; }
 inline unsigned long long sqr(unsigned long long x) { return x*x; }
#endif
 inline float              sqr(float x)              { return x*x; }
 inline double             sqr(double x)             { return x*x; }

 // cube
 inline bool               cube(bool x)               { return x; }
 inline int                cube(int x)                { return x*x*x; }
 inline unsigned int       cube(unsigned int x)       { return x*x*x; }
 inline long               cube(long x)               { return x*x*x; }
 inline unsigned long      cube(unsigned long x)      { return x*x*x; }
#if VCL_HAS_LONG_LONG
 inline long long          cube(long long x)          { return x*x*x; }
 inline unsigned long long cube(unsigned long long x) { return x*x*x; }
#endif
 inline float              cube(float x)              { return x*x*x; }
 inline double             cube(double x)             { return x*x*x; }

 // sgn (sign in -1, 0, +1)
 inline int sgn(int x)       { return x?((x>0)?1:-1):0; }
 inline int sgn(long x)      { return x?((x>0)?1:-1):0; }
#if VCL_HAS_LONG_LONG
 inline int sgn(long long x) { return x?((x>0)?1:-1):0; }
#endif
 inline int sgn(float x)     { return (x != 0)?((x>0)?1:-1):0; }
 inline int sgn(double x)    { return (x != 0)?((x>0)?1:-1):0; }

 // sgn0 (sign in -1, +1 only, useful for reals)
 inline int sgn0(int x)         { return (x>=0)?1:-1; }
 inline int sgn0(long x)        { return (x>=0)?1:-1; }
#if VCL_HAS_LONG_LONG
 inline int sgn0(long long x)   { return (x>=0)?1:-1; }
#endif
 inline int sgn0(float x)       { return (x>=0)?1:-1; }
 inline int sgn0(double x)      { return (x>=0)?1:-1; }

 // squared_magnitude
 inline unsigned int       squared_magnitude(char               x) { return int(x)*int(x); }
 inline unsigned int       squared_magnitude(unsigned char      x) { return int(x)*int(x); }
 inline unsigned int       squared_magnitude(int                x) { return x*x; }
 inline unsigned int       squared_magnitude(unsigned int       x) { return x*x; }
 inline unsigned long      squared_magnitude(long               x) { return x*x; }
 inline unsigned long      squared_magnitude(unsigned long      x) { return x*x; }
#if VCL_HAS_LONG_LONG
 inline unsigned long long squared_magnitude(long long          x) { return x*x; }
 inline unsigned long long squared_magnitude(unsigned long long x) { return x*x; }
#endif
 inline float              squared_magnitude(float              x) { return x*x; }
 inline double             squared_magnitude(double             x) { return x*x; }
 inline long double        squared_magnitude(long double        x) { return x*x; }

 // cuberoot
 inline float  cuberoot(float  a) { return float((a<0) ? -vcl_exp(vcl_log(-a)/3) : vcl_exp(vcl_log(a)/3)); }
 inline double cuberoot(double a) { return       (a<0) ? -vcl_exp(vcl_log(-a)/3) : vcl_exp(vcl_log(a)/3); }

 // hypotenuse
 inline double      hypot(int         x, int         y) { return vcl_sqrt(double(x*x + y*y)); }
 inline float       hypot(float       x, float       y) { return float( vcl_sqrt(double(x*x + y*y)) ); }
 inline double      hypot(double      x, double      y) { return vcl_sqrt(x*x + y*y); }
 inline long double hypot(long double x, long double y) { return vcl_sqrt(x*x + y*y); }

} // end of namespace vnl_math

#endif // vnl_math_h_
