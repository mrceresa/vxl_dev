// This is mul/vpdfl/vpdfl_pc_gaussian_builder.h
#ifndef vpdfl_pc_gaussian_builder_h
#define vpdfl_pc_gaussian_builder_h
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif
//:
// \file
// \brief Interface for Multi-variate Principle Component Gaussian PDF Builder.
// \author Ian Scott
// \date 21-Jul-2000
//
// \verbatim
// Modifications
// 23 April 2001 IMS - Ported to VXL
// \endverbatim


//=======================================================================
// inclusions
#include <vpdfl/vpdfl_gaussian_builder.h>


//=======================================================================

class vpdfl_gaussian;
class vpdfl_pdf_base;
class vpdfl_pc_gaussian;

//: Class to build vpdfl_pc_gaussian objects with a fixed number of principle components
class vpdfl_pc_gaussian_builder : public vpdfl_gaussian_builder
{
 public:
  enum partitionMethods { fixed, proportionate };
 private:

  vpdfl_pc_gaussian& gaussian(vpdfl_pdf_base& model) const;

  //: The method used to decide how to calculate the number of principle components.
  // defaults to fixed.
  partitionMethods partitionMethod_;

  //: The proportion of variance that should be encoded with the principle components..
  // Isn't used by default..
  double proportionOfVariance_;

  //: The number of components to represent in the principle space.
  unsigned fixed_partition_;

 public:
  //: Dflt ctor
  vpdfl_pc_gaussian_builder();

  //: Destructor
  virtual ~vpdfl_pc_gaussian_builder();

  //: Create empty model
  virtual vpdfl_pdf_base* new_model() const;

  //: Build default model with given mean
  virtual void build(vpdfl_pdf_base& model,
                     const vnl_vector<double>& mean) const;

  //: Build model from data
  virtual void build(vpdfl_pdf_base& model,
                     mbl_data_wrapper<vnl_vector<double> >& data) const;

  //: Build model from weighted data
  virtual void weighted_build(vpdfl_pdf_base& model,
                              mbl_data_wrapper<vnl_vector<double> >& data,
                              const vcl_vector<double>& wts) const;

  //: Computes mean and covariance of given data
  void mean_covar(vnl_vector<double>& mean, vnl_matrix<double>& covar,
                  mbl_data_wrapper<vnl_vector<double> >& data) const;

  //: Decide where to partition an Eigenvector space
  // Returns the number of principle components to be used.
  // Pass in the eigenvalues (eVals), the number of samples
  // that went to make up this Gaussian (nSamples), and the noise floor
  // for the dataset. The method may use simplified algorithms if
  // you indicate that the number of samples or noise floor is unknown
  // (by setting the latter parameters to 0.)
  virtual unsigned decide_partition(const vnl_vector<double>& eVals,
                                    unsigned nSamples=0, double noise=0.0) const;

  //: Return the number of principle components when using fixed partition.
  int fixed_partition() const
  {
    if (partitionMethod_ == vpdfl_pc_gaussian_builder::fixed) return fixed_partition_;
    else return -1;
  }

  //: Set the number of principle components when using fixed partition.
  void set_fixed_partition(int n_principle_components);

  //: Use proportion of variance to decide on the number of principle components.
  // Specify the proportion (between 0 and 1).
  // The default setting uses a fixed number of principle components.
  void set_proportion_partition( double proportion);

  //: Find the proportion of variance to decide on the number of principle components.
  // returns a negative value if not using proportion of variance method.
  double proportion_partition() const
  {
    if (partitionMethod_ == proportionate) return proportionOfVariance_;
    else return -1.0;
  }

  //: How is the partition between principle and complementary spaces
  partitionMethods partition_method() const
  {return partitionMethod_;}

  //: Version number for I/O
  short version_no() const;

  //: Name of the class
  virtual vcl_string is_a() const;

  //: Does the name of the class match the argument?
  virtual bool is_class(vcl_string const& s) const;

  //: Create a copy on the heap and return base class pointer
  virtual vpdfl_builder_base* clone() const;

  //: Print class to os
  virtual void print_summary(vcl_ostream& os) const;

  //: Save class to binary file stream
  virtual void b_write(vsl_b_ostream& bfs) const;

  //: Load class from binary file stream
  virtual void b_read(vsl_b_istream& bfs);
};

#endif // vpdfl_pc_gaussian_builder_h
