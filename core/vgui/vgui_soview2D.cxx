//-*- c++ -*-------------------------------------------------------------------
#ifdef __GNUC__
#pragma implementation
#endif
//
// .NAME vgui_soview2D
// Author: Philip C. Pritchett, RRG, University of Oxford
// Created: 24 Mar 99
//
//-----------------------------------------------------------------------------

#include "vgui_soview2D.h"

#include <vcl_cmath.h>
#include <vcl_iostream.h>
#include <vcl_cmath.h>

#include <vgl/vgl_clip.h>
#include <vgl/vgl_distance.h>
#include <vnl/vnl_math.h>

#include <vgui/vgui_gl.h>
#include <vgui/vgui_style.h>
#include <vgui/vgui_projection_inspector.h>

vgui_soview2D::vgui_soview2D() {
}

//--------------------------------------------------------------------------//

vcl_ostream& vgui_soview2D_point::print(vcl_ostream& s) const {
  s << "[vgui_soview2D_point " << x << "," << y << " ";
  s << " "; return vgui_soview2D::print(s) << "]";
}

void vgui_soview2D_point::draw() {

  glPointSize(style->point_size);
  glBegin(GL_POINTS);
  glVertex2f(x,y);
  glEnd();
}

float vgui_soview2D_point::distance_squared(float x, float y) {
  float dx = this->x - x;
  float dy = this->y - y;
  return dx*dx + dy*dy;
}

void vgui_soview2D_point::get_centroid(float* x, float* y)
{
  *x = this->x;
  *y = this->y;
}

void vgui_soview2D_point::translate(float tx, float ty)
{
  x += tx;
  y += ty;
}

//--------------------------------------------------------------------------//

vcl_ostream& vgui_soview2D_lineseg::print(vcl_ostream& s) const {
  s << "[vgui_soview2D_lineseg " << x0 << "," << y0 << " -- " << x1 << "," << y1;
  s << " "; return vgui_soview2D::print(s) << "]";
}

void vgui_soview2D_lineseg::draw() {
  //vcl_cerr << "line id : " << id << vcl_endl;

  //glLineWidth(style->line_width);
  glBegin(GL_LINES);
  glVertex2f(x0,y0);
  glVertex2f(x1,y1);
  glEnd();
}

float vgui_soview2D_lineseg::distance_squared(float x, float y) {
  return vgl_distance2_to_linesegment(x0, y0, x1, y1, x, y);
}

void vgui_soview2D_lineseg::get_centroid(float* x, float* y)
{
  *x = (x0 + x1) / 2;
  *y = (y0 + y1) / 2;
}

void vgui_soview2D_lineseg::translate(float tx, float ty)
{
  x0 += tx;
  y0 += ty;
  x1 += tx;
  y1 += ty;
}

//--------------------------------------------------------------------------//

void vgui_soview2D_group::set_style(vgui_style *s)
{
  for( unsigned int i=0; i< ls.size(); i++)
    if( !ls[i]->get_style())
      ls[i]->set_style(s);

  vgui_soview::set_style( s);
}

vcl_ostream& vgui_soview2D_group::print(vcl_ostream& s) const {
  s << "[vgui_soview2D_group ";

  for( unsigned int i=0; i< ls.size(); i++)
    ls[i]->print(s);

  return vgui_soview2D::print(s) << "]";
}

void vgui_soview2D_group::draw() {
  for( unsigned int i=0; i< ls.size(); i++)
    ls[i]->draw();
}

float vgui_soview2D_group::distance_squared(float x, float y) {
  float min= -1;

  for( unsigned int i=0; i< ls.size(); i++) {
    float d= ls[i]->distance_squared( x, y);

    if(( min== -1) || ( d< min))
      min= d;
  }

  return min;
}

void vgui_soview2D_group::get_centroid(float* x, float* y)
{
  *x = 0;
  *y = 0;
  int n = ls.size();

  for( int i=0; i < n; i++) {
    float cx, cy;
    ls[i]->get_centroid(&cx, &cy);
    *x += cx;
    *y += cy;
  }

  float s = 1.0f/float(n);
  *x *= s;
  *y *= s;
}

void vgui_soview2D_group::translate(float tx, float ty)
{
  int n = ls.size();

  for( unsigned int i=0; i < n; i++)
    ls[i]->translate(tx, ty);
}

//--------------------------------------------------------------------------//

vcl_ostream& vgui_soview2D_infinite_line::print(vcl_ostream& s) const {
  s << "[vgui_soview2D_infinite_line " << a << "," << b << "," << c;
  s << " "; return vgui_soview2D::print(s) << "]";
}


extern bool vgui_draw_line(double a, double b, double c);

void vgui_soview2D_infinite_line::draw() {
#if 0
  // compute the bounding box
  // this will be the viewport but in the coords of
  // the image

  vgui_projection_inspector pi;
  float x0, y0, x1, y1;
  pi.image_viewport(x0,y0,x1,y1);

  double x3, y3;
  double x4, y4;
  bool f = vgl_clip_line_to_box(a,b,c,
                                x0, y0, x1, y1,
                                x3, y3, x4,y4);
  if (!f)
    return; // no intersection

  //glLineWidth(style->line_width);
  glBegin(GL_LINES);
  glVertex2f(x3,y3);
  glVertex2f(x4,y4);
  glEnd();
#else
  vgui_draw_line(a, b, c);
#endif
}


float vgui_soview2D_infinite_line::distance_squared(float x, float y) {
  float tmp = a*x + b*y + c;
  return tmp*tmp/(a*a + b*b);
}

void vgui_soview2D_infinite_line::get_centroid(float* x, float* y)
{
  *x = 0;
  *y = 0;
}

