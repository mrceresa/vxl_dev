//-*- c++ -*-------------------------------------------------------------------
#ifdef __GNUC__
#pragma implementation
#endif
//
// .NAME vgui_text_tableau
// Author: Philip C. Pritchett, RRG, University of Oxford
// Created: 19 Oct 99
//   
//-----------------------------------------------------------------------------

#include "vgui_text_tableau.h"
#include <vgui/vgui_text.h>
#include <vgui/vgui_event.h>
#include <vgui/vgui_gl.h>

vgui_text_tableau::vgui_text_tableau() : first_empty(0) { }

vcl_string vgui_text_tableau::type_name() const { return "vgui_text_tableau"; }

unsigned vgui_text_tableau::size() const {
  unsigned N=xs.size();
//assert(N == ys.size()); // just to
//assert(N == ts.size()); // make sure.
  return N;
}

void vgui_text_tableau::clear() {
  if (size() > 0) {
    xs.clear();
    ys.clear();
    ts.clear();
    post_redraw();
    first_empty = 0;
  }
}

int vgui_text_tableau::add(float x, float y, char const *text) {
  int return_val = first_empty;
  if (first_empty < size()) {
    xs[first_empty] = x;
    ys[first_empty] = y;
    ts[first_empty] = text;
    // Find next empty slot:
    while (first_empty < size()  &&  xs[first_empty] != -1)
      first_empty++;
  }
  else {
    xs.push_back(x);
    ys.push_back(y);
    ts.push_back(text);
    first_empty++;
  }
  post_redraw();
  return return_val;
}

void vgui_text_tableau::move(int handle, float nx, float ny) {
  if (handle >= size()) handle = size()-1;
  if (handle < 0) handle = 0;
  xs[handle] = nx;
  ys[handle] = ny;
  post_redraw();
}

float vgui_text_tableau::get_posx(int handle) const {
  if (handle >= size()) handle = size()-1;
  if (handle < 0) handle = 0;
  return xs[handle];
}

float vgui_text_tableau::get_posy(int handle) const {
  if (handle >= size()) handle = size()-1;
  if (handle < 0) handle = 0;
  return ys[handle];
}

vcl_string const &vgui_text_tableau::get_text(int handle) const {
  if (handle >= size()) handle = size()-1;
  if (handle < 0) handle = 0;
  return ts[handle];
}

void vgui_text_tableau::remove(int handle) {
  if (handle >= size()) handle = size()-1;
  if (handle < 0) handle = 0;
  xs[handle] = -1;

  // KYM - don't do this because it changes handles of values remaining in list:
  //xs.erase(xs.begin() + handle);
  //ys.erase(ys.begin() + handle);
  //ts.erase(ts.begin() + handle);

  post_redraw();
  if (handle < first_empty)
    first_empty = handle;
}

void vgui_text_tableau::change(int handle, char const *ntext) {
  if (handle >= size()) handle = size()-1;
  if (handle < 0) handle = 0;
  ts[handle] = ntext;
  post_redraw();
}

bool vgui_text_tableau::handle(vgui_event const &e) {
  if (e.type != vgui_DRAW)
    return false;

  // set OpenGL state suitable for rendering 2D bitmap fonts :
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_LIGHTING);
  glShadeModel(GL_FLAT);
  
  glColor3f(1,0,0); // FIXME

  for (unsigned i=0; i<size(); ++i) {
    if (xs[i] != -1) {
      glRasterPos2f(xs[i], ys[i]);
      ::vgui_text_put(ts[i].c_str());
    }
  }
  return true;
}
