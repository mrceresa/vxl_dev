#ifndef vsl_edgel_chain_h_
#define vsl_edgel_chain_h_
#ifdef __GNUC__
#pragma interface
#endif
// .NAME vsl_edgel_chain
// .INCLUDE vsl/vsl_edgel_chain.h
// .FILE vsl_edgel_chain.cxx
// \author fsm@robots.ox.ac.uk

#include <vcl_iosfwd.h>

class vsl_edgel_chain {
public:
  int n;
  float *x, *y, *grad, *theta;
  
  vsl_edgel_chain();
  vsl_edgel_chain(int n_);
  vsl_edgel_chain(vsl_edgel_chain const &);
  void operator=(vsl_edgel_chain const &);
  ~vsl_edgel_chain();
  void SetLength(int);
  
  float  GetGrad(int i) const;
  float *GetGrad() const;
  float  GetTheta(int i) const;
  float *GetTheta() const;
  float  GetX(int i) const;
  float *GetX() const;
  float  GetY(int i) const;
  float *GetY() const;
  void SetGrad(float v, int i);
  void SetTheta(float v, int i);
  void SetX(float v, int i);
  void SetY(float v, int i);
  int size() const;
  //
  void write_ascii(vcl_ostream &) const;
  void read_ascii(vcl_istream &);
};

// urgh
typedef vsl_edgel_chain vsl_EdgelChain;

#endif // vsl_edgel_chain_h_
