//------------------------------------------------------------------------------
// FileName             : zhang_linear_calibrate.h
// Author               : Kongbin Kang (kk@lems.brown.edu)
// Company              : Brown University
// Purpose              : zhang linear camera calibration from planary pattern
// Date Of Creation     : 3/23/2003
// Modification History :
// Date             Modifications
//------------------------------------------------------------------------------


#ifndef AFX_ZHANGLINEARCALIBRATE_H__A6F24F23_8E39_494F_83FB_3ABF8093481D__INCLUDED_
#define AFX_ZHANGLINEARCALIBRATE_H__A6F24F23_8E39_494F_83FB_3ABF8093481D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vcsl/vcsl_graph_sptr.h>
#include <vnl/vnl_vector_fixed.h>
#include <vnl/vnl_double_3x3.h>
#include <vgl/vgl_homg_point_2d.h>
#include <vgl/algo/vgl_h_matrix_2d.h>
#include "camera_graph.h"
#include "calibrate_plane.h"
#include "zhang_camera_node.h"
#include "euclidean_transformation.h"

class zhang_linear_calibrate
{
public:
  // compute extrinsic parameters of camera
  vgl_h_matrix_3d<double> compute_extrinsic(\
  vgl_h_matrix_2d<double> const &H, vnl_double_3x3 const &A);

  // calibrate the camera array
  int calibrate();

  // set camera graph
  void setCameraGraph(camera_graph<calibrate_plane, zhang_camera_node, euclidean_transformation>* pG);
  zhang_linear_calibrate();
  virtual ~zhang_linear_calibrate();
  
protected:
	int calibrate_extrinsic();
	void calibrate_intrinsic();
  int compute_homography();
  vnl_vector_fixed<double, 6> homg_constrain(vgl_h_matrix_2d<double> const& hm, int i, int j);
  // homographies list hm_list[0...n-1]
  vnl_double_3x3 compute_intrinsic(vgl_h_matrix_2d<double> *hm_list, int n);
  
  // get closet rotation matrix from a none rotation matrix Q
	vnl_double_3x3 get_closest_rotation(vnl_double_3x3 const & Q);

  // allocate memory to store homography according to camera graph
  int initialize();
  // clear allocated memory.
  int clear();

private:
  camera_graph<calibrate_plane, zhang_camera_node, euclidean_transformation>* camGraph_;
  vcl_vector<vgl_h_matrix_2d<double> *> h_matrice_;
  vcl_vector<int> num_views_;
};

#endif // AFX_ZHANGLINEARCALIBRATE_H__A6F24F23_8E39_494F_83FB_3ABF8093481D__INCLUDED_




