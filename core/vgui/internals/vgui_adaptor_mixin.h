// This is oxl/vgui/internals/vgui_adaptor_mixin.h

//:
// \file
// \author fsm@robots.ox.ac.uk
// \brief  Holds the dimensions of the adaptor and the event causing popup menus.

#ifndef vgui_adaptor_mixin_h_
#define vgui_adaptor_mixin_h_
#ifdef __GNUC__
#pragma interface
#endif

#include <vgui/vgui_event.h>

//: Holds the dimensions of the adaptor and the event causing popup menus.
class vgui_adaptor_mixin {
public:
  vgui_adaptor_mixin();
  ~vgui_adaptor_mixin();

  //: Cached width of adaptor render area.
  unsigned width;
  //: Cached height of adaptor render area.
  unsigned height;

  // These specify the event that causes the popup menu to appear
  //: Mouse button (used with popup_modifier) which causes the popup menu to appear.
  vgui_button popup_button;
  //: Modifier (used with popup_button) which causes the popup menu to appear.
  vgui_modifier popup_modifier;
};

#endif // vgui_adaptor_mixin_h_
