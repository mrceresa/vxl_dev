// This is core/vnl/vnl_decnum.h
#ifndef vnl_decnum_h_
#define vnl_decnum_h_
//:
// \file
// \brief Infinite precision numbers with decimal arithmetic
//
// The vnl_decnum class implements infinite precision integers and
// floating point numbers, with all necessary arithmetic, by using a
// dynamically allocated string of decimals for the mantissa.
// Implicit conversion to the system defined types short, int, and long
// is supported by overloaded operator member functions.
// Addition and subtraction operators are performed by simple decimal
// arithmetic with checks for carry flag propagation.
// The multiplication, division, and remainder operations utilize the
// naive, commonly known decimal algorithms. Beware that these could be
// considerably slower than both the operations on built-in integer
// types and those implemented in the class vnl_bignum.
// On the other hand, the I/O could be faster since no decimal <==> binary
// conversion needs to be performed.
// Rounding errors due to imprecise internal representation can never be made.
//
// Since the internal representation is decimal, there is no overhead
// for converting between a vnl_decnum and a decimal character string
// representation. Hence, there is a constructor from std::string, there
// are both << and >> operators, and a cast method to std::string, all
// of them essentially no-ops.
//
// No decimal point notation is supported, neither for input not for output;
// only the scientific notation (mantissa e exponent) is used, with integer
// mantissa (and, on output, without "e exponent" for integers).
// Beware that, even for non-integers, the division and modulo operators
// are essentially integer operations on the mantissa. E.g., the result of
// 42e-3 / 30e-4 is 1e1, while 42e-3 / 3e-3 has outcome 14, even though
// numerically 30e-4 == 3e-3. But then also 42e-3 % 30e-4 returns 12e-3
// while 42e-3 % 3e-3 returns 0, which are both compatible with
//   (a/b) * b + (a%b) = a
// which is true for other integral types as well.
//
// The difference between using e.g. 1000 and 10e2 is essentially similar to
// the use of the "P" format in Cobol: in the former case, a precision of
// four digits is kept, while in the latter only two digits are present (with
// two zeros implicitly there). Arithmetic is done with the available precision
// only, which explains the outcomes of operator/ and operator%.
//
// When none of the used vnl_decnum constructor values have an "e" (or when
// all constructors that you use in your program are on integers), no "e" is
// ever introduced except when explicitly using the method compactify().
// In that case, vnl_decnum really behaves as an integral type, like vnl_bignum.
//
// \sa vnl_bignum
//
// \author Peter Vanroose, ABIS Leuven
// \date   August 2011
//
// \verbatim
//  Modifications
//   15 Aug. 2011 - Peter Vanroose - added member exp_ (and non-integer arith)
// \endverbatim

#include <vcl_iostream.h>
#include <vcl_string.h>

class vnl_decnum
{
  char sign_;      // Sign of vnl_decnum ('+' or '-'; for zero, the sign is ' ')
  vcl_string data_;// The decimal mantissa data (absolute value)
                   // data_ consists of decimals (0-9) only, guaranteed without
                   // leading zero. This holds even for zero: represented by "".
  long exp_;       // The exponent; nonnegative for integers.

