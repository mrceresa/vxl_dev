//-*- c++ -*-------------------------------------------------------------------
#ifdef __GNUC__
#pragma implementation
#endif
//
// .NAME vgui_active_visible
// Author: Philip C. Pritchett, RRG, University of Oxford
// Created: 25 Jan 00
//
//-----------------------------------------------------------------------------

#include "vgui_active_visible.h"

#include "vgui_command.h"
#include "vgui_menu.h"

vgui_active_visible::vgui_active_visible(vgui_tableau_sptr const & t)
  : vgui_wrapper_tableau(t)
  , active_(true)
  , visible_(true)
{
}

vgui_active_visible::~vgui_active_visible()
{
}

vcl_string vgui_active_visible::type_name() const {return "vgui_active_visible";}


bool vgui_active_visible::handle(const vgui_event &e) {
  // if tableau is not visible then do nothing with vgui_DRAW, vgui_CLEAR, vgui_DRAW_OVERLAY
  if (!visible_)
    if (e.type == vgui_DRAW || e.type == vgui_DRAW_OVERLAY)
      return false;

  if (!active_)
    if (e.type != vgui_DRAW &&e.type != vgui_DRAW_OVERLAY)
      return false;

  return vgui_wrapper_tableau::handle(e);

}


void vgui_active_visible::add_popup(vgui_menu& menu) {

  vcl_string active_label("Toggle active ");
  if (active_) active_label+="[on]";
  else active_label+="[off]";

  menu.add(active_label.c_str(),
           new vgui_command_simple<vgui_active_visible>(this, &vgui_active_visible::toggle_active));


  vcl_string visible_label("Toggle visible ");
  if (visible_) visible_label+="[on]";
  else visible_label+="[off]";

  menu.add(visible_label.c_str(),
           new vgui_command_simple<vgui_active_visible>(this, &vgui_active_visible::toggle_visible));

}


void vgui_active_visible::toggle_active() {
  active_ = !active_;
  post_redraw();
}

void vgui_active_visible::toggle_visible() {
  visible_ = !visible_;
  post_redraw();
}

