// This is core/vpdl/vpdl_kernel_gaussian_sfbw.h
#ifndef vpdl_kernel_gaussian_sfbw_h_
#define vpdl_kernel_gaussian_sfbw_h_
//:
// \file
// \author Matthew Leotta
// \date February 24, 2009
// \brief A fixed bandwidth spherical Gaussian kernel distribution
//
// \verbatim
//  Modifications
//   None
// \endverbatim

#include <vpdl/vpdl_kernel_base.h>
#include <vnl/vnl_erf.h>
#include <vcl_limits.h>
#include <vcl_cassert.h>

//: A fixed bandwidth spherical Gaussian kernel distribution
// The bandwidth is the standard deviation of the Gaussian kernel.
template<class T, unsigned int n=0>
class vpdl_kernel_gaussian_sfbw : public vpdl_kernel_fbw_base<T,n>
{
 public:
  //: the data type used for vectors (e.g. the mean)
  typedef typename vpdl_base_traits<T,n>::vector vector;
  //: the data type used for matrices (e.g. covariance)
  typedef typename vpdl_base_traits<T,n>::matrix matrix;

  //: Default Constructor
  vpdl_kernel_gaussian_sfbw(unsigned int var_dim = n)
  : vpdl_kernel_fbw_base<T,n>(var_dim) {}

  //: Constructor - from sample centers and bandwidth (variance)
  vpdl_kernel_gaussian_sfbw(const vcl_vector<vector>& samples, 
                            T bandwidth = T(1))
  : vpdl_kernel_fbw_base<T,n>(samples,bandwidth) {}
  

  //: Create a copy on the heap and return base class pointer
  virtual vpdl_distribution<T,n>* clone() const
  {
    return new vpdl_kernel_gaussian_sfbw<T,n>(*this);
  }

  //: Evaluate the probability density at a point
  virtual T prob_density(const vector& pt) const
  {
    const unsigned int nc = this->num_components();
    if (nc <= 0)
      return 0.0;
    
    const unsigned int d = this->dimension();    
    T k = norm_const()/nc;
    T sum = T(0);
    typedef typename vcl_vector<vector>::const_iterator vitr;
    for(vitr s=this->samples().begin(); s!=this->samples().end(); ++s) {
      T ssd = T(0);
      for (unsigned int i=0; i<d; ++i) {
        T tmp = (index(pt,i)-index(*s,i))/this->bandwidth();
        ssd += tmp*tmp;
      }
      sum += vcl_exp(-0.5*ssd);
    }
        
    return k*sum;
  }


  //: Evaluate the cumulative distribution function at a point
  // This is the integral of the density function from negative infinity
  // (in all dimensions) to the point in question
  virtual T cumulative_prob(const vector& pt) const
  {
    const unsigned int nc = this->num_components();
    if (nc <= 0)
      return 0.0;
    
    const unsigned int d = this->dimension();
    double s2 = 1/(this->bandwidth()*vcl_sqrt(2.0));
    
    double sum = 0.0;
    typedef typename vcl_vector<vector>::const_iterator vitr;
    for(vitr s=this->samples().begin(); s!=this->samples().end(); ++s) {
      double val = 1.0;
      for (unsigned int i=0; i<d; ++i) {
        val *= 0.5*vnl_erf(s2*(index(pt,i)-index(*s,i))) + 0.5;
      }
      sum += val;
    }
    return static_cast<T>(sum/nc);
  }

  //: The probability of being in an axis-aligned box
  // The box is defined by two points, the minimum and maximum.
  // Reimplemented for effeciency since the axis are independent
  T box_prob(const vector& min_pt, const vector& max_pt) const
  {
    const unsigned int nc = this->num_components();
    if (nc <= 0)
      return 0.0;
    
    const unsigned int dim = this->dimension();
    double s2 = 1/(this->bandwidth()*vcl_sqrt(2.0));
    
    double sum = 0.0;
    typedef typename vcl_vector<vector>::const_iterator vitr;
    for(vitr s=this->samples().begin(); s!=this->samples().end(); ++s) {
      double prob = 1.0;
      for (unsigned int i=0; i<dim; ++i){
        if (index(max_pt,i)<=index(min_pt,i))
          return T(0);
        prob *= (vnl_erf(s2*(index(max_pt,i)-index(*s,i))) -
                 vnl_erf(s2*(index(min_pt,i)-index(*s,i))))/2;
      }
      sum += prob;
    }
    return static_cast<T>(sum/nc);
  }


  //: Compute the covariance of the distribution.
  virtual void compute_covar(matrix& covar) const
  {
    const unsigned int d = this->dimension();
    const unsigned int nc = this->num_components();
    vector mean;
    m_init(covar,d,T(0));
    v_init(mean,d,T(0));
    typedef typename vcl_vector<vector>::const_iterator samp_itr;
    for (samp_itr s = this->samples().begin(); s != this->samples().end(); ++s){
      covar += outer_product(*s,*s);
      mean += *s;
    }
    mean /= nc;
    covar /= nc;
    covar -= outer_product(mean,mean);
    T var = this->bandwidth()*this->bandwidth();
    for(unsigned int i=0; i<d; ++i)
      index(covar,i,i) += var;
  }

 protected:
  //: compute the normalization constant (independent of sample point).
  // Can be precomputed when evaluating at multiple points
  // non-virtual for efficiency
  T norm_const() const
  {
    const unsigned int dim = this->dimension();
    double v2pi = this->bandwidth()*this->bandwidth()*2.0*vnl_math::pi;
    double denom = v2pi;
    for(unsigned int i=1; i<dim; ++i)
      denom *= v2pi;
    
    return static_cast<T>(vcl_sqrt(1/denom));
  }
  
};


#endif // vpdl_kernel_gaussian_sfbw_h_
