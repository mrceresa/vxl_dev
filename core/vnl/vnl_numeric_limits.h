//-*- c++ -*-------------------------------------------------------------------
#ifndef vnl_numeric_limits_h_
#define vnl_numeric_limits_h_
#ifdef __GNUC__
#pragma interface "vnl_numeric_limits.h"
#endif
//
// Class : numeric_limits
//
// .SECTION Description
//    Implementation of the May 96 ANSI Draft Working Paper (DWP)
//    numeric_limits class.  Numbering in
//    the documentation below refers to section 18.2 of the DWP.
//
// .NAME        numeric_limits - Standard limits for numeric datatypes
// .LIBRARY     vnl
// .HEADER	Numerics Package
// .INCLUDE     vnl/vnl_numeric_limits.h
// .FILE        vnl/vnl_numeric_limits.cxx
// .SECTION Author
//     Andrew W. Fitzgibbon, Oxford RRG, 28 Aug 96
//
// .SECTION Modifications:
//     <none yet>
//
//-----------------------------------------------------------------------------

#include <vcl/vcl_compiler.h>

// -- 18.2.1.3  Type float_round_style                     [lib.round.style]

enum vnl_float_round_style {
  vnl_round_indeterminate       = -1,
  vnl_round_toward_zero         =  0,
  vnl_round_to_nearest          =  1,
  vnl_round_toward_infinity     =  2,
  vnl_round_toward_neg_infinity =  3
};

#ifdef infinity
# error
#endif

template<class T>
class vnl_numeric_limits {
public:

// --  The member is_specialized makes it possible to distinguish between
// scalar types, which have specializations, and non-scalar types, which do
// not.
  static const bool is_specialized;

// --  Minimum finite value. (Equivalent to CHAR_MIN, SHRT_MIN, FLT_MIN,
// DBL_MIN, etc)
// 
//  For  floating types with denormalization, returns the minimum positive
//   normalized value, denorm_min().
// 
//  Meaningful for all specializations in which  is_bounded  ==  true,  or
//   is_bounded == false && is_signed == false.
  static T min();

// --  Maximum finite value. Equivalent to CHAR_MAX, SHRT_MAX, FLT_MAX, DBL_MAX, etc.
//  Meaningful for all specializations in which is_bounded == true.
  static T max();
  
//  Number of radix digits which can be represented without change.
//  For built-in integer types, the number of non-sign bits in the
//   representation.
//  For floating point types, the number of radix digits in the mantissa.
//   Equivalent to FLT_MANT_DIG, DBL_MANT_DIG, LDBL_MANT_DIG.
  static const int  digits;

// --  Number of base 10 digits which can be represented without change.
//   Equivalent to FLT_DIG, DBL_DIG, LDBL_DIG.
// Meaningful for all specializations in which is_bounded == true.
  static const int  digits10;