void vgui_soview2D_infinite_line::translate(float tx, float ty)
{
  c += a * tx + b * ty;
}

//--------------------------------------------------------------------------//

const int vgui__CIRCLE2D_LIST = 1;

void vgui_soview2D_circle::compile() {
  glNewList(vgui__CIRCLE2D_LIST, GL_COMPILE);
  glBegin(GL_LINE_LOOP); // <--- should be GL_LINE_LOOP?
  for(int i=0;i<100;i++){
    double angle = i*(2*vnl_math::pi/100);
    glVertex2d(vcl_cos(angle), vcl_sin(angle));
  }
  glEnd();
  glEndList();
}


vcl_ostream& vgui_soview2D_circle::print(vcl_ostream& s) const {
  s << "[vgui_soview2D_circle " << x << "," << y << " r" << r;
  s << " "; return vgui_soview2D::print(s) << "]";
}

void vgui_soview2D_circle::draw() {
  glBegin(GL_LINE_LOOP);
  for(int i=0;i<100;i++){
    double angle = i*(2*vnl_math::pi/100);
    glVertex2d(x+r*vcl_cos(angle), y+r*vcl_sin(angle));
  }
  glEnd();
}

float vgui_soview2D_circle::distance_squared(float x, float y) {
  float dx = this->x - x;
  float dy = this->y - y;

  // distance from point to centre
  float dcentre = vcl_sqrt(dx*dx + dy*dy);

  // signed distance from point to circumference
  float dcircum = dcentre - this->r;

  return dcircum * dcircum;
}

void vgui_soview2D_circle::get_centroid(float* x, float* y)
{
  *x = this->x;
  *y = this->y;
}

void vgui_soview2D_circle::translate(float tx, float ty)
{
  x += tx;
  y += ty;
}

//--------------------------------------------------------------------------------

vgui_soview2D_linestrip::vgui_soview2D_linestrip(unsigned n_, float const *x_, float const *y_)
  : n(n_), x(new float[n]), y(new float[n])
{
  for (unsigned i=0; i<n; ++i) {
    x[i] = x_[i];
    y[i] = y_[i];
  }
}

vgui_soview2D_linestrip::~vgui_soview2D_linestrip() {
  n=0;
  delete [] x; x=0;
  delete [] y; y=0;
}

void vgui_soview2D_linestrip::draw() {
  glBegin(GL_LINE_STRIP);
  for (unsigned i=0; i<n; ++i)
    glVertex2f(x[i], y[i]);
  glEnd();
}

vcl_ostream& vgui_soview2D_linestrip::print(vcl_ostream&s) const { return s << "[a linestrip. FIXME]"; }

float vgui_soview2D_linestrip::distance_squared(float x, float y) {
  double tmp = vgl_distance_to_non_closed_polygon(this->x, this->y, this->n,
                                                  x, y);
  return tmp*tmp;
}

void vgui_soview2D_linestrip::get_centroid(float* x, float* y)
{
  *x = 0;
  *y = 0;
  for (unsigned i=0; i<n; ++i) {
    *x += this->x[i];
    *y += this->y[i];
  }
  float s = 1.0f / float(n);
  *x *= s;
  *y *= s;
}

void vgui_soview2D_linestrip::translate(float tx, float ty)
{
  for (unsigned i=0; i<n; ++i) {
    x[i] += tx;
    y[i] += ty;
  }
}

void vgui_soview2D_linestrip::set_size(unsigned nn) {
  if (nn < n) {
    n = nn;
    return;
  }

  // we know that n <= nn
  float *nx = new float[nn];
  float *ny = new float[nn];
  for (unsigned i=0; i<n; ++i) {
    nx[i] = x[i];
    ny[i] = y[i];
  }

  n = nn;
  delete [] x; x = nx;
  delete [] y; y = ny;
}

//--------------------------------------------------------------------------------

vgui_soview2D_polygon::vgui_soview2D_polygon(unsigned n_, float const *x_, float const *y_)
  : n(n_), x(new float[n]), y(new float[n])
{
  for (unsigned i=0; i<n; ++i) {
    x[i] = x_[i];
    y[i] = y_[i];
  }
}

vgui_soview2D_polygon::~vgui_soview2D_polygon() {
  n=0;
  delete [] x; x=0;
  delete [] y; y=0;
}

void vgui_soview2D_polygon::draw() {
  glBegin(GL_LINE_LOOP);
  for (unsigned i=0; i<n; ++i)
    glVertex2f(x[i], y[i]);
  glEnd();
}

vcl_ostream& vgui_soview2D_polygon::print(vcl_ostream&s) const { return s << "[a polygon. FIXME]"; }

float vgui_soview2D_polygon::distance_squared(float x, float y) {
  double tmp = vgl_distance_to_closed_polygon(this->x, this->y, this->n,
                                              x, y);
  return tmp*tmp;
}

void vgui_soview2D_polygon::get_centroid(float* x, float* y)
{
  *x = 0;
  *y = 0;
  for (unsigned i=0; i<n; ++i) {
    *x += this->x[i];
    *y += this->y[i];
  }
  float s = 1.0f / float(n);
  *x *= s;
  *y *= s;
}

void vgui_soview2D_polygon::translate(float tx, float ty)
{
  for (unsigned i=0; i<n; ++i) {
    x[i] += tx;
    y[i] += ty;
  }
}

void vgui_soview2D_polygon::set_size(unsigned nn) {
  if (nn < n) {
    n = nn;
    return;
  }

  // we know that n <= nn
  float *nx = new float[nn];
  float *ny = new float[nn];
  for (unsigned i=0; i<n; ++i) {
    nx[i] = x[i];
    ny[i] = y[i];
  }

  n = nn;
  delete [] x; x = nx;
  delete [] y; y = ny;
}
