#include <vgui/vgui.h>
#include <vgui/vgui_key.h>
#include <vgui/vgui_modifier.h>
#include <vgui/vgui_menu.h>
#include <vvid/vvid_live_video_manager.h>
#include <vvid/bin/videx_menus.h>

//Static munu callback functions

void videx_menus::quit_callback()
{
  vvid_live_video_manager::instance()->quit();
}


void videx_menus::set_camera_params_callback()
{
  vvid_live_video_manager::instance()->set_camera_params();
}

void videx_menus::start_live_video_callback()
{
  vvid_live_video_manager::instance()->start_live_video();
}

void videx_menus::stop_live_video_callback()
{
  vvid_live_video_manager::instance()->stop_live_video();
}

//videx_menus definition
vgui_menu videx_menus::get_menu()
{
  vgui_menu menubar;
  vgui_menu menufile;
  vgui_menu menuview;
  vgui_menu menuedit;
  //file menu entries
  menufile.add( "Quit", quit_callback,(vgui_key)'q', vgui_CTRL);

  //view menu entries
  menuview.add( "Start Live Video", start_live_video_callback);
  menuview.add( "Stop Live Video", stop_live_video_callback);

  //edit menu entries
  menuedit.add("Camera Settings", set_camera_params_callback);

  //Top level menu layout
  menubar.add( "File", menufile);
  menubar.add( "View", menuview);
  menubar.add( "Edit", menuedit);
  return menubar;
}