  static const bool is_signed;   // True if the type is signed.
  static const bool is_integer;  // True if the type is integer

// --  True if the type uses an exact representation.  All integer types are
//    exact, but not vice versa.  For example, rational and fixed-exponent
//    representations are exact but not integer.
  static const bool is_exact;

// --  For floating types, specifies the base or radix of the exponent
//    representation (often 2).  Equivalent to FLT_RADIX.
// 
//  For integer types, specifies the base of the representation -
//    distinguishes types with bases other than 2 (e.g. BCD).
  static const int  radix;

// --  Machine epsilon: The difference between 1 and the least value greater
//    than 1 that is representable.  Equivalent to FLT_EPSILON, DBL_EPSILON,
//    LDBL_EPSILON.
//  Meaningful only for floating point types.
  static T epsilon();
  
//  Measure of the maximum rounding error.  This has a precise definition in
//    the Language Independent Arithmetic (LIA-1) standard.  Required by LIA-1.
  static T round_error();


// --  Minimum negative integer such that radix raised to that  power  is  in
//    range.  Equivalent to FLT_MIN_EXP, DBL_MIN_EXP, LDBL_MIN_EXP.
//  Meaningful only for floating point types.
  static const int  min_exponent;

// --  Minimum  negative  integer  such  that  10  raised to that power is in
//    range. Equivalent to FLT_MIN_10_EXP, DBL_MIN_10_EXP, LDBL_MIN_10_EXP.
//  Meaningful only for floating point types.
  static const int  min_exponent10;
  
//  Maximum positive integer such that radix raised to that  power  is  in
//    range. Equivalent to FLT_MAX_EXP, DBL_MAX_EXP, LDBL_MAX_EXP.
//  Meaningful only for floating point types.
  static const int  max_exponent;

// --  Maximum  positive  integer  such  that  10  raised to that power is in
//    range. Equivalent to FLT_MAX_10_EXP, DBL_MAX_10_EXP, LDBL_MAX_10_EXP.
//  Meaningful only for floating point types.
  static const int  max_exponent10;

// --  True if the type has a representation for positive infinity.
//  Meaningful only for floating point types.
//  Shall be true for all specializations in which is_iec559 == true.
  static const bool has_infinity;

// --  True if the type has a representation for a quiet (non-signaling)
//   ``Not a Number.''.  RLIA
//  Meaningful only for floating point types.
//  Shall be true for all specializations in which is_iec559 == true.
  static const bool has_quiet_NaN;

// --  True if the type has a representation for a signaling
//    ``Not a Number.''.
//  Meaningful only for floating point types.
//  Shall be true for all specializations in which is_iec559 == true.
  static const bool has_signaling_NaN;

// --  True if the type allows denormalized values (variable number of exponent
//    bits).
//  Meaningful only for flotaing point types.
  static const bool has_denorm;

// --  Representation of positive infinity, if available. 
  static T infinity();

// --  Representation of a quiet ``Not a Number,'' if available.
  static T quiet_NaN();

// --  Representation of a signaling ``Not a Number,'' if available.
  static T signaling_NaN();

// --  Minimum positive denormalized value.
//  Meaningful for all floating point types.
//  In specializations for which has_denorm == false, returns the  minimum
//    positive normalized value.
//  For types with has_denorm == false, the member denorm_min() shall
//    return the same value as the member min().
  static T denorm_min();

// --  True if and only if the type adheres to IEC 559 standard.
//    International Electrotechnical Commission standard 559 is the same as
//    IEEE 754.
  static const bool is_iec559;

// --  True if the set of values representable by the type is finite.
//    All built-in types are bounded, this member would be false for arbitrary
//    precision types.
  static const bool is_bounded;

// --  True if the type is modulo. A type is modulo if it is  possible  to
//    add  two  positive  numbers  and have a result which wraps around to a
//    third number which is less.
//  Generally, this is false for floating types, true for unsigned integers,
//    and true for signed integers on most machines.
  static const bool is_modulo;

// --  True if trapping is implemented for the type.
  static const bool traps;

// --  True if tinyness is detected before rounding. Refer to IEC 559.
  static const bool tinyness_before;

// --  The rounding style for the type. Equivalent to FLT_ROUNDS.
//    Specializations for integer types shall return round_toward_zero.
  static const vnl_float_round_style round_style;
};

// SPECIALIZATIONS :

// -------------------- #include <vnl/vnl_numeric_limits_int.h>

#ifdef IUE_64_BIT
# define vnl_tmp_signed32 ????
#else
# define vnl_tmp_signed32 int
#endif

VCL_DEFINE_SPECIALIZATION
class vnl_numeric_limits<vnl_tmp_signed32> {
public:
  static const bool is_specialized VCL_STATIC_CONST_INIT_INT(true);
  inline static int min() { return -0x7fffffff; }
  inline static int max() { return  0x7fffffff; }
  static const int digits   VCL_STATIC_CONST_INIT_INT(31);
  static const int digits10 VCL_STATIC_CONST_INIT_INT(9);
  static const bool is_signed  VCL_STATIC_CONST_INIT_INT(true);
  static const bool is_integer VCL_STATIC_CONST_INIT_INT(true);
  static const bool is_exact   VCL_STATIC_CONST_INIT_INT(true);
  static const int radix VCL_STATIC_CONST_INIT_INT(2);
  inline static int epsilon()     { return 0; }
  inline static int round_error() { return 0; }
  static const int min_exponent   VCL_STATIC_CONST_INIT_INT(-31);
  static const int min_exponent10 VCL_STATIC_CONST_INIT_INT(-9);
  static const int max_exponent   VCL_STATIC_CONST_INIT_INT(31);
  static const int max_exponent10 VCL_STATIC_CONST_INIT_INT(9);
  static const bool has_infinity      VCL_STATIC_CONST_INIT_INT(false);
  static const bool has_quiet_NaN     VCL_STATIC_CONST_INIT_INT(false);
  static const bool has_signaling_NaN VCL_STATIC_CONST_INIT_INT(false);
  static const bool has_denorm        VCL_STATIC_CONST_INIT_INT(false);
  static int infinity() { return max(); }
  static int quiet_NaN();
  static int signaling_NaN();
  inline static int denorm_min()    { return min(); }
  static const bool is_iec559  VCL_STATIC_CONST_INIT_INT(false);
  static const bool is_bounded VCL_STATIC_CONST_INIT_INT(true);
  static const bool is_modulo  VCL_STATIC_CONST_INIT_INT(true);
  static const bool traps      VCL_STATIC_CONST_INIT_INT(false);
  static const bool tinyness_before VCL_STATIC_CONST_INIT_INT(false);
  static const vnl_float_round_style round_style VCL_STATIC_CONST_INIT_INT(vnl_round_toward_zero);
};

