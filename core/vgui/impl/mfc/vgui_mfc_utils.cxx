//-*- c++ -*-------------------------------------------------------------------
#ifdef __GNUC__
#pragma implementation
#endif
//
// Class: vgui_mfc_util
// Author: Marko Bacic, Oxford RRG
// Created: 4 August 2000 
//
//-----------------------------------------------------------------------------
#include <vgui/impl/mfc/vgui_mfc_utils.h>
#include <vgui/vgui_command.h>
static bool debug = false;
vgui_mfc_utils *vgui_mfc_utils::instance_ = NULL;

// -- This menu dispatcher is being called within message service routine
// of vgui_mfc_mainfrm
void vgui_mfc_utils::menu_dispatcher(UINT nID)
{
	// -- Make sure nID is in the relevant range
	ASSERT(nID>=ID_MENU_ITEMS && nID<ID_MENU_ITEMS+item_count);
	// -- Call the callback function associated with the menu item
	callbacks[nID-ID_MENU_ITEMS]->execute();
}
vgui_mfc_utils *vgui_mfc_utils::instance()
{
	if(!instance_)
		instance_= new vgui_mfc_utils;
	return instance_;
}
vgui_mfc_utils::vgui_mfc_utils()
{
	item_count = 0;
}
HMENU vgui_mfc_utils::add_submenu(const vgui_menu& menu) 
{
CMenu *popdown_menu;

  // -- Create a new menu
  popdown_menu = new CMenu();
  popdown_menu->CreatePopupMenu();

  for (unsigned i=0;i<menu.size();i++) {

    if (menu[i].is_separator()) {
      if (debug) vcl_cerr << " <separator> " << vcl_endl;
	  popdown_menu->AppendMenu(MF_SEPARATOR);	
    }
    else if (menu[i].is_command()) {
      if (debug) vcl_cerr << " <command>" << vcl_endl;
	  popdown_menu->AppendMenu(MF_STRING,ID_MENU_ITEMS+item_count++,menu[i].name.c_str());
	  // -- Add to our callback list, the associated callback function pointer
	  vgui_command *cmnd = static_cast<vgui_command *>(menu[i].cmnd.as_pointer());
	  callbacks.push_back(cmnd);
	  if(menu[i].short_cut.mod!=vgui_MODIFIER_NULL || 
	     menu[i].short_cut.key!=vgui_KEY_NULL)
		{
	    // -- FIXME: Accelerators
		}
	}
    else if (menu[i].is_submenu()) {
      if (debug) vcl_cerr << " <submenu>" << vcl_endl;
	 popdown_menu->AppendMenu(MF_POPUP,(UINT)add_submenu(*menu[i].menu),menu[i].name.c_str());
    }
  }   
  return popdown_menu->GetSafeHmenu();
}

// -- Sets the menu of the application window
void vgui_mfc_utils::set_menu(const vgui_menu& menu) 
{
CMenu *menu_bar;

  // -- Get the application thread
  CWinApp *theapp = AfxGetApp();
  // -- Obtain the main window associated with it
  CWnd *window = theapp->GetMainWnd();
  // -- See whether the application window already has a menu associated with it
  if(window->GetMenu() == NULL)
  {
	  menu_bar = new CMenu();
	  menu_bar->CreateMenu();
	  window->SetMenu(menu_bar);
  }
  for (unsigned i=0;i<menu.size();i++) {
    
    if (menu[i].is_separator()) {
      if (debug) vcl_cerr << " <separator> " << vcl_endl;
	  menu_bar->AppendMenu(MF_SEPARATOR);	
    }
    else if (menu[i].is_command()) {
      if (debug) vcl_cerr << " <command> " << menu[i].name << vcl_endl;
      menu_bar->AppendMenu(MF_STRING|MF_ENABLED,ID_MENU_ITEMS+item_count++,
						   menu[i].name.c_str());
	  if(menu[i].short_cut.mod!=vgui_MODIFIER_NULL || 
	     menu[i].short_cut.key!=vgui_KEY_NULL)
		{
	    // -- FIXME: Accelerators
		}  
    }
    else if (menu[i].is_submenu()) {
      if (debug) vcl_cerr << " <submenu> " << menu[i].name << vcl_endl;
	  menu_bar->AppendMenu(MF_POPUP,(UINT)add_submenu(*menu[i].menu),menu[i].name.c_str());
    }
  }
  window->DrawMenuBar();
  window->UpdateWindow();
  window->ShowWindow(SW_SHOW);
}
// -- Create popup menu. Make sure to call delete after its use
// otherwise MLK!!!
CMenu *vgui_mfc_utils::set_popup_menu(const vgui_menu &menu)
{
CMenu *pop_up;

  // -- Create a new menu
  pop_up = new CMenu();
  pop_up->CreatePopupMenu();

  for (unsigned i=0;i<menu.size();i++) {

    if (menu[i].is_separator()) {
      if (debug) vcl_cerr << " <separator> " << vcl_endl;
	  pop_up->AppendMenu(MF_SEPARATOR);	
    }
    else if (menu[i].is_command()) {
      if (debug) vcl_cerr << " <command>" << vcl_endl;
	  pop_up->AppendMenu(MF_STRING,ID_MENU_ITEMS+item_count++,menu[i].name.c_str());
	  // -- Add to our callback list, the associated callback function pointer
	  vgui_command *cmnd = static_cast<vgui_command *>(menu[i].cmnd.as_pointer());
	  callbacks.push_back(cmnd);
	  if(menu[i].short_cut.mod!=vgui_MODIFIER_NULL || 
	     menu[i].short_cut.key!=vgui_KEY_NULL)
		{
	    // -- FIXME: Accelerators
		}
	}
    else if (menu[i].is_submenu()) {
      if (debug) vcl_cerr << " <submenu>" << vcl_endl;
	 pop_up->AppendMenu(MF_POPUP,(UINT)add_submenu(*menu[i].menu),menu[i].name.c_str());
    }
  }   
  return pop_up;
}
