// This is oxl/vgui/vgui_debug_tableau.h

//:
// \file
// \author awf
// \date   08-11-99
// \brief  Event-printing "tee" tableau.

#ifndef vgui_debug_tableau_h_
#define vgui_debug_tableau_h_
#ifdef __GNUC__
#pragma interface
#endif

#include "vgui_debug_tableau_sptr.h"
#include <vgui/vgui_wrapper_tableau.h>

//: Event-printing "tee" tableau.
//  A vgui_tableau that prints events and then forwards them to
//  a child.
//
// \verbatim
//  Modifications:
//    991008 AWF Initial version.
//    26-APR-2002 K.Y.McGaul - Converted to doxygen style comments.
// \endverbatim
class vgui_debug_tableau : public vgui_wrapper_tableau {
public:
  // Public interface at the top
  vgui_debug_tableau(vgui_tableau_sptr const& child);

  vcl_string type_name() const { return "debug_tableau"; }

  // Protected members and data
protected:
  ~vgui_debug_tableau();
  bool handle(const vgui_event& e);
  int verbosity;
};

struct vgui_debug_tableau_new : public vgui_debug_tableau_sptr {
  typedef vgui_debug_tableau_sptr base;
  vgui_debug_tableau_new(vgui_tableau_sptr const& child) : base(new vgui_debug_tableau(child)) { }
};

#endif // vgui_debug_tableau_h_