#undef vnl_tmp_signed32

// -------------------- #include <vnl/vnl_numeric_limits_float.h>

// IEEE 754 single precision
VCL_DEFINE_SPECIALIZATION
class vnl_numeric_limits<float> {
public:
  static const bool is_specialized VCL_STATIC_CONST_INIT_INT(true);
  inline static float min() { return 1.17549435E-38F; }
  inline static float max() { return 3.40282347E+38F; }
  static const int digits   VCL_STATIC_CONST_INIT_INT(24);
  static const int digits10 VCL_STATIC_CONST_INIT_INT(6);
  static const bool is_signed  VCL_STATIC_CONST_INIT_INT(true);
  static const bool is_integer VCL_STATIC_CONST_INIT_INT(false);
  static const bool is_exact   VCL_STATIC_CONST_INIT_INT(false);
  static const int radix VCL_STATIC_CONST_INIT_INT(2);
  inline static float epsilon()     { return 1.19209290E-07F; }
  inline static float round_error() { return 0.5F; }
  static const int min_exponent   VCL_STATIC_CONST_INIT_INT(-125);
  static const int min_exponent10 VCL_STATIC_CONST_INIT_INT(-37);
  static const int max_exponent   VCL_STATIC_CONST_INIT_INT(128);
  static const int max_exponent10 VCL_STATIC_CONST_INIT_INT(38);
  static const bool has_infinity      VCL_STATIC_CONST_INIT_INT(true);
  static const bool has_quiet_NaN     VCL_STATIC_CONST_INIT_INT(true);
  static const bool has_signaling_NaN VCL_STATIC_CONST_INIT_INT(true);
  static const bool has_denorm        VCL_STATIC_CONST_INIT_INT(false);
  static float infinity();
  static float quiet_NaN();
  static float signaling_NaN();
  inline static float denorm_min()    { return min(); }
  static const bool is_iec559  VCL_STATIC_CONST_INIT_INT(true);
  static const bool is_bounded VCL_STATIC_CONST_INIT_INT(true);
  static const bool is_modulo  VCL_STATIC_CONST_INIT_INT(false);
  static const bool traps      VCL_STATIC_CONST_INIT_INT(true);
  static const bool tinyness_before VCL_STATIC_CONST_INIT_INT(true);
  static const vnl_float_round_style round_style VCL_STATIC_CONST_INIT_INT(vnl_round_to_nearest);
};

// -------------------- #include <vnl/vnl_numeric_limits_double.h>

// IEEE 754 double precision with denorm
VCL_DEFINE_SPECIALIZATION
class vnl_numeric_limits<double> {
public:
  static const bool is_specialized VCL_STATIC_CONST_INIT_INT(true);
  inline static double min() { return 2.2250738585072014e-308; }
  inline static double max() { return 1.7976931348623157e+308; }
  static const int digits   VCL_STATIC_CONST_INIT_INT(53);
  static const int digits10 VCL_STATIC_CONST_INIT_INT(15);
  static const bool is_signed  VCL_STATIC_CONST_INIT_INT(true);
  static const bool is_integer VCL_STATIC_CONST_INIT_INT(false);
  static const bool is_exact   VCL_STATIC_CONST_INIT_INT(false);
  static const int radix VCL_STATIC_CONST_INIT_INT(2);
  inline static double epsilon()     { return 2.220446049250313e-16; }
  inline static double round_error() { return 0.5; }
  static const int min_exponent   VCL_STATIC_CONST_INIT_INT(-1021);
  static const int min_exponent10 VCL_STATIC_CONST_INIT_INT(-307);
  static const int max_exponent   VCL_STATIC_CONST_INIT_INT(1024);
  static const int max_exponent10 VCL_STATIC_CONST_INIT_INT(308);
  static const bool has_infinity      VCL_STATIC_CONST_INIT_INT(true);
  static const bool has_quiet_NaN     VCL_STATIC_CONST_INIT_INT(true);
  static const bool has_signaling_NaN VCL_STATIC_CONST_INIT_INT(true);
  static const bool has_denorm        VCL_STATIC_CONST_INIT_INT(false);
  static double infinity();
  static double quiet_NaN();
  static double signaling_NaN();
  inline static double denorm_min() { return /* 5e-324 */ min(); }
  static const bool is_iec559  VCL_STATIC_CONST_INIT_INT(true);
  static const bool is_bounded VCL_STATIC_CONST_INIT_INT(true);
  static const bool is_modulo  VCL_STATIC_CONST_INIT_INT(false);
  static const bool traps      VCL_STATIC_CONST_INIT_INT(true);
  static const bool tinyness_before VCL_STATIC_CONST_INIT_INT(true);
  static const vnl_float_round_style round_style VCL_STATIC_CONST_INIT_INT(vnl_round_to_nearest);
};

#endif // vnl_numeric_limits_h_