  // private constructor: arguments should satisfy the above constraints
  vnl_decnum(char s, vcl_string const& d, long e) : sign_(s), data_(d), exp_(e) {}
 public:
  vcl_string data() const { return data_; }
  char       sign() const { return sign_; }
  long       exp () const { return exp_; }
  //: void constructor - creates a zero vnl_decnum.
  vnl_decnum() : sign_(' '), data_(""), exp_(0L) {}
  //: Constructor from string
  //  This is the principal constructor for vnl_decnum; it essentially parses
  //  the input into (in that order) the sign, the mantissa, and the exponent,
  //  which turn out (surprise!) to be the three data members of this class.
  vnl_decnum(vcl_string const&);
  vnl_decnum(char const* r) { operator=(vcl_string(r)); }
  //: Creates a vnl_decnum from an unsigned long integer.
  explicit vnl_decnum(unsigned long);
  //: Creates a vnl_decnum from a long integer.
  // Uses the "unsigned long" constructor and additionally sets the sign
  explicit vnl_decnum(long r)
  : sign_(r<0 ? '-' : r>0 ? '+' : ' ')
  { vnl_decnum d((unsigned long)(r<0?-r:r)); data_=d.data(); exp_=d.exp(); }
  //: Creates a vnl_decnum from an unsigned integer.
  // Uses the "unsigned long" constructor.
  explicit vnl_decnum(unsigned int r)
  : sign_(r>0 ? '+' : ' ')
  { vnl_decnum d((unsigned long)r); data_=d.data(); exp_=d.exp(); }
  //: Creates a vnl_decnum from an integer.
  // Uses the "unsigned long" constructor and additionally sets the sign
  explicit vnl_decnum(int r)
  : sign_(r<0 ? '-' : r>0 ? '+' : ' ')
  { vnl_decnum d((unsigned long)(r<0?-r:r)); data_=d.data(); exp_=d.exp(); }
  // Copy constructor
  vnl_decnum(vnl_decnum const& r)
  : sign_(r.sign_), data_(r.data_), exp_(r.exp_) {}

  ~vnl_decnum() {}    // Destructor

  //: Implicit type conversion to a decimal string
  operator vcl_string() const;

  operator long() const;  // type conversion
  operator unsigned long() const;  // type conversion, drop the sign
  operator int() const;  // type conversion
  operator unsigned int() const;  // type conversion, drop the sign

  //: Unary plus operator
  inline vnl_decnum operator+() const { return *this; }
  //: Unary minus operator
  inline vnl_decnum operator-() const { if (sign_==' ') return *this; else return vnl_decnum(sign_=='-'?'+':'-', data_, exp_); }

  //: Left "bit" shift operator (actually: digit shift, or exponent translation)
  inline vnl_decnum operator<<(long int r) const { return vnl_decnum(sign_, data_, exp_+r); }
  inline vnl_decnum operator<<(int r) const { return vnl_decnum(sign_, data_, exp_+r); }
  //: Right "bit" shift operator (actually: digit shift, or exponent translation)
  inline vnl_decnum operator>>(long int r) const { return vnl_decnum(sign_, data_, exp_-r); }
  inline vnl_decnum operator>>(int r) const { return vnl_decnum(sign_, data_, exp_-r); }
  //: Left "bit" shift operator (actually: digit shift, or exponent translation)
  inline vnl_decnum& operator<<=(long int r) { exp_ += r; return *this; }
  inline vnl_decnum& operator<<=(int r) { exp_ += r; return *this; }
  //: Right "bit" shift operator (actually: digit shift, or exponent translation)
  inline vnl_decnum& operator>>=(long int r) { exp_ -= r; return *this; }
  inline vnl_decnum& operator>>=(int r) { exp_ -= r; return *this; }

  //: Remove all trailing zeros from the mantissa, and  increase the exponent accordingly.
  // Return the (thus modified) *this.
  // This effectively compactifies the data representation of *this, and meanwhile increases the exponent.
  // No other methods have this effect; to the contrary: e.g. operator+(1) often decreases the exponent.
  vnl_decnum& compactify();

  //: Expand integers to their non-compactified representation, i.e., without "e" notation.
  // Other operators (like + or -) might implicitly have this effect, as the implementation here indeed suggests.
  inline vnl_decnum& expand() { return *this = operator+(1L)-1L; }

  //: Assignment operator
  inline vnl_decnum& operator=(const vnl_decnum& r) { sign_=r.sign(); data_=r.data(); exp_=r.exp(); return *this; }
  //: Sum
  vnl_decnum operator+(vnl_decnum const& r) const;
  //: Difference
  inline vnl_decnum operator-(vnl_decnum const& r) const { return operator+(-r); }
  //: Product
  vnl_decnum operator*(vnl_decnum const& r) const;
  //: division operator
  // \returns integral part of quotient (long division) of this with \p r
  // \p r should not be zero (assert)
  vnl_decnum operator/(vnl_decnum const& r) const;
  //: modulo operator
  // \returns remainder of long division of this with \p r
  // \p r should not be zero (assert)
  vnl_decnum operator%(vnl_decnum const& r) const;

