
// Load a vrml file, overlay 3d and view
// awf@robots.ox.ac.uk, Nov 2000

#include <vcl_cmath.h>
#include <vcl_fstream.h>

#include <vnl/vnl_double_3.h>
#include <vnl/vnl_double_3x3.h>
#include <vnl/algo/vnl_scatter_3x3.h>
#include <vnl/algo/vnl_symmetric_eigensystem.h>

#include <vil/vil_image.h>
#include <vil/vil_memory_image_of.h>
#include <vil/vil_load.h>
#include <vil/vil_rgb_byte.h>

#include <vgl/vgl_polygon_scan_iterator.h>
#include <vgl/vgl_point_2d.h>

// vgui basics:
#include <vgui/vgui.h> 
// tableaux:
#include <vgui/vgui_easy3D.h> 
#include <vgui/vgui_easy2D.h> 
#include <vgui/vgui_rubberbander.h> 
#include <vgui/vgui_viewer3D.h>
#include <vgui/vgui_viewer2D.h>
#include <vgui/vgui_composite.h>
#include <vgui/vgui_deck.h>
#include <vgui/vgui_text.h>
#include <vgui/vgui_image_tableau.h>
#include <vgui/vgui_image_blender.h>
#include <vgui/vgui_shell_tableau.h>
#include <vgui/vgui_projection_inspector.h>
#include <vgui/vrml/vgui_vrml_tableau.h>
// extras:
#include <vgui/vgui_menu.h>
#include <vgui/vgui_dialog.h>
#include <vgui/vgui_error_dialog.h>

#ifdef WIN32
# include <vgui/vgui_linker_hack.h>
# include <vgui/impl/mfc/vgui_mfc_app_init.h>
vgui_mfc_app_init theAppinit;
#else
# include <vgui/vgui_linker_hack.h>
#endif

vgui_deck_ref thedeck;
vcl_vector<vgui_vrml_tableau_ref> thevrmls;

vgui_tableau_ref make_3d_tableau(char const* filename)
{
  vgui_vrml_tableau_new vrml(filename, true);

  // Create a easy3D tableau: 
  vgui_easy3D_new list3D;

  list3D->set_point_radius(1); 
  // Add a point at the origin 
  list3D->set_foreground(1,1,1);
  list3D->add_point(0,0,0);

  // Add a line in the xaxis: 
  list3D->set_foreground(1,1,0); 
  list3D->add_line(1,0,0, 4,0,0); 
  list3D->add_point(5,0,0);

  // Add a line in the yaxis: 
  list3D->set_foreground(0,1,0); 
  list3D->add_line(0,1,0, 0,4,0); 
  list3D->add_point(0,5,0);

  // Add a line in the zaxis: 
  list3D->set_foreground(0,0,1); 
  list3D->add_line(0,0,1, 0,0,4); 
  list3D->add_point(0,0,5);
  list3D->set_point_radius(1); 

  // Composite the easy and vrml

  vgui_text_new text;
  text->add(0,0, filename);

  vgui_composite_new c(vrml, text);
  
  thevrmls.push_back(vrml);

  return c;
}

void load_vrml()
{
  vgui_dialog d("Load vrml");
  vcl_string filename = "/vrmls/ashmolean/vase1/ppm_cropped/s.000.ppm";
  vcl_string re = "*.*";
  d.inline_file("Filename", re, filename);
  if (!d.ask())
    return;
  vgui_tableau_ref tab3d = make_3d_tableau(filename.c_str());
  thedeck->add(tab3d);
}

bool set_style(int i)
{
  if (i == 0) {
    glPointSize(1);
    return true;
  }

  if (i == 1) {
    glPointSize(2);
    return true;
  }

  cerr << "unknown style " << i << "\n";
  return false;
}

void next_style()
{
  static int the_style = 0;
  if (!set_style(the_style++))
    the_style = 0;

  for(int i = 0; i < thevrmls.size(); ++i)
    thevrmls[i]->invalidate_vrml();
}


vgui_menu make_menubar()
{
  // Make submenus
  vgui_menu file;
  file.add("Load vrml", load_vrml, vgui_key_CTRL('A'));

  vgui_menu display;
  display.add("Change style", next_style, vgui_key_CTRL('t'));

  // Make bar
  vgui_menu bar;
  bar.add("File", file);
  bar.add("Display", display);
  return bar;
}


int main(int argc, char **argv)
{ 
  vgui::init(argc, argv); 

  vgui_deck_new deck;
  thedeck = deck;
  for(int i = 1; i < argc; ++i) {
    vgui_tableau_ref tab3d = make_3d_tableau(argv[i]);
    thedeck->add(tab3d);
  }

  vgui_viewer3D_new viewer(deck);

  vgui_shell_tableau_new shell(viewer);

  vgui_menu menu = make_menubar();

  next_style();

  int w = 512;
  int h = 512;

  return vgui::run(shell, w, h, menu, "vxl vrml viewer");
}
