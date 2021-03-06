#ifndef msm_pose_maker_h_
#define msm_pose_maker_h_
//:
// \file
// \author Tim Cootes
// \brief Compute a direction at each point, usually normal to curve

#include <msm/msm_points.h>
#include <msm/msm_curve.h>
#include <vgl/vgl_vector_2d.h>
#include <vcl_iosfwd.h>

//: Compute a direction at each point, usually normal to curve.
//  Generates a unit direction vector for each point.
//  Typically these are defined as normals to the curve passing
//  through the point.
//
//  Define tangent at point i as line between end0_[i] and end1_[i].
//  If end0_[i]==end1_[i] then use vector (1,0) in ref. frame.
class msm_pose_maker
{
 private:
  //: Define tangent at point i as line between end0_[i] and end1_[i]
  vcl_vector<unsigned> end0_;

  //: Define tangent at point i as line between end0_[i] and end1_[i]
  vcl_vector<unsigned> end1_;

 public:
  ~msm_pose_maker() {}

  //: Set up definitions of directions from the curves.
  //  Where multiple curves pass through a point, the direction
  //  is defined by the first in the list.
  void set_from_curves(unsigned n_points, const msm_curves& curves);

  //: Returns true if direction defined at point i
  bool defined(unsigned i) { return end0_[i]!=0 || end1_[i]!=0; }

  //: Compute a direction at each point, usually normal to curve.
  void create_vectors(const msm_points& points,
                      vcl_vector<vgl_vector_2d<double> >& dir) const;

  //: Print class to os
  void print_summary(vcl_ostream& os) const;

  //: Save class to binary file stream
  void b_write(vsl_b_ostream& bfs) const;

  //: Load class from binary file stream
  void b_read(vsl_b_istream& bfs);
};

//: Binary file stream output operator for class reference
void vsl_b_write(vsl_b_ostream& bfs, const msm_pose_maker& b);

//: Binary file stream input operator for class reference
void vsl_b_read(vsl_b_istream& bfs, msm_pose_maker& b);

//: Stream output operator for class reference
vcl_ostream& operator<<(vcl_ostream& os,const msm_pose_maker& b);

#endif // msm_pose_maker_h_