  inline vnl_decnum& operator+=(vnl_decnum const& r) { return *this = operator+(r); }
  inline vnl_decnum& operator-=(vnl_decnum const& r) { return *this = operator+(-r); }
  inline vnl_decnum& operator*=(vnl_decnum const& r) { return *this = operator*(r); }
  inline vnl_decnum& operator/=(vnl_decnum const& r) { return *this = operator/(r); }
  inline vnl_decnum& operator%=(vnl_decnum const& r) { return *this = operator%(r); }

  // === overloads for the above operators with other datatypes as rhs:

  inline vnl_decnum& operator=(vcl_string const& r) { return operator=(vnl_decnum(r)); }
  inline vnl_decnum& operator=(char const* r) { return operator=(vnl_decnum(vcl_string(r))); }
  inline vnl_decnum& operator=(unsigned long r) { return operator=(vnl_decnum(r)); }
  inline vnl_decnum& operator=(long r) { return operator=(vnl_decnum(r)); }
  inline vnl_decnum& operator=(unsigned  int r) { return operator=(vnl_decnum(r)); }
  inline vnl_decnum& operator=(int r) { return operator=(vnl_decnum(r)); }

  inline vnl_decnum operator+(vcl_string const& r) const { return operator+(vnl_decnum(r)); }
  inline vnl_decnum operator+(char const* r) const { return operator+(vnl_decnum(vcl_string(r))); }
  inline vnl_decnum operator+(unsigned long r) const { return operator+(vnl_decnum(r)); }
  inline vnl_decnum operator+(long r) const { return operator+(vnl_decnum(r)); }
  inline vnl_decnum operator+(unsigned int r) const { return operator+(vnl_decnum(r)); }
  inline vnl_decnum operator+(int r) const { return operator+(vnl_decnum(r)); }

  inline vnl_decnum operator-(vcl_string const& r) const { return operator-(vnl_decnum(r)); }
  inline vnl_decnum operator-(char const* r) const { return operator-(vnl_decnum(vcl_string(r))); }
  inline vnl_decnum operator-(long r) const { return operator+(vnl_decnum(-r)); }
  inline vnl_decnum operator-(unsigned long r) const { return operator-(vnl_decnum(r)); }
  inline vnl_decnum operator-(int r) const { return operator+(vnl_decnum(-r)); }
  inline vnl_decnum operator-(unsigned int r) const { return operator-(vnl_decnum(r)); }

  inline vnl_decnum operator*(vcl_string const& r) const { return operator*(vnl_decnum(r)); }
  inline vnl_decnum operator*(char const* r) const { return operator*(vnl_decnum(vcl_string(r))); }
  inline vnl_decnum operator*(long r) const { return operator*(vnl_decnum(r)); }
  inline vnl_decnum operator*(unsigned long r) const { return operator*(vnl_decnum(r)); }
  inline vnl_decnum operator*(int r) const { return operator*(vnl_decnum(r)); }
  inline vnl_decnum operator*(unsigned int r) const { return operator*(vnl_decnum(r)); }

  inline vnl_decnum operator/(vcl_string const& r) const { return operator/(vnl_decnum(r)); }
  inline vnl_decnum operator/(char const* r) const { return operator/(vnl_decnum(vcl_string(r))); }
  inline vnl_decnum operator/(unsigned long r) const { return operator/(vnl_decnum(r)); }
  inline vnl_decnum operator/(long r) const { return operator/(vnl_decnum(r)); }
  inline vnl_decnum operator/(unsigned int r) const { return operator/(vnl_decnum(r)); }
  inline vnl_decnum operator/(int r) const { return operator/(vnl_decnum(r)); }

