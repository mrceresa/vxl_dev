#ifndef vgui_accelerate_x11_h_
#define vgui_accelerate_x11_h_
#ifdef __GNUC__
#pragma interface
#endif
// .NAME vgui_accelerate_x11
// .INCLUDE vgui/internals/vgui_accelerate_x11.h
// .FILE internals/vgui_accelerate_x11.cxx

#include <vgui/vgui_gl.h>
#include <vgui/internals/vgui_accelerate.h>

#if VGUI_MESA
#  include <GL/xmesa.h>
#endif

#ifdef HAS_HERMES
#  include <Hermes/Hermes.h>
#endif

class vgui_accelerate_x11 : public vgui_accelerate {
public:
  vgui_accelerate_x11();
  virtual ~vgui_accelerate_x11();
 
  virtual bool vgui_glDrawPixels( GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels );
  virtual bool vgui_glClear( GLbitfield mask );
  virtual bool vgui_choose_cache_format( GLenum* format, GLenum* type);
  virtual bool vgui_copy_aux_to_back();
  virtual bool vgui_copy_back_to_aux();
protected:
  char* aux_buffer;
  int aux_buffer_size;

#ifdef HAS_HERMES
  HermesHandle hermes_clearer;
  HermesHandle hermes_converter;
#endif
};

#endif // vgui_accelerate_x11_h_
