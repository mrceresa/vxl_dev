#ifdef __GNUC__
#pragma implementation
#endif

// This is vxl/vnl/vnl_real_polynomial.cxx

//:
// \file
// \brief Evaluation of real polynomials - the implemenation
// \author Andrew W. Fitzgibbon, Oxford RRG 23 Aug 96
//
// Modifications
// IMS (Manchester) 14/03/2001: Added Manchester IO scheme

#include "vnl_real_polynomial.h"
#include <vcl_iostream.h>
#include <vcl_complex.h>
#include <vcl_cmath.h>

// This is replacing a member template...
template <class T>
T vnl_real_polynomial_evaluate(double const *a, int n, T const& x)
{
  --n;
  T acc = a[n];
  T xn = x;
  do {
    acc += a[--n] * xn;
    xn *= x;
  } while (n);

  return acc;
}

#ifdef WIN32
#define SELECT(T) <T >
#else
#define SELECT(T)
#endif

//: Instantiate templates before use
template double vnl_real_polynomial_evaluate SELECT(double )
      (double const*,int,double const&);
template vcl_complex<double> vnl_real_polynomial_evaluate SELECT(vcl_complex<double>)
      (double const*,int,vcl_complex<double> const&);

//: Evaluate polynomial at value x
double vnl_real_polynomial::evaluate(double x) const
{
  return vnl_real_polynomial_evaluate SELECT(double)(coeffs_.data_block(), coeffs_.size(), x);
}


//: Evaluate polynomial at complex value x
vcl_complex<double> vnl_real_polynomial::evaluate(vcl_complex<double> const& x) const
{
  return vnl_real_polynomial_evaluate SELECT(vcl_complex<double>)
     (coeffs_.data_block(), coeffs_.size(), x);
}

//: Evaluate derivative at value x. Not implemented.
double vnl_real_polynomial::devaluate(double /*x*/) const
{
  return HUGE_VAL;
}


//: Evaluate derivative at complex value x. Not implemented.
vcl_complex<double> vnl_real_polynomial::devaluate(vcl_complex<double> const& /*x*/) const
{
  return HUGE_VAL;
}

//: Evaluate integral at x (assuming constant of integration is zero)
double vnl_real_polynomial::evaluate_integral(double x) const
{
  int d = coeffs_.size()-1;
	const double* f = coeffs_.data_block();
	double sum = 0.0;
	int di=1;
	double xi=x;
	for (int i=d;i>=0;--i)
	{
	  sum += f[i]*xi/di;
		xi*=x;
		di++;
	}

	return sum;
}

//: Evaluate integral between x1 and x2
double vnl_real_polynomial::evaluate_integral(double x1, double x2) const
{
  return evaluate_integral(x2)-evaluate_integral(x1);
}

//: Returns sum of two polynomials f1(x)+f2(x)
vnl_real_polynomial operator+(const vnl_real_polynomial& f1, const vnl_real_polynomial& f2)
{
  // Degree of result is highest of the two inputs
  int d1=f1.degree();
	int d2=f2.degree();
	int d = d1;
	if (d2>d) d=d2;

	vnl_real_polynomial sum(d);

	// Coefficients are stored such that f(i) is coef. on x^(d-i)
	for (int i=0;i<=d;++i)
	{
	  sum[d-i]=0.0;
		if (i<=d1) sum[d-i]+=f1[d1-i];
		if (i<=d2) sum[d-i]+=f2[d2-i];
	}

	return sum;
}

//: Returns sum of two polynomials f1(x)-f2(x)
vnl_real_polynomial operator-(const vnl_real_polynomial& f1, const vnl_real_polynomial& f2)
{
  // Degree of result is highest of the two inputs
  int d1=f1.degree();
	int d2=f2.degree();
	int d = d1;
	if (d2>d) d=d2;

	vnl_real_polynomial sum(d);

	// Coefficients are stored such that f(i) is coef. on x^(d-i)
	for (int i=0;i<=d;++i)
	{
	  sum[d-i]=0.0;
		if (i<=d1) sum[d-i]+=f1[d1-i];
		if (i<=d2) sum[d-i]-=f2[d2-i];
	}

	return sum;
}

//: Returns polynomial which is product of two polynomials f1(x)*f2(x)
vnl_real_polynomial operator*(const vnl_real_polynomial& f1, const vnl_real_polynomial& f2)
{
  int d1=f1.degree();
	int d2=f2.degree();
	int d = d1+d2;

	vnl_real_polynomial sum(d);
  sum.coefficients().fill(0.0);

	for (int i=0;i<=d1;++i)
	  for (int j=0;j<=d2;++j)
		  sum[d-(i+j)] += (f1[d1-i]*f2[d2-j]);

  return sum;
}

//: Returns RMS difference between f1 and f2 over range [x1,x2]
//  $\sqrt{1/\x2-x1|\int_{x1}^{x2}(f1(x)-f2(x))^2dx}$
double vnl_rms_difference(const vnl_real_polynomial& f1, const vnl_real_polynomial& f2,
                          double x1, double x2)
{
  double dx = vcl_fabs(x2-x1);
	if (dx==0.0) return 0;

  vnl_real_polynomial df(f2-f1);
	vnl_real_polynomial df2(df*df);
	double area = vcl_fabs(df2.evaluate_integral(x2-x1));
  return sqrt(area/dx);
}