  inline vnl_decnum operator%(vcl_string const& r) const { return operator%(vnl_decnum(r)); }
  inline vnl_decnum operator%(char const* r) const { return operator%(vnl_decnum(vcl_string(r))); }
  inline vnl_decnum operator%(unsigned long r) const { return operator%(vnl_decnum(r)); }
  inline vnl_decnum operator%(long r) const { return operator%(vnl_decnum(r)); }
  inline vnl_decnum operator%(unsigned int r) const { return operator%(vnl_decnum(r)); }
  inline vnl_decnum operator%(int r) const { return operator%(vnl_decnum(r)); }

  inline vnl_decnum& operator+=(vcl_string const& r) { return *this = operator+(vnl_decnum(r)); }
  inline vnl_decnum& operator-=(vcl_string const& r) { return *this = operator-(vnl_decnum(r)); }
  inline vnl_decnum& operator*=(vcl_string const& r) { return *this = operator*(vnl_decnum(r)); }
  inline vnl_decnum& operator/=(vcl_string const& r) { return *this = operator/(vnl_decnum(r)); }
  inline vnl_decnum& operator%=(vcl_string const& r) { return *this = operator%(vnl_decnum(r)); }

  inline vnl_decnum& operator+=(char const* r) { return *this = operator+(vcl_string(r)); }
  inline vnl_decnum& operator-=(char const* r) { return *this = operator-(vcl_string(r)); }
  inline vnl_decnum& operator*=(char const* r) { return *this = operator*(vcl_string(r)); }
  inline vnl_decnum& operator/=(char const* r) { return *this = operator/(vcl_string(r)); }
  inline vnl_decnum& operator%=(char const* r) { return *this = operator%(vcl_string(r)); }

  inline vnl_decnum& operator+=(unsigned long r) { return *this = operator+(vnl_decnum(r)); }
  inline vnl_decnum& operator-=(unsigned long r) { return *this = operator-(vnl_decnum(r)); }
  inline vnl_decnum& operator*=(unsigned long r) { return *this = operator*(vnl_decnum(r)); }
  inline vnl_decnum& operator/=(unsigned long r) { return *this = operator/(vnl_decnum(r)); }
  inline vnl_decnum& operator%=(unsigned long r) { return *this = operator%(vnl_decnum(r)); }

  inline vnl_decnum& operator+=(long r) { return *this = operator+(vnl_decnum(r)); }
  inline vnl_decnum& operator-=(long r) { return *this = operator+(vnl_decnum(-r)); }
  inline vnl_decnum& operator*=(long r) { return *this = operator*(vnl_decnum(r)); }
  inline vnl_decnum& operator/=(long r) { return *this = operator/(vnl_decnum(r)); }
  inline vnl_decnum& operator%=(long r) { return *this = operator%(vnl_decnum(r)); }

  inline vnl_decnum& operator+=(unsigned int r) { return *this = operator+(vnl_decnum(r)); }
  inline vnl_decnum& operator-=(unsigned int r) { return *this = operator-(vnl_decnum(r)); }
  inline vnl_decnum& operator*=(unsigned int r) { return *this = operator*(vnl_decnum(r)); }
  inline vnl_decnum& operator/=(unsigned int r) { return *this = operator/(vnl_decnum(r)); }
  inline vnl_decnum& operator%=(unsigned int r) { return *this = operator%(vnl_decnum(r)); }

  inline vnl_decnum& operator+=(int r) { return *this = operator+(vnl_decnum(r)); }
  inline vnl_decnum& operator-=(int r) { return *this = operator+(vnl_decnum(-r)); }
  inline vnl_decnum& operator*=(int r) { return *this = operator*(vnl_decnum(r)); }
  inline vnl_decnum& operator/=(int r) { return *this = operator/(vnl_decnum(r)); }
  inline vnl_decnum& operator%=(int r) { return *this = operator%(vnl_decnum(r)); }

