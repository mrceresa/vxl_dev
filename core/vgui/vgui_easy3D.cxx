//-*- c++ -*-------------------------------------------------------------------
#ifdef __GNUC__
#pragma implementation
#endif
//
// Class: vgui_easy3D
// Author: Philip C. Pritchett, RRG, University of Oxford
// Created: 24 Sep 99
//
//-----------------------------------------------------------------------------

#include "vgui_easy3D.h"

#include "vgui_soview3D.h"
#include "vgui_displaylist3D.h"
#include "vgui_style_factory.h"

vgui_easy3D::vgui_easy3D() {
  fg[0] = 0.0f;
  fg[1] = 1.0f;
  fg[2] = 0.0f;
  
  line_width = 1;
  point_size = 3;
}


void vgui_easy3D::add(vgui_soview3D* object) {

  vgui_style *style = vgui_style_factory::instance()->get_style(fg[0], fg[1], fg[2],
								point_size, line_width);
  
  object->set_style(style);
  vgui_displaylist3D::add(object);
}


vgui_point3D* vgui_easy3D::add_point(float x, float y, float z)
{
  vgui_point3D *obj = new vgui_point3D;

  obj->x = x;
  obj->y = y;
  obj->z = z;

  add(obj);
  return obj;
}


vgui_lineseg3D* vgui_easy3D::add_line(float x0, float y0, float z0,
				      float x1, float y1, float z1) {
  vgui_lineseg3D *obj = new vgui_lineseg3D;
  
  obj->x0 = x0;
  obj->y0 = y0;
  obj->z0 = z0;
  obj->x1 = x1;
  obj->y1 = y1;
  obj->z1 = z1;

  add(obj);
  return obj;
}








void vgui_easy3D::set_foreground(float r, float g, float b)
{
  fg[0] = r;
  fg[1] = g;
  fg[2] = b;
}

void vgui_easy3D::set_line_width(float w)
{
  line_width = w;
}

void vgui_easy3D::set_point_radius(float r)
{
  point_size = r;
}

