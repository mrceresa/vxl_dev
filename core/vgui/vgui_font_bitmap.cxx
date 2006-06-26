// This is core/vgui/vgui_font_bitmap.cxx
//=========================================================================
//:
// \file
// \brief  vgui_font derived class that uses hardcoded fonts.
//
// See vgui_font_bitmap.h for details.
//=========================================================================

#include <vgui/vgui_font_bitmap.h>
#include <vgui/vgui_macro.h>

#include <vcl_iostream.h>

//-------------------------------------------------------------------------
// Supported fonts - declarations (definitions below).
//-------------------------------------------------------------------------
namespace
{
  extern const GLubyte bitmaps8x13[][13];
}

//----------------------------------------------------------------------------
// vgui_font_bitmap implementation
//----------------------------------------------------------------------------
//: Constructor - default.
vgui_font_bitmap::vgui_font_bitmap(bitmap_font_type t)
  : display_list_base_id_(0)
{
  vgui_macro_report_errors;

  // build the display list for each symbol
  if ((display_list_base_id_ = glGenLists(256)) == 0)
  {
#ifdef DEBUG
    vcl_cerr << __FILE__ ":Failed to create display lists!" << vcl_endl;
#endif
    return;
  }

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  switch(t)
  {
  case BITMAP_FONT_8_13:
    for (unsigned int i = 0; i < 256; i++)
    {
      glNewList(display_list_base_id_ + i, GL_COMPILE);
      glBitmap(8, 13, 0.f, 2.f, 10.f, 0.f, bitmaps8x13[i]);
      glEndList();
    }
    break;

#ifdef DEBUG
  default:
    vcl_cerr << __FILE__ ":Invalid bitmap_font_type!" << vcl_endl;
#endif
  }

  vgui_macro_report_errors;
}

//: Destructor.
vgui_font_bitmap::~vgui_font_bitmap(void)
{
  if (display_list_base_id_) { glDeleteLists(display_list_base_id_, 256); }
}

//: Draw a string of font symbols.
void vgui_font_bitmap::draw(const vcl_string& str) const
{
  for (unsigned int i = 0; i < str.size(); ++i)
  {
    glBitmap(8, 13, 0.f, 2.f, 10.f, 0.f, bitmaps8x13[str[i]]);
  }

#if 0 // display lists not working
  if (!display_list_base_id_) { return; }

  glPushAttrib(GL_LIST_BIT);
  glListBase(display_list_base_id_);
  glCallLists(str.size(), GL_UNSIGNED_BYTE, str.c_str());
  glPopAttrib();
#endif
}