  //: prefix increment (++b)
  inline vnl_decnum& operator++() { return *this = operator+(1L); }
  //: decrement
  inline vnl_decnum& operator--() { return *this = operator-(1L); }
  //: postfix increment (b++)
  inline vnl_decnum operator++(int) { vnl_decnum b=(*this); operator++(); return b; }
  //: decrement
  inline vnl_decnum operator--(int) { vnl_decnum b=(*this); operator--(); return b; }

  bool operator==(vnl_decnum const&) const; // equality
  bool operator< (vnl_decnum const&) const; // less than
  inline bool operator!=(vnl_decnum const& r) const { return !operator==(r); }
  inline bool operator> (vnl_decnum const& r) const { return r<(*this); }
  inline bool operator<=(vnl_decnum const& r) const { return !operator>(r); }
  inline bool operator>=(vnl_decnum const& r) const { return !operator<(r); }

  inline bool operator==(vcl_string const& r) const { return operator==(vnl_decnum(r)); }
  inline bool operator< (vcl_string const& r) const { return operator< (vnl_decnum(r)); }
  inline bool operator!=(vcl_string const& r) const { return operator!=(vnl_decnum(r)); }
  inline bool operator> (vcl_string const& r) const { return operator> (vnl_decnum(r)); }
  inline bool operator<=(vcl_string const& r) const { return operator<=(vnl_decnum(r)); }
  inline bool operator>=(vcl_string const& r) const { return operator>=(vnl_decnum(r)); }

  inline bool operator==(char const* r) const { return operator==(vcl_string(r)); }
  inline bool operator< (char const* r) const { return operator< (vcl_string(r)); }
  inline bool operator!=(char const* r) const { return operator!=(vcl_string(r)); }
  inline bool operator> (char const* r) const { return operator> (vcl_string(r)); }
  inline bool operator<=(char const* r) const { return operator<=(vcl_string(r)); }
  inline bool operator>=(char const* r) const { return operator>=(vcl_string(r)); }

  inline bool operator==(unsigned long r) const { return operator==(vnl_decnum(r)); }
  inline bool operator< (unsigned long r) const { return operator< (vnl_decnum(r)); }
  inline bool operator!=(unsigned long r) const { return operator!=(vnl_decnum(r)); }
  inline bool operator> (unsigned long r) const { return operator> (vnl_decnum(r)); }
  inline bool operator<=(unsigned long r) const { return operator<=(vnl_decnum(r)); }
  inline bool operator>=(unsigned long r) const { return operator>=(vnl_decnum(r)); }

  inline bool operator==(long r) const { return operator==(vnl_decnum(r)); }
  inline bool operator< (long r) const { return operator< (vnl_decnum(r)); }
  inline bool operator!=(long r) const { return operator!=(vnl_decnum(r)); }
  inline bool operator> (long r) const { return operator> (vnl_decnum(r)); }
  inline bool operator<=(long r) const { return operator<=(vnl_decnum(r)); }
  inline bool operator>=(long r) const { return operator>=(vnl_decnum(r)); }

  inline bool operator==(unsigned int r) const { return operator==(vnl_decnum(r)); }
  inline bool operator< (unsigned int r) const { return operator< (vnl_decnum(r)); }
  inline bool operator!=(unsigned int r) const { return operator!=(vnl_decnum(r)); }
  inline bool operator> (unsigned int r) const { return operator> (vnl_decnum(r)); }
  inline bool operator<=(unsigned int r) const { return operator<=(vnl_decnum(r)); }
  inline bool operator>=(unsigned int r) const { return operator>=(vnl_decnum(r)); }

  inline bool operator==(int r) const { return operator==(vnl_decnum(r)); }
  inline bool operator< (int r) const { return operator< (vnl_decnum(r)); }
  inline bool operator!=(int r) const { return operator!=(vnl_decnum(r)); }
  inline bool operator> (int r) const { return operator> (vnl_decnum(r)); }
  inline bool operator<=(int r) const { return operator<=(vnl_decnum(r)); }
  inline bool operator>=(int r) const { return operator>=(vnl_decnum(r)); }

