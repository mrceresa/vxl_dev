#include <vgui/vgui.h>
#include <vgui/vgui_key.h>
#include <vgui/vgui_modifier.h>
#include <vgui/vgui_menu.h>
#include <segv/segv_segmentation_manager.h>
#include <segv/segv_menus.h>

//Static munu callback functions

void segv_menus::quit_callback()
{
  segv_segmentation_manager::instance()->quit();
}


void segv_menus::load_image_callback()
{
  segv_segmentation_manager::instance()->load_image();
}

void segv_menus::clear_display_callback()
{
  segv_segmentation_manager::instance()->clear_display();
}

void segv_menus::original_image_callback()
{
  segv_segmentation_manager::instance()->original_image();
}

void segv_menus::roi_callback()
{
  segv_segmentation_manager::instance()->roi();
}

void segv_menus::gaussian_callback()
{
  segv_segmentation_manager::instance()->gaussian();
}

void segv_menus::downsample_callback()
{
  segv_segmentation_manager::instance()->downsample();
}

void segv_menus::convolution_callback()
{
  segv_segmentation_manager::instance()->convolution();
}

void segv_menus::harris_measure_callback()
{
  segv_segmentation_manager::instance()->harris_measure();
}

void segv_menus::beaudet_measure_callback()
{
  segv_segmentation_manager::instance()->beaudet_measure();
}

void segv_menus::vd_edges_callback()
{
  segv_segmentation_manager::instance()->vd_edges();
}

void segv_menus::regions_callback()
{
  segv_segmentation_manager::instance()->regions();
}

void segv_menus::fit_lines_callback()
{
  segv_segmentation_manager::instance()->fit_lines();
}

void segv_menus::test_camera_parms_callback()
{
  segv_segmentation_manager::instance()->test_camera_parms();
}
#if 0
#ifdef HAS_XERCES
void segv_menus::read_xml_edges_callback()
{
  segv_segmentation_manager::instance()->read_xml_edges();
}
#endif
#endif

void segv_menus::test_face_callback()
{
segv_segmentation_manager::instance()->test_face();
}

void segv_menus::test_digital_lines_callback()
{
segv_segmentation_manager::instance()->test_digital_lines();
}

void segv_menus::display_IHS_callback()
{
segv_segmentation_manager::instance()->display_IHS();
}

//segv_menus definition
vgui_menu segv_menus::get_menu()
{
  vgui_menu menubar;
  vgui_menu menufile;
  vgui_menu menuview;
  vgui_menu menuedit;
  //file menu entries
  menufile.add( "Quit", quit_callback,(vgui_key)'q', vgui_CTRL);
  menufile.add( "Load Image", load_image_callback,(vgui_key)'l', vgui_CTRL);
#if 0
#ifdef HAS_XERCES
  menufile.add( "Load XML Edges", read_xml_edges_callback);
#endif
#endif
  //view menu entries
  menuview.add("Original Image", original_image_callback);
  menuview.add("Clear Display", clear_display_callback);
  menuview.add("Display IHS", display_IHS_callback);
  //edit menu entries
  menuedit.add("Region Of Interest", roi_callback);
  menuedit.add("Gaussian", gaussian_callback);
  menuedit.add("Downsample", downsample_callback);
  menuedit.add("Convolution", convolution_callback);
  menuedit.add("Harris", harris_measure_callback);
  menuedit.add("Beaudet", beaudet_measure_callback);
  menuedit.add("VD Edges", vd_edges_callback);
  menuedit.add("Regions", regions_callback);
  menuedit.add("Line Segments", fit_lines_callback);
  menuedit.add("TestFace", test_face_callback);
  menuedit.add("TestDigitalCurves", test_digital_lines_callback);
  menuedit.add("Test Camera Parms",test_camera_parms_callback);
  //Top level menu layout
  menubar.add( "File", menufile);
  menubar.add( "View", menuview);
  menubar.add( "Edit", menuedit);
  return menubar;
}

