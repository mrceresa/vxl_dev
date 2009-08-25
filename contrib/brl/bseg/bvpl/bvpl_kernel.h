// This is//Projects/vxl/src/contrib/brl/bseg/bvpl/bvpl_kernel.h
#ifndef bvpl_kernel_h
#define bvpl_kernel_h

//:
// \file
// \brief A class to hold a kernel created by bvpl_kernel_factory
// \author Isabel Restrepo mir@lems.brown.edu
// \date  8/25/09
//
// \verbatim
//  Modifications
//   8/25/09 Moved out of bvpl_kernel_factory.h
// \endverbatim


#include <vcl_iostream.h>
#include <vcl_string.h>
#include <vnl/vnl_float_3.h>
#include <vbl/vbl_ref_count.h>
#include "bvpl_kernel_iterator.h"
#include <vgl/vgl_vector_3d.h>

//: A simple class to hold bvpl_kernel_iterator and its bounding cube
class bvpl_kernel: public vbl_ref_count
{
  public:
    //: Default constructor
    bvpl_kernel() {id_=++id_cnt;}
    //: Constructor
    bvpl_kernel(bvpl_kernel_iterator kernel, vgl_vector_3d<int> dim, vgl_point_3d<int> max_pt, vgl_point_3d<int> min_pt)
    : kernel_(kernel),dim_(dim),min_(min_pt),max_(max_pt) {id_=++id_cnt;}
    //: Destructor
    ~bvpl_kernel() {}
    bvpl_kernel_iterator iterator(){return kernel_;}
    vgl_vector_3d<int> dim()const {return dim_;}
    vgl_point_3d<int> min() const {return min_;}
    vgl_point_3d<int> max() const {return max_;}
    
    void print()
    {
      kernel_.begin();
      while (!kernel_.isDone()) {
        vgl_point_3d<int> coord =kernel_.index();
        float val= ((*kernel_).c_);
        
        vcl_cout.precision(2);
        vcl_cout << coord << "  " << val<< vcl_endl;
        ++kernel_;
      }
    }
    
    void print_to_file(vcl_string filename);
    
    bool save_raw(vcl_string filename);
    
    // Returns a sum of kernel values. Useful to check if they add up to zero
    float cum_sum()
    {
      float val = 0.0f;
      kernel_.begin();
      while (!kernel_.isDone()) {
        val += ((*kernel_).c_);
        ++kernel_;
      }
      vcl_cout << "Kernel sums to : " << val << vcl_endl;
      return val;
    }
    
    unsigned id(){return id_;}
    static unsigned id_cnt;
  
  private:
    bvpl_kernel_iterator kernel_;
    vgl_vector_3d<int> dim_;
    vgl_point_3d<int> min_;
    vgl_point_3d<int> max_;
    unsigned int id_;
  };

typedef vbl_smart_ptr<bvpl_kernel> bvpl_kernel_sptr;

//: A simple class to hold a vector of kernels
class bvpl_kernel_vector : public vbl_ref_count
{
  public:
    typedef vcl_vector< vcl_pair<vnl_float_3, bvpl_kernel_sptr > >::iterator iterator;
    //: Default constructor
    bvpl_kernel_vector() {}
    
    iterator begin() { return kernels_.begin(); }
    iterator end()   { return kernels_.end(); }
    //: vector of kernel and their corresponding orientation axis.
    // Note that the magnitude of the vector corresponds to the rotation angle around that axis
    vcl_vector< vcl_pair<vnl_float_3, bvpl_kernel_sptr > > kernels_;
};

typedef vbl_smart_ptr<bvpl_kernel_vector> bvpl_kernel_vector_sptr;

#endif