  inline vnl_decnum abs() const { return sign_=='-' ? operator-() : *this; }
  inline vnl_decnum trunc() const { return exp_>=0L ? *this : vnl_decnum(sign_,data_.substr(0L,data_.length()+exp_),0L); }
  inline vnl_decnum roundup() const { return operator==(trunc()) ? *this : sign_=='-' ? trunc()-1 : trunc()+1; }
  inline vnl_decnum floor() const { return sign_=='-' ? roundup() : trunc(); }
  inline vnl_decnum ceil() const { return sign_=='-' ? trunc() : roundup(); }
  inline vnl_decnum pow(unsigned long p) const { return p==0L ? vnl_decnum(1L) : p==1L ? *this : pow(p/2)*pow((p+1)/2); }

 private: // === Helper functions ===
  //: Returns the sum of the two first arguments (interpreted as mantissas with the same exponent).
  // Both arguments should consist of digits only.
  // The third argument will be used as the exponent of the result.
  static vnl_decnum plus(vcl_string const&, vcl_string const&, long);
  //: Returns the difference of the two first arguments (interpreted as mantissas with the same exponent).
  // Both arguments should consist of digits only
  // and the first one should be numerically larger than the second one.
  // The third argument will be used as the exponent of the result.
  static vnl_decnum minus(vcl_string const&, vcl_string const&, long);
  //: This is "operator<" for strings.
  // The arguments should consist of digits only (interpreted as mantissas with the same exponent).
  // The shorter of the two arguments is implicitly zero-padded.
  static bool comp(vcl_string const&, vcl_string const&);
  // Append n zeros to the source string, and return the new padded string
  inline static vcl_string add_zeros(vcl_string const& source, unsigned long n)
  { vcl_string d = source; while (n--!=0) d.push_back('0'); return d; }
  //: Returns the product of the two arguments.
  // The first argument should consist of digits only;
  // the second argument should be a single digit.
  static vcl_string mult(vcl_string const&, char);
  //: Returns the largest one-significant-digit divisor of the two arguments.
  // The largest multiple of the second argument not larger than the first one
  // is returned in the second argument.
  // (I.e.: the product of the original second argument with the returned divisor.)
  // The arguments should consist of digits only
  // and the first one should be numerically larger than the second one.
  static vcl_string div(vcl_string const&, vcl_string&);
};

//: decimal output
// \relatesalso vnl_decnum
inline vcl_ostream& operator<<(vcl_ostream& s, vnl_decnum const& r)
{ return s << vcl_string(r); }

//: decimal input
// \relatesalso vnl_decnum
vcl_istream& operator>>(vcl_istream& s, vnl_decnum& r);

inline vnl_decnum ceil(vnl_decnum const& x) { return x.ceil(); }
inline vnl_decnum floor(vnl_decnum const& x) { return x.floor(); }
inline vnl_decnum pow(vnl_decnum const& x, unsigned long p) { return x.pow(p); }

inline vnl_decnum vnl_math_abs(vnl_decnum const& x) { return x.abs(); }
inline vnl_decnum vnl_math_sqr(vnl_decnum const& x) { return x*x; }
inline vnl_decnum vnl_math_cube(vnl_decnum const& x) { return x*x*x; }
inline vnl_decnum vnl_math_squared_magnitude(vnl_decnum const& x) { return x*x; }
inline bool vnl_math_isnan(vnl_decnum const&) { return false; }
inline bool vnl_math_isfinite(vnl_decnum const& x) { return true; }
inline vnl_decnum vnl_math_max(vnl_decnum const& x, vnl_decnum const& y) { return (x < y) ? y : x; }
inline vnl_decnum vnl_math_min(vnl_decnum const& x, vnl_decnum const& y) { return (x < y) ? x : y; }
inline int vnl_math_sgn(vnl_decnum x) { return x.sign()==' '?0:x.sign()=='+'?1:-1; }
inline int vnl_math_sgn0(vnl_decnum x) { return x.sign()=='-'?-1:1; }

#endif // vnl_decnum_h_