//-------------------------------------------------------------------------
// Supported fonts - definitions.
//-------------------------------------------------------------------------
namespace
{
//-------------------------------------------------------------------------
// This was generated from the output of calling showfont on the 8x13 font
// that comes with X.  It had this copyright message:
//   COPYRIGHT       Public domain font.  Share and enjoy.
//-------------------------------------------------------------------------
const GLubyte bitmaps8x13[][13] = {
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x10, 0x38, 0x7c, 0xfe, 0x7c, 0x38, 0x10, 0x00, 0x00, 0x00},
   {0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x00},
   {0x08, 0x08, 0x08, 0x3e, 0x88, 0x88, 0xf8, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00},
   {0x10, 0x10, 0x1c, 0x10, 0x9e, 0x80, 0xe0, 0x80, 0xf0, 0x00, 0x00, 0x00, 0x00},
   {0x22, 0x22, 0x3c, 0x22, 0x3c, 0x78, 0x80, 0x80, 0x78, 0x00, 0x00, 0x00, 0x00},
   {0x20, 0x20, 0x3c, 0x20, 0x3e, 0xf8, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x44, 0x44, 0x38, 0x00, 0x00},
   {0x00, 0x00, 0xfe, 0x10, 0x10, 0xfe, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x3e, 0x20, 0x20, 0x20, 0x88, 0x98, 0xa8, 0xc8, 0x88, 0x00, 0x00, 0x00, 0x00},
   {0x08, 0x08, 0x08, 0x08, 0x3e, 0x20, 0x50, 0x88, 0x88, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
   {0x10, 0x10, 0x10, 0x10, 0x10, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x10, 0x10, 0x10, 0x10, 0x10, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
   {0x10, 0x10, 0x10, 0x10, 0x10, 0xff, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x10, 0x10, 0x10, 0x10, 0x10, 0x1f, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
   {0x10, 0x10, 0x10, 0x10, 0x10, 0xf0, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
   {0x10, 0x10, 0x10, 0x10, 0x10, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
   {0x00, 0x00, 0xfe, 0x02, 0x08, 0x20, 0x80, 0x20, 0x08, 0x02, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0xfe, 0x80, 0x20, 0x08, 0x02, 0x08, 0x20, 0x80, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x44, 0x24, 0x24, 0x24, 0x24, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x80, 0x40, 0xfe, 0x10, 0xfe, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0xdc, 0x62, 0x20, 0x20, 0x20, 0x70, 0x20, 0x22, 0x1c, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x10, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x24, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x24, 0x24, 0x7e, 0x24, 0x7e, 0x24, 0x24, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x10, 0x78, 0x14, 0x38, 0x50, 0x3c, 0x10, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x44, 0x2a, 0x24, 0x10, 0x08, 0x08, 0x24, 0x52, 0x22, 0x00, 0x00},
   {0x00, 0x00, 0x3a, 0x44, 0x4a, 0x30, 0x48, 0x48, 0x30, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x30, 0x38, 0x00, 0x00},
   {0x00, 0x00, 0x04, 0x08, 0x08, 0x10, 0x10, 0x10, 0x08, 0x08, 0x04, 0x00, 0x00},
   {0x00, 0x00, 0x20, 0x10, 0x10, 0x08, 0x08, 0x08, 0x10, 0x10, 0x20, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x24, 0x18, 0x7e, 0x18, 0x24, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x40, 0x30, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x10, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x80, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x02, 0x00, 0x00},
   {0x00, 0x00, 0x18, 0x24, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x50, 0x30, 0x10, 0x00, 0x00},
   {0x00, 0x00, 0x7e, 0x40, 0x20, 0x18, 0x04, 0x02, 0x42, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x02, 0x02, 0x1c, 0x08, 0x04, 0x02, 0x7e, 0x00, 0x00},
   {0x00, 0x00, 0x04, 0x04, 0x7e, 0x44, 0x44, 0x24, 0x14, 0x0c, 0x04, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x02, 0x02, 0x62, 0x5c, 0x40, 0x40, 0x7e, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x62, 0x5c, 0x40, 0x40, 0x20, 0x1c, 0x00, 0x00},
   {0x00, 0x00, 0x20, 0x20, 0x10, 0x10, 0x08, 0x08, 0x04, 0x02, 0x7e, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x3c, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0x38, 0x04, 0x02, 0x02, 0x3a, 0x46, 0x42, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x10, 0x38, 0x10, 0x00, 0x00, 0x10, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x40, 0x30, 0x38, 0x00, 0x00, 0x10, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x02, 0x04, 0x08, 0x10, 0x20, 0x10, 0x08, 0x04, 0x02, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x08, 0x10, 0x20, 0x40, 0x00, 0x00},
   {0x00, 0x00, 0x08, 0x00, 0x08, 0x08, 0x04, 0x02, 0x42, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x40, 0x4a, 0x56, 0x52, 0x4e, 0x42, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00},
   {0x00, 0x00, 0xfc, 0x42, 0x42, 0x42, 0x7c, 0x42, 0x42, 0x42, 0xfc, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x40, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0xfc, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0xfc, 0x00, 0x00},
   {0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00},
   {0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x78, 0x40, 0x40, 0x40, 0x7e, 0x00, 0x00},
   {0x00, 0x00, 0x3a, 0x46, 0x42, 0x4e, 0x40, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x00},
   {0x00, 0x00, 0x38, 0x44, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1e, 0x00, 0x00},
   {0x00, 0x00, 0x42, 0x44, 0x48, 0x50, 0x60, 0x50, 0x48, 0x44, 0x42, 0x00, 0x00},
   {0x00, 0x00, 0x7e, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x00, 0x00},
   {0x00, 0x00, 0x82, 0x82, 0x82, 0x92, 0x92, 0xaa, 0xc6, 0x82, 0x82, 0x00, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x42, 0x46, 0x4a, 0x52, 0x62, 0x42, 0x42, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0x40, 0x40, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x00, 0x00},
   {0x00, 0x02, 0x3c, 0x4a, 0x52, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0x42, 0x44, 0x48, 0x50, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x02, 0x02, 0x3c, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0xfe, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00},
   {0x00, 0x00, 0x10, 0x28, 0x28, 0x28, 0x44, 0x44, 0x44, 0x82, 0x82, 0x00, 0x00},
   {0x00, 0x00, 0x44, 0xaa, 0x92, 0x92, 0x92, 0x82, 0x82, 0x82, 0x82, 0x00, 0x00},
   {0x00, 0x00, 0x82, 0x82, 0x44, 0x28, 0x10, 0x28, 0x44, 0x82, 0x82, 0x00, 0x00},
   {0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x28, 0x44, 0x82, 0x82, 0x00, 0x00},
   {0x00, 0x00, 0x7e, 0x40, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x7e, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0x02, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x80, 0x00, 0x00},
   {0x00, 0x00, 0x78, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x78, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x44, 0x28, 0x10, 0x00, 0x00},
   {0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x18, 0x38, 0x00, 0x00},
   {0x00, 0x00, 0x3a, 0x46, 0x42, 0x3e, 0x02, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x5c, 0x62, 0x42, 0x42, 0x62, 0x5c, 0x40, 0x40, 0x40, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x3a, 0x46, 0x42, 0x42, 0x46, 0x3a, 0x02, 0x02, 0x02, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x40, 0x7e, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x7c, 0x20, 0x20, 0x22, 0x1c, 0x00, 0x00},
   {0x3c, 0x42, 0x3c, 0x40, 0x38, 0x44, 0x44, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x62, 0x5c, 0x40, 0x40, 0x40, 0x00, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x30, 0x00, 0x10, 0x00, 0x00, 0x00},
   {0x38, 0x44, 0x44, 0x04, 0x04, 0x04, 0x04, 0x0c, 0x00, 0x04, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x42, 0x44, 0x48, 0x70, 0x48, 0x44, 0x40, 0x40, 0x40, 0x00, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x30, 0x00, 0x00},
   {0x00, 0x00, 0x82, 0x92, 0x92, 0x92, 0x92, 0xec, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x62, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x40, 0x40, 0x40, 0x5c, 0x62, 0x42, 0x62, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x02, 0x02, 0x02, 0x3a, 0x46, 0x42, 0x46, 0x3a, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x22, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x0c, 0x30, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x1c, 0x22, 0x20, 0x20, 0x20, 0x7c, 0x20, 0x20, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x3a, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x10, 0x28, 0x28, 0x44, 0x44, 0x44, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x44, 0xaa, 0x92, 0x92, 0x82, 0x82, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x3c, 0x42, 0x02, 0x3a, 0x46, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x7e, 0x20, 0x10, 0x08, 0x04, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x0e, 0x10, 0x10, 0x08, 0x30, 0x08, 0x10, 0x10, 0x0e, 0x00, 0x00},
   {0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00},
   {0x00, 0x00, 0x70, 0x08, 0x08, 0x10, 0x0c, 0x10, 0x08, 0x08, 0x70, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x54, 0x24, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x10, 0x38, 0x54, 0x50, 0x50, 0x54, 0x38, 0x10, 0x00, 0x00},
   {0x00, 0x00, 0xdc, 0x62, 0x20, 0x20, 0x20, 0x70, 0x20, 0x22, 0x1c, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x42, 0x3c, 0x24, 0x24, 0x3c, 0x42, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x10, 0x10, 0x7c, 0x10, 0x7c, 0x28, 0x44, 0x82, 0x82, 0x00, 0x00},
   {0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x10, 0x10, 0x10, 0x10, 0x00, 0x00},
   {0x00, 0x00, 0x18, 0x24, 0x04, 0x18, 0x24, 0x24, 0x18, 0x20, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6c, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x38, 0x44, 0x92, 0xaa, 0xa2, 0xaa, 0x92, 0x44, 0x38, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x7c, 0x00, 0x3c, 0x44, 0x3c, 0x04, 0x38, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x12, 0x24, 0x48, 0x90, 0x48, 0x24, 0x12, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x02, 0x02, 0x02, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x38, 0x44, 0xaa, 0xb2, 0xaa, 0xaa, 0x92, 0x44, 0x38, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7e, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x24, 0x18, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x7c, 0x00, 0x10, 0x10, 0x7c, 0x10, 0x10, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x40, 0x30, 0x08, 0x48, 0x30, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x48, 0x08, 0x10, 0x48, 0x30, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x40, 0x5a, 0x66, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x14, 0x14, 0x14, 0x14, 0x34, 0x74, 0x74, 0x74, 0x3e, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x18, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x20, 0x20, 0x20, 0x60, 0x20, 0x00},
   {0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x30, 0x48, 0x48, 0x30, 0x00, 0x00},
   {0x00, 0x00, 0x00, 0x90, 0x48, 0x24, 0x12, 0x24, 0x48, 0x90, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x06, 0x1a, 0x12, 0x0a, 0xe6, 0x42, 0x40, 0x40, 0xc0, 0x40, 0x00},
   {0x00, 0x00, 0x1e, 0x10, 0x0c, 0x02, 0xf2, 0x4c, 0x40, 0x40, 0xc0, 0x40, 0x00},
   {0x00, 0x00, 0x06, 0x1a, 0x12, 0x0a, 0x66, 0x92, 0x10, 0x20, 0x90, 0x60, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x40, 0x20, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x24, 0x18, 0x00, 0x08, 0x10, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x24, 0x18, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x24, 0x18, 0x00, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x24, 0x18, 0x00, 0x28, 0x14, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x24, 0x18, 0x00, 0x24, 0x24, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x7e, 0x42, 0x42, 0x24, 0x18, 0x18, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x9e, 0x90, 0x90, 0xf0, 0x9c, 0x90, 0x90, 0x90, 0x6e, 0x00, 0x00},
   {0x10, 0x08, 0x3c, 0x42, 0x40, 0x40, 0x40, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00},
   {0x00, 0x00, 0x7e, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7e, 0x00, 0x08, 0x10, 0x00},
   {0x00, 0x00, 0x7e, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7e, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x00, 0x7e, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7e, 0x00, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x7e, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7e, 0x00, 0x24, 0x24, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x08, 0x10, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x10, 0x7c, 0x00, 0x28, 0x28, 0x00},
   {0x00, 0x00, 0xfc, 0x42, 0x42, 0x42, 0xe2, 0x42, 0x42, 0x42, 0xfc, 0x00, 0x00},
   {0x00, 0x00, 0x82, 0x86, 0x8a, 0x92, 0xa2, 0xc2, 0x82, 0x00, 0x28, 0x14, 0x00},
   {0x00, 0x00, 0x7c, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x00, 0x08, 0x10, 0x00},
   {0x00, 0x00, 0x7c, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x00, 0x7c, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x00, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x7c, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x00, 0x28, 0x14, 0x00},
   {0x00, 0x00, 0x7c, 0x82, 0x82, 0x82, 0x82, 0x82, 0x7c, 0x00, 0x28, 0x28, 0x00},
   {0x00, 0x00, 0x00, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x40, 0x3c, 0x62, 0x52, 0x52, 0x52, 0x4a, 0x4a, 0x46, 0x3c, 0x02, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x08, 0x10, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x42, 0x42, 0x00, 0x24, 0x24, 0x00},
   {0x00, 0x00, 0x10, 0x10, 0x10, 0x10, 0x28, 0x44, 0x44, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x00, 0x40, 0x40, 0x40, 0x7c, 0x42, 0x42, 0x42, 0x7c, 0x40, 0x00, 0x00},
   {0x00, 0x40, 0x5c, 0x62, 0x42, 0x42, 0x7c, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x3a, 0x46, 0x42, 0x3e, 0x02, 0x3c, 0x00, 0x00, 0x08, 0x10, 0x00},
   {0x00, 0x00, 0x3a, 0x46, 0x42, 0x3e, 0x02, 0x3c, 0x00, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x00, 0x3a, 0x46, 0x42, 0x3e, 0x02, 0x3c, 0x00, 0x00, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x3a, 0x46, 0x42, 0x3e, 0x02, 0x3c, 0x00, 0x00, 0x28, 0x14, 0x00},
   {0x00, 0x00, 0x3a, 0x46, 0x42, 0x3e, 0x02, 0x3c, 0x00, 0x00, 0x24, 0x24, 0x00},
   {0x00, 0x00, 0x3a, 0x46, 0x42, 0x3e, 0x02, 0x3c, 0x00, 0x18, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x6c, 0x92, 0x90, 0x7c, 0x12, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x10, 0x08, 0x3c, 0x42, 0x40, 0x40, 0x42, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x40, 0x7e, 0x42, 0x3c, 0x00, 0x00, 0x08, 0x10, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x40, 0x7e, 0x42, 0x3c, 0x00, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x40, 0x7e, 0x42, 0x3c, 0x00, 0x00, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x40, 0x7e, 0x42, 0x3c, 0x00, 0x00, 0x24, 0x24, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x30, 0x00, 0x00, 0x10, 0x20, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x30, 0x00, 0x00, 0x20, 0x10, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x30, 0x00, 0x00, 0x48, 0x30, 0x00},
   {0x00, 0x00, 0x7c, 0x10, 0x10, 0x10, 0x10, 0x30, 0x00, 0x00, 0x28, 0x28, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x04, 0x28, 0x18, 0x24, 0x00},
   {0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x62, 0x5c, 0x00, 0x00, 0x28, 0x14, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x08, 0x10, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x28, 0x14, 0x00},
   {0x00, 0x00, 0x3c, 0x42, 0x42, 0x42, 0x42, 0x3c, 0x00, 0x00, 0x24, 0x24, 0x00},
   {0x00, 0x00, 0x00, 0x10, 0x10, 0x00, 0x7c, 0x00, 0x10, 0x10, 0x00, 0x00, 0x00},
   {0x00, 0x40, 0x3c, 0x62, 0x52, 0x4a, 0x46, 0x3c, 0x02, 0x00, 0x00, 0x00, 0x00},
   {0x00, 0x00, 0x3a, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x08, 0x10, 0x00},
   {0x00, 0x00, 0x3a, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x10, 0x08, 0x00},
   {0x00, 0x00, 0x3a, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x24, 0x18, 0x00},
   {0x00, 0x00, 0x3a, 0x44, 0x44, 0x44, 0x44, 0x44, 0x00, 0x00, 0x24, 0x24, 0x00},
   {0x3c, 0x42, 0x02, 0x3a, 0x46, 0x42, 0x42, 0x42, 0x00, 0x00, 0x10, 0x08, 0x00},
   {0x40, 0x40, 0x5c, 0x62, 0x42, 0x42, 0x62, 0x5c, 0x40, 0x40, 0x00, 0x00, 0x00},
   {0x3c, 0x42, 0x02, 0x3a, 0x46, 0x42, 0x42, 0x42, 0x00, 0x00, 0x24, 0x24, 0x00}
};
} // namespace anonymous
