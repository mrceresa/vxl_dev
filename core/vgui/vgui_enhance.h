// This is ./oxl/vgui/vgui_enhance.h

//:
// \file
// \author Philip C. Pritchett, Robotics Research Group, University of Oxford
// \date   16 Nov 99
// \brief  Magnify or display another tableau in a region around the mouse pointer.
//  Contains classes: vgui_enhance
//
// \verbatim
//  Modifications:
//    16-NOV-1999 P.Pritchett - Initial version.
//    26-APR-2002 K.Y.McGaul - Converted to and added doxygen style comments.
// \endverbatim

#ifndef vgui_enhance_h_
#define vgui_enhance_h_
#ifdef __GNUC__
#pragma interface
#endif

#include <vgui/vgui_tableau.h>
#include <vgui/vgui_slot.h>
#include <vgui/vgui_enhance_sptr.h>

class vgui_event;

//: Magnify or display another tableau in a region around the mouse pointer.
//  This happens when the user presses the left mouse button inside the 
//  rendering area.
//  Use '[' and ']' to change size of region around the mouse pointer.
//  Use '{' and '}' to change the magnification.
class vgui_enhance : public vgui_tableau {
public:
  //: Constructor - magnifies area under the mouse pointer.
  vgui_enhance();
  //: Constructor - magnifies area of the given tableau under the mouse pointer.
  vgui_enhance(vgui_tableau_sptr const& t);
  //: Constructor - displays t2 on top of t1 in a region around the mouse pointer.
  vgui_enhance(vgui_tableau_sptr const& t1, vgui_tableau_sptr const& t2);

  vcl_string file_name() const;
  vcl_string type_name() const;

  void set_enable_key_bindings(bool on) { enable_key_bindings = on; }
  void set_child(vgui_tableau_sptr const& t);

protected:
 ~vgui_enhance();
  bool handle(const vgui_event&);

  vgui_slot slot1;
  vgui_slot slot2;

  bool enhancing_;
  int x, y;
  int size;
  float zoom_factor;
  bool enable_key_bindings;
};

struct vgui_enhance_new : public vgui_enhance_sptr {
  vgui_enhance_new() :
    vgui_enhance_sptr(new vgui_enhance()) { }

  vgui_enhance_new(vgui_tableau_sptr const&t) :
    vgui_enhance_sptr(new vgui_enhance(t)) { }

  vgui_enhance_new(vgui_tableau_sptr const&t1, vgui_tableau_sptr const&t2) :
    vgui_enhance_sptr(new vgui_enhance(t1,t2)) { }
};

#endif // vgui_enhance_h_
