#ifndef vgui_soview2D_h_
#define vgui_soview2D_h_
#ifdef __GNUC__
#pragma interface
#endif
// 
// .NAME vgui_soview2D - Undocumented class FIXME
// .LIBRARY vgui
// .HEADER vxl Package
// .INCLUDE vgui/vgui_soview2D.h
// .FILE vgui_soview2D.cxx
//
// .SECTION Description
//
// vgui_soview2D is a class that Phil hasnt documented properly. FIXME
//
//
// Notes:
//   We use floats instead of doubles as size is a speed issue (sic.)
//
// .SECTION Author
//              Philip C. Pritchett, 24 Mar 99
//              Robotics Research Group, University of Oxford
//
// .SECTION Modifications
// 10 Feb 2000 fsm@robots - removed dependency on MultiView
// 04 Jul 2000 Marko Bacic - Fixed vgui_soview2D_circle
//
//-----------------------------------------------------------------------------

#include "vgui_soview.h"

class vgui_soview2D : public vgui_soview {
public:
  vgui_soview2D();
  virtual ~vgui_soview2D() {}

  virtual float distance_squared(float x, float y) = 0;
  virtual void get_centroid(float* x, float* y) = 0;
  virtual void translate(float x, float y) = 0;
};


class vgui_soview2D_point : public vgui_soview2D {
public:
  vgui_soview2D_point() : x(0), y(0) {}
  vgui_soview2D_point(float x_, float y_) : x(x_), y(y_) {}

  virtual void draw();
  virtual vcl_ostream& print(vcl_ostream&) const;
  virtual float distance_squared(float x, float y);
  vcl_string type_name() const { return "vgui_soview2D_point"; }
  void get_centroid(float* x, float* y);
  void translate(float x, float y);

  float x,y;
};


class vgui_soview2D_lineseg : public vgui_soview2D {
public:
  vgui_soview2D_lineseg() :
    x0(0), y0(0), x1(0), y1(0) {}
  vgui_soview2D_lineseg(float x0_, float y0_, float x1_, float y1_) :
    x0(x0_), y0(y0_), x1(x1_), y1(y1_) {}

  vgui_soview2D_lineseg( vgui_soview2D_lineseg &l_) { x0= l_.x0; y0= l_.y0; x1= l_.x1; y1= l_.y1; }

  virtual void draw();
  virtual vcl_ostream& print(vcl_ostream&) const;
  virtual float distance_squared(float x, float y);
  vcl_string type_name() const { return "vgui_soview2D_lineseg"; }
  void get_centroid(float* x, float* y);
  void translate(float x, float y);

  float x0, y0, x1, y1;
};


class vgui_soview2D_group : public vgui_soview2D {
public:
  vgui_soview2D_group() {}
  vgui_soview2D_group( vcl_vector<vgui_soview2D *> ls_) :
    ls( ls_) {}

  virtual void draw();
  virtual vcl_ostream& print(vcl_ostream&) const;
  virtual float distance_squared(float x, float y);
  vcl_string type_name() const { return "vgui_soview2D_group"; }
  void get_centroid(float* x, float* y);
  void translate(float x, float y);

  vcl_vector<vgui_soview2D *> ls;
};

class vgui_soview2D_infinite_line : public vgui_soview2D {
public:
  vgui_soview2D_infinite_line() {}
  vgui_soview2D_infinite_line( float a_, float b_, float c_) :
    a(a_), b(b_), c(c_) {}

  virtual void draw();
  virtual vcl_ostream& print(vcl_ostream&) const;
  virtual float distance_squared(float x, float y);
  vcl_string type_name() const { return "vgui_soview2D_infinite_line"; }

  void get_centroid(float* x, float* y);
  void translate(float x, float y);

  float a,b,c;
};


class vgui_soview2D_circle : public vgui_soview2D {
public:
  vgui_soview2D_circle() {}

  virtual void draw();
  virtual vcl_ostream& print(vcl_ostream&) const;
  virtual float distance_squared(float x, float y);
  vcl_string type_name() const { return "vgui_soview2D_circle"; }

  void get_centroid(float* x, float* y);
  void translate(float x, float y);

  float x,y,r;

  // compile the vcl_list
  static void compile();
};

class vgui_soview2D_linestrip : public vgui_soview2D {
public:
  vgui_soview2D_linestrip(unsigned, float const *, float const *);
  vgui_soview2D_linestrip() : n(0), x(0), y(0) {}
  ~vgui_soview2D_linestrip();
  
  virtual void draw();
  virtual vcl_ostream& print(vcl_ostream&) const;
  virtual float distance_squared(float x, float y);
  vcl_string type_name() const { return "vgui_soview2D_linestrip"; }

  void get_centroid(float* x, float* y);
  void translate(float x, float y);

  unsigned n;
  float *x, *y;
  void set_size(unsigned );

  //static void compile();
};

class vgui_soview2D_polygon : public vgui_soview2D {
public:
  vgui_soview2D_polygon(unsigned, float const *, float const *);
  vgui_soview2D_polygon() : n(0), x(0), y(0) {}
  ~vgui_soview2D_polygon();
  
  virtual void draw();
  virtual vcl_ostream& print(vcl_ostream&) const;
  virtual float distance_squared(float x, float y);
  vcl_string type_name() const { return "vgui_soview2D_polygon"; }

  void get_centroid(float* x, float* y);
  void translate(float x, float y);

  unsigned n;
  float *x, *y;
  void set_size(unsigned );

  //static void compile();
};

#endif // vgui_soview2D_h_
