//------------------------------------------------------------------------------
// FileName        : zhang_camera_node.h
// Author          : Kongbin Kang (kk@lems.brown.edu)
// Company         : Brown University
// Purpose         : Zhang Camera Node
// Date Of Creation: 3/23/2003
// Modification History :
// Date             Modifications
//------------------------------------------------------------------------------

#ifndef AFX_ZHANGCAMERA_H__EB787129_58FA_4195_A386_71D7CC0C9546__INCLUDED_
#define AFX_ZHANGCAMERA_H__EB787129_58FA_4195_A386_71D7CC0C9546__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "camera_node.h"
#include <vcl_vector.h>
#include <vgl/vgl_homg_point_2d.h>

class zhang_camera_node : public camera_node
{
 private:
   vcl_vector< vgl_homg_point_2d<double> > *pPointLists_;

 public: // constructor and deconstructor
   zhang_camera_node(int id=0);
   virtual ~zhang_camera_node();
 public:
  int removeData();

  // set get function
  vcl_vector< vgl_homg_point_2d<double> >& getPoints(int iView)
  {
    return pPointLists_[iView];
  }

  int readData(char* fname, int iView);  // for debugging

  virtual void set_beat(vcl_vector<double> const& new_beats);
};

#endif // AFX_ZHANGCAMERA_H__EB787129_58FA_4195_A386_71D7CC0C9546__INCLUDED_
