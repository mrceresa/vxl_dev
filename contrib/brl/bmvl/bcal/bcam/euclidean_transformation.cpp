// euclidean_transformation.cpp: implementation of the euclidean_transformation class.
//
//////////////////////////////////////////////////////////////////////

#include "euclidean_transformation.h"
#include <vgl/algo/vgl_h_matrix_3d.h>
#include <vcl_iostream.h>
#include <vcl_cassert.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

euclidean_transformation::euclidean_transformation()
{
}

euclidean_transformation::~euclidean_transformation()
{
}

void euclidean_transformation::set_beat(vcl_vector<double> const& new_beat)
{
  vcsl_spatial_transformation::set_beat(new_beat);

  // allocate new space for data
  this->remove();
  int nBeats = new_beat.size();
  trans_.resize(nBeats);
}

int euclidean_transformation::remove()
{
  trans_.clear();
  return 0;
}

bool euclidean_transformation::is_invertible(double time) const
{
  assert(valid_time(time));
  return true;
}

vnl_vector<double> euclidean_transformation::execute(const vnl_vector<double> &v, double tims) const
{
  // require not completed yet
  assert(is_valid());
  assert(v.size()==3);
  assert(v[2] == 1);

  return v;
}


vnl_vector<double> euclidean_transformation::inverse(const vnl_vector<double> &v, double time) const
{
  // not finished yet
  vcl_cerr<<"\n euclidean_transformation::inverse() ----- not finished yet\n";
  return v;
}

void euclidean_transformation::set_transformations(vcl_vector<vgl_h_matrix_3d<double> > &trans)
{
  trans_ = trans; 
}
