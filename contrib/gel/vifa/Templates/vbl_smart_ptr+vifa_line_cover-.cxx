// This avoids automatic instantiation of other smart pointers:
#define vsol_box_2d_sptr_h
class vsol_box_2d_sptr {};

#include <vifa/vifa_line_cover.h>
#include <vbl/vbl_smart_ptr.txx>

VBL_SMART_PTR_INSTANTIATE(vifa_line_cover);
