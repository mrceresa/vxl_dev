//--------------------------------------------------------------
//
// Class : HMatrix1DCompute
//
// .SECTION Description:
//
// Base class of classes to generate a line-to-line projectivity matrix from
// a set of matched points.
//
// .NAME HMatrix1DCompute
// .LIBRARY MViewCompute
// .HEADER MultiView package
// .INCLUDE mvl/HMatrix1DCompute.h
// .FILE HMatrix1DCompute.C
//
// .SECTION Author
//  Frederik Schaffalitzky , Robotic Research Group
//

#ifndef _HMatrix1DCompute_h
#define _HMatrix1DCompute_h

class HMatrix1D;
#include <mvl/HomgPoint1D.h>
#include <vcl/vcl_vector.h>

// these make the headers easier to read (in my opinion) - fsm

//
// definition of class :
//
class HMatrix1DCompute {
 private:
  
 protected:
  bool verbose_;

  virtual bool compute_cool_homg(const vcl_vector<HomgPoint1D> &,
				 const vcl_vector<HomgPoint1D> &,
				 HMatrix1D *);
  virtual bool compute_array_dbl(const double [],
				 const double [],
				 int N,
				 HMatrix1D *);
 public:
  //
  HMatrix1DCompute() : verbose_(false) { }
  virtual ~HMatrix1DCompute() { }

  // 
  void verbose(bool);	// set this to true for verbose run-time information

  // 
  // Compute methods :
  //
  bool compute(const vcl_vector<HomgPoint1D>&,
	       const vcl_vector<HomgPoint1D>&, 
	       HMatrix1D *);
  bool compute(const double [],
	       const double [],
	       int,
	       HMatrix1D *);
};

#endif


