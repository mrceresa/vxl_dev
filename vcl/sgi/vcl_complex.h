#ifndef vcl_sgi_complex_h_
#define vcl_sgi_complex_h_

# include "iso/vcl_complex.h"
# undef  vcl_abs
# define vcl_abs vcl_abs
template <typename T>
inline T vcl_abs(std::complex<T> const &z) { return std::abs(z); }

# undef vcl_log10
# define vcl_log10 vcl_log10
template <typename T>
inline T vcl_log10(std::complex<T> const &z) { return ::log10(z); }

# undef vcl_exp
# define vcl_exp vcl_exp
template <typename T>
inline std::complex<T> vcl_exp(std::complex<T> const &z) { return vcl_polar(T(exp(z.real())), z.imag()); }

# undef vcl_sqrt
# define vcl_sqrt vcl_sqrt
template <typename T>
inline std::complex<T> vcl_sqrt(std::complex<T> const &z) { return vcl_exp(vcl_log(z)/T(2)); }

# undef vcl_cos
# define vcl_cos vcl_cos
template <typename T>
inline std::complex<T> vcl_cos(std::complex<T> const &z)
{ return std::complex<T>(vcl_cos(z.real())*vcl_cosh(z.imag()),-vcl_sin(z.real())*vcl_sinh(z.imag())); }

# undef vcl_sin
# define vcl_sin vcl_sin
template <typename T>
inline std::complex<T> vcl_sin(std::complex<T> const &z)
{ return std::complex<T>(vcl_sin(z.real())*vcl_cosh(z.imag()),vcl_cos(z.real())*vcl_sinh(z.imag())); }

#endif // vcl_sgi_complex_h_
