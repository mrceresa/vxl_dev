#ifndef vgui_glut_window_h_
#define vgui_glut_window_h_
// .NAME vgui_glut_window
// .LIBRARY vgui-glut
// .INCLUDE vgui/impl/glut/vgui_glut_window.h
// .FILE vgui_glut_window.cxx
// @author fsm@robots.ox.ac.uk

struct vgui_glut_adaptor;
#include <vgui/vgui_menu.h>
#include <vgui/vgui_window.h>

struct vgui_glut_window : public vgui_window
{
  vgui_glut_window(char const *title, unsigned w, unsigned h, int posx =-1, int posy=-1);
  ~vgui_glut_window();
  
  vgui_menu menubar;
  void set_menubar(vgui_menu const &m) { menubar = m; }

  vgui_glut_adaptor *glutwin;
  vgui_adaptor *get_adaptor();

  //
  void show();
  void hide();
  void iconify();
  void reshape(unsigned, unsigned);
  void reposition(int, int);
  void set_title(vcl_string const &);
  
private:
  bool pending_reshape;
  bool pending_reposition;
  int data0;
  int data1;
  
  friend struct vgui_glut_adaptor;
  void hello_from_adaptor();
};

#endif // vgui_glut_window_h_
