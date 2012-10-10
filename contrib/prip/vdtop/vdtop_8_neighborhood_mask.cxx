// This is prip/vdtop/vdtop_8_neighborhood_mask.cxx
#include "vdtop_8_neighborhood_mask.h"

#include <vil/vil_pixel_format.h>
#include <vil/vil_image_view.txx>

VCL_DEFINE_SPECIALIZATION
inline vil_pixel_format vil_pixel_format_of(vdtop_8_neighborhood_mask ) { return VIL_PIXEL_FORMAT_BYTE; }

VCL_DEFINE_SPECIALIZATION
inline bool convert_components_from_planes(vil_image_view<vdtop_8_neighborhood_mask> & /*lhs*/,
                                           const vil_image_view_base & /*rhs_base*/)
{
  return false;
}

VIL_IMAGE_VIEW_INSTANTIATE(vdtop_8_neighborhood_mask) ;

const vxl_byte vdtop_8_neighborhood_mask::nb8cc_[256] = {
  0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1,
  1, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1,
  1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 2,
  1, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1,
  1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1,
  1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
  1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1,
  1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
  1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1,
  2, 2, 3, 2, 2, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1,
  2, 2, 3, 2, 3, 2, 3, 2, 3, 3, 4, 3, 3, 2, 3, 2,
  2, 2, 3, 2, 2, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1,
  1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1,
  1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1,
  1, 1, 2, 1, 2, 1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1,
  1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1 };

const vxl_byte vdtop_8_neighborhood_mask::nb4cc_[256] = {
  0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1,
  1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1,
  0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1,
  1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1,
  1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 2, 3, 2, 2,
  2, 3, 2, 3, 3, 4, 3, 3, 2, 3, 2, 3, 2, 3, 2, 2,
  1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 2, 3, 2, 2,
  1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1,
  0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1,
  1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1,
  0, 1, 0, 1, 1, 2, 1, 1, 0, 1, 0, 1, 1, 2, 1, 1,
  1, 2, 1, 2, 2, 3, 2, 2, 1, 2, 1, 2, 1, 2, 1, 1,
  1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 1,
  2, 2, 2, 2, 3, 3, 3, 2, 2, 2, 2, 2, 2, 2, 2, 1,
  1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 1,
  1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

const vxl_byte vdtop_8_neighborhood_mask::m8cc_[256][4] = {
  {0,0,0,0},{1,0,0,0},{2,0,0,0},{3,0,0,0},{4,0,0,0},{5,0,0,0},{6,0,0,0},{7,0,0,0},
  {8,0,0,0},{1,8,0,0},{2,8,0,0},{3,8,0,0},{12,0,0,0},{13,0,0,0},{14,0,0,0},{15,0,0,0},
  {16,0,0,0},{1,16,0,0},{2,16,0,0},{3,16,0,0},{20,0,0,0},{21,0,0,0},{22,0,0,0},{23,0,0,0},
  {24,0,0,0},{1,24,0,0},{2,24,0,0},{3,24,0,0},{28,0,0,0},{29,0,0,0},{30,0,0,0},{31,0,0,0},
  {32,0,0,0},{1,32,0,0},{2,32,0,0},{3,32,0,0},{4,32,0,0},{5,32,0,0},{6,32,0,0},{7,32,0,0},
  {8,32,0,0},{1,8,32,0},{2,8,32,0},{3,8,32,0},{12,32,0,0},{13,32,0,0},{14,32,0,0},{15,32,0,0},
  {48,0,0,0},{1,48,0,0},{2,48,0,0},{3,48,0,0},{52,0,0,0},{53,0,0,0},{54,0,0,0},{55,0,0,0},
  {56,0,0,0},{1,56,0,0},{2,56,0,0},{3,56,0,0},{60,0,0,0},{61,0,0,0},{62,0,0,0},{63,0,0,0},
  {64,0,0,0},{65,0,0,0},{2,64,0,0},{67,0,0,0},{4,64,0,0},{69,0,0,0},{6,64,0,0},{71,0,0,0},
  {8,64,0,0},{65,8,0,0},{2,8,64,0},{67,8,0,0},{12,64,0,0},{77,0,0,0},{14,64,0,0},{79,0,0,0},
  {80,0,0,0},{81,0,0,0},{2,80,0,0},{83,0,0,0},{84,0,0,0},{85,0,0,0},{86,0,0,0},{87,0,0,0},
  {88,0,0,0},{89,0,0,0},{2,88,0,0},{91,0,0,0},{92,0,0,0},{93,0,0,0},{94,0,0,0},{95,0,0,0},
  {96,0,0,0},{97,0,0,0},{2,96,0,0},{99,0,0,0},{4,96,0,0},{101,0,0,0},{6,96,0,0},{103,0,0,0},
  {8,96,0,0},{97,8,0,0},{2,8,96,0},{99,8,0,0},{12,96,0,0},{109,0,0,0},{14,96,0,0},{111,0,0,0},
  {112,0,0,0},{113,0,0,0},{2,112,0,0},{115,0,0,0},{116,0,0,0},{117,0,0,0},{118,0,0,0},{119,0,0,0},
  {120,0,0,0},{121,0,0,0},{2,120,0,0},{123,0,0,0},{124,0,0,0},{125,0,0,0},{126,0,0,0},{127,0,0,0},
  {128,0,0,0},{129,0,0,0},{2,128,0,0},{131,0,0,0},{4,128,0,0},{133,0,0,0},{6,128,0,0},{135,0,0,0},
  {8,128,0,0},{129,8,0,0},{2,8,128,0},{131,8,0,0},{12,128,0,0},{141,0,0,0},{14,128,0,0},{143,0,0,0},
  {16,128,0,0},{129,16,0,0},{2,16,128,0},{131,16,0,0},{20,128,0,0},{149,0,0,0},{22,128,0,0},{151,0,0,0},
  {24,128,0,0},{129,24,0,0},{2,24,128,0},{131,24,0,0},{28,128,0,0},{157,0,0,0},{30,128,0,0},{159,0,0,0},
  {32,128,0,0},{129,32,0,0},{2,32,128,0},{131,32,0,0},{4,32,128,0},{133,32,0,0},{6,32,128,0},{135,32,0,0},
  {8,32,128,0},{129,8,32,0},{2,8,32,128},{131,8,32,0},{12,32,128,0},{141,32,0,0},{14,32,128,0},{143,32,0,0},
  {48,128,0,0},{129,48,0,0},{2,48,128,0},{131,48,0,0},{52,128,0,0},{181,0,0,0},{54,128,0,0},{183,0,0,0},
  {56,128,0,0},{129,56,0,0},{2,56,128,0},{131,56,0,0},{60,128,0,0},{189,0,0,0},{62,128,0,0},{191,0,0,0},
  {192,0,0,0},{193,0,0,0},{2,192,0,0},{195,0,0,0},{4,192,0,0},{197,0,0,0},{6,192,0,0},{199,0,0,0},
  {8,192,0,0},{193,8,0,0},{2,8,192,0},{195,8,0,0},{12,192,0,0},{205,0,0,0},{14,192,0,0},{207,0,0,0},
  {208,0,0,0},{209,0,0,0},{2,208,0,0},{211,0,0,0},{212,0,0,0},{213,0,0,0},{214,0,0,0},{215,0,0,0},
  {216,0,0,0},{217,0,0,0},{2,216,0,0},{219,0,0,0},{220,0,0,0},{221,0,0,0},{222,0,0,0},{223,0,0,0},
  {224,0,0,0},{225,0,0,0},{2,224,0,0},{227,0,0,0},{4,224,0,0},{229,0,0,0},{6,224,0,0},{231,0,0,0},
  {8,224,0,0},{225,8,0,0},{2,8,224,0},{227,8,0,0},{12,224,0,0},{237,0,0,0},{14,224,0,0},{239,0,0,0},
  {240,0,0,0},{241,0,0,0},{2,240,0,0},{243,0,0,0},{244,0,0,0},{245,0,0,0},{246,0,0,0},{247,0,0,0},
  {248,0,0,0},{249,0,0,0},{2,248,0,0},{251,0,0,0},{252,0,0,0},{253,0,0,0},{254,0,0,0},{255,0,0,0} };

const vxl_byte vdtop_8_neighborhood_mask::m4cc_[256][4] = {
  {0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{4,0,0,0},{1,4,0,0},{4,0,0,0},{5,0,0,0},
  {0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{4,0,0,0},{1,4,0,0},{4,0,0,0},{5,0,0,0},
  {16,0,0,0},{1,16,0,0},{16,0,0,0},{1,16,0,0},{4,16,0,0},{1,4,16,0},{4,16,0,0},{5,16,0,0},
  {16,0,0,0},{1,16,0,0},{16,0,0,0},{1,16,0,0},{20,0,0,0},{1,20,0,0},{20,0,0,0},{21,0,0,0},
  {0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{4,0,0,0},{1,4,0,0},{4,0,0,0},{5,0,0,0},
  {0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{4,0,0,0},{1,4,0,0},{4,0,0,0},{5,0,0,0},
  {16,0,0,0},{1,16,0,0},{16,0,0,0},{1,16,0,0},{4,16,0,0},{1,4,16,0},{4,16,0,0},{5,16,0,0},
  {16,0,0,0},{1,16,0,0},{16,0,0,0},{1,16,0,0},{20,0,0,0},{1,20,0,0},{20,0,0,0},{21,0,0,0},
  {64,0,0,0},{1,64,0,0},{64,0,0,0},{1,64,0,0},{4,64,0,0},{1,4,64,0},{4,64,0,0},{5,64,0,0},
  {64,0,0,0},{1,64,0,0},{64,0,0,0},{1,64,0,0},{4,64,0,0},{1,4,64,0},{4,64,0,0},{5,64,0,0},
  {16,64,0,0},{1,16,64,0},{16,64,0,0},{1,16,64,0},{4,16,64,0},{1,4,16,64},{4,16,64,0},{5,16,64,0},
  {16,64,0,0},{1,16,64,0},{16,64,0,0},{1,16,64,0},{20,64,0,0},{1,20,64,0},{20,64,0,0},{21,64,0,0},
  {64,0,0,0},{1,64,0,0},{64,0,0,0},{1,64,0,0},{4,64,0,0},{1,4,64,0},{4,64,0,0},{5,64,0,0},
  {64,0,0,0},{1,64,0,0},{64,0,0,0},{1,64,0,0},{4,64,0,0},{1,4,64,0},{4,64,0,0},{5,64,0,0},
  {80,0,0,0},{1,80,0,0},{80,0,0,0},{1,80,0,0},{4,80,0,0},{1,4,80,0},{4,80,0,0},{5,80,0,0},
  {80,0,0,0},{1,80,0,0},{80,0,0,0},{1,80,0,0},{84,0,0,0},{1,84,0,0},{84,0,0,0},{85,0,0,0},
  {0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{4,0,0,0},{1,4,0,0},{4,0,0,0},{5,0,0,0},
  {0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{4,0,0,0},{1,4,0,0},{4,0,0,0},{5,0,0,0},
  {16,0,0,0},{1,16,0,0},{16,0,0,0},{1,16,0,0},{4,16,0,0},{1,4,16,0},{4,16,0,0},{5,16,0,0},
  {16,0,0,0},{1,16,0,0},{16,0,0,0},{1,16,0,0},{20,0,0,0},{1,20,0,0},{20,0,0,0},{21,0,0,0},
  {0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{4,0,0,0},{1,4,0,0},{4,0,0,0},{5,0,0,0},
  {0,0,0,0},{1,0,0,0},{0,0,0,0},{1,0,0,0},{4,0,0,0},{1,4,0,0},{4,0,0,0},{5,0,0,0},
  {16,0,0,0},{1,16,0,0},{16,0,0,0},{1,16,0,0},{4,16,0,0},{1,4,16,0},{4,16,0,0},{5,16,0,0},
  {16,0,0,0},{1,16,0,0},{16,0,0,0},{1,16,0,0},{20,0,0,0},{1,20,0,0},{20,0,0,0},{21,0,0,0},
  {64,0,0,0},{65,0,0,0},{64,0,0,0},{65,0,0,0},{4,64,0,0},{65,4,0,0},{4,64,0,0},{69,0,0,0},
  {64,0,0,0},{65,0,0,0},{64,0,0,0},{65,0,0,0},{4,64,0,0},{65,4,0,0},{4,64,0,0},{69,0,0,0},
  {16,64,0,0},{65,16,0,0},{16,64,0,0},{65,16,0,0},{4,16,64,0},{65,4,16,0},{4,16,64,0},{69,16,0,0},
  {16,64,0,0},{65,16,0,0},{16,64,0,0},{65,16,0,0},{20,64,0,0},{65,20,0,0},{20,64,0,0},{85,0,0,0},
  {64,0,0,0},{65,0,0,0},{64,0,0,0},{65,0,0,0},{4,64,0,0},{65,4,0,0},{4,64,0,0},{69,0,0,0},
  {64,0,0,0},{65,0,0,0},{64,0,0,0},{65,0,0,0},{4,64,0,0},{65,4,0,0},{4,64,0,0},{69,0,0,0},
  {80,0,0,0},{81,0,0,0},{80,0,0,0},{81,0,0,0},{4,80,0,0},{81,4,0,0},{4,80,0,0},{85,0,0,0},
  {80,0,0,0},{81,0,0,0},{80,0,0,0},{81,0,0,0},{84,0,0,0},{85,0,0,0},{84,0,0,0},{85,0,0,0} };

const bool vdtop_8_neighborhood_mask::m8simple_[256] = {
  false, true, true, true, true, true, true, true, true,false,false,false, true, true, true, true,
   true,false,false,false, true, true, true, true, true,false,false,false, true, true, true, true,
   true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
   true,false,false,false, true, true, true, true, true,false,false,false, true, true, true, true,
   true, true,false, true,false, true,false, true,false,false,false,false,false, true,false, true,
   true, true,false, true, true,false, true,false, true, true,false, true, true,false, true,false,
   true, true,false, true,false, true,false, true,false,false,false,false,false, true,false, true,
   true, true,false, true, true,false, true,false, true, true,false, true, true,false, true,false,
   true, true,false, true,false, true,false, true,false,false,false,false,false, true,false, true,
  false,false,false,false,false, true,false, true,false,false,false,false,false, true,false, true,
  false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
  false,false,false,false,false, true,false, true,false,false,false,false,false, true,false, true,
   true, true,false, true,false, true,false, true,false,false,false,false,false, true,false, true,
   true, true,false, true, true,false, true,false, true, true,false, true, true,false, true,false,
   true, true,false, true,false, true,false, true,false,false,false,false,false, true,false, true,
   true, true,false, true, true,false, true,false, true, true,false, true, true,false, true,false };

const bool vdtop_8_neighborhood_mask::m4simple_[256] = {
  false, true,false, true, true,false, true, true,false, true,false, true, true,false, true, true,
   true,false, true,false,false,false,false,false, true,false, true,false, true,false, true, true,
  false, true,false, true, true,false, true, true,false, true,false, true, true,false, true, true,
   true,false, true,false,false,false,false,false, true,false, true,false, true,false, true, true,
   true,false, true,false,false,false,false,false, true,false, true,false,false,false,false,false,
  false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
   true,false, true,false,false,false,false,false, true,false, true,false,false,false,false,false,
   true,false, true,false,false,false,false,false, true,false, true,false, true,false, true, true,
  false, true,false, true, true,false, true, true,false, true,false, true, true,false, true, true,
   true,false, true,false,false,false,false,false, true,false, true,false, true,false, true, true,
  false, true,false, true, true,false, true, true,false, true,false, true, true,false, true, true,
   true,false, true,false,false,false,false,false, true,false, true,false, true,false, true, true,
   true, true, true, true,false,false,false, true, true, true, true, true,false,false,false, true,
  false,false,false,false,false,false,false,false,false,false,false,false,false,false,false, true,
   true, true, true, true,false,false,false, true, true, true, true, true,false,false,false, true,
   true, true, true, true,false,false,false, true, true, true, true, true, true, true, true,false };

const bool vdtop_8_neighborhood_mask::m8simple_or_isolated_[256] = {
   true, true, true, true, true, true, true, true, true,false,false,false, true, true, true, true,
   true,false,false,false, true, true, true, true, true,false,false,false, true, true, true, true,
   true,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
   true,false,false,false, true, true, true, true, true,false,false,false, true, true, true, true,
   true, true,false, true,false, true,false, true,false,false,false,false,false, true,false, true,
   true, true,false, true, true,false, true,false, true, true,false, true, true,false, true,false,
   true, true,false, true,false, true,false, true,false,false,false,false,false, true,false, true,
   true, true,false, true, true,false, true,false, true, true,false, true, true,false, true,false,
   true, true,false, true,false, true,false, true,false,false,false,false,false, true,false, true,
  false,false,false,false,false, true,false, true,false,false,false,false,false, true,false, true,
  false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
  false,false,false,false,false, true,false, true,false,false,false,false,false, true,false, true,
   true, true,false, true,false, true,false, true,false,false,false,false,false, true,false, true,
   true, true,false, true, true,false, true,false, true, true,false, true, true,false, true,false,
   true, true,false, true,false, true,false, true,false,false,false,false,false, true,false, true,
   true, true,false, true, true,false, true,false, true, true,false, true, true,false, true,false };

const bool vdtop_8_neighborhood_mask::m4simple_or_isolated_[256] = {
   true, true, true, true, true,false, true, true, true, true, true, true, true,false, true, true,
   true,false, true,false,false,false,false,false, true,false, true,false, true,false, true, true,
   true, true, true, true, true,false, true, true, true, true, true, true, true,false, true, true,
   true,false, true,false,false,false,false,false, true,false, true,false, true,false, true, true,
   true,false, true,false,false,false,false,false, true,false, true,false,false,false,false,false,
  false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,
   true,false, true,false,false,false,false,false, true,false, true,false,false,false,false,false,
   true,false, true,false,false,false,false,false, true,false, true,false, true,false, true, true,
   true, true, true, true, true,false, true, true, true, true, true, true, true,false, true, true,
   true,false, true,false,false,false,false,false, true,false, true,false, true,false, true, true,
   true, true, true, true, true,false, true, true, true, true, true, true, true,false, true, true,
   true,false, true,false,false,false,false,false, true,false, true,false, true,false, true, true,
   true, true, true, true,false,false,false, true, true, true, true, true,false,false,false, true,
  false,false,false,false,false,false,false,false,false,false,false,false,false,false,false, true,
   true, true, true, true,false,false,false, true, true, true, true, true,false,false,false, true,
   true, true, true, true,false,false,false, true, true, true, true, true, true, true, true,false };

const vxl_byte vdtop_8_neighborhood_mask::nb4neighbors_[256] = {
  0,1,0,1,1,2,1,2,0,1,0,1,1,2,1,2,1,2,1,2,2,3,2,3,1,2,1,2,2,3,2,3,0,1,0,1,1,2,1,2,0,1,0,1,1,2,1,2,1,2,1,2,2,3,2,3,1,2,1,2,2,3,2,3,
  1,2,1,2,2,3,2,3,1,2,1,2,2,3,2,3,2,3,2,3,3,4,3,4,2,3,2,3,3,4,3,4,1,2,1,2,2,3,2,3,1,2,1,2,2,3,2,3,2,3,2,3,3,4,3,4,2,3,2,3,3,4,3,4,
  0,1,0,1,1,2,1,2,0,1,0,1,1,2,1,2,1,2,1,2,2,3,2,3,1,2,1,2,2,3,2,3,0,1,0,1,1,2,1,2,0,1,0,1,1,2,1,2,1,2,1,2,2,3,2,3,1,2,1,2,2,3,2,3,
  1,2,1,2,2,3,2,3,1,2,1,2,2,3,2,3,2,3,2,3,3,4,3,4,2,3,2,3,3,4,3,4,1,2,1,2,2,3,2,3,1,2,1,2,2,3,2,3,2,3,2,3,3,4,3,4,2,3,2,3,3,4,3,4 };
const vxl_byte vdtop_8_neighborhood_mask::nb8neighbors_[256] = {
  0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,
  2,3,3,4,3,4,4,5,3,4,4,5,4,5,5,6,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,3,4,4,5,4,5,5,6,4,5,5,6,5,6,6,7,4,5,5,6,5,6,6,7,5,6,6,7,6,7,7,8 };
const vxl_byte vdtop_8_neighborhood_mask::m4dir_[256][4] = {
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,0,0},{0,2,0,0},{2,0,0,0},{0,2,0,0},
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,0,0},{0,2,0,0},{2,0,0,0},{0,2,0,0},
  {4,0,0,0},{0,4,0,0},{4,0,0,0},{0,4,0,0},{2,4,0,0},{0,2,4,0},{2,4,0,0},{0,2,4,0},
  {4,0,0,0},{0,4,0,0},{4,0,0,0},{0,4,0,0},{2,4,0,0},{0,2,4,0},{2,4,0,0},{0,2,4,0},
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,0,0},{0,2,0,0},{2,0,0,0},{0,2,0,0},
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,0,0},{0,2,0,0},{2,0,0,0},{0,2,0,0},
  {4,0,0,0},{0,4,0,0},{4,0,0,0},{0,4,0,0},{2,4,0,0},{0,2,4,0},{2,4,0,0},{0,2,4,0},
  {4,0,0,0},{0,4,0,0},{4,0,0,0},{0,4,0,0},{2,4,0,0},{0,2,4,0},{2,4,0,0},{0,2,4,0},
  {6,0,0,0},{0,6,0,0},{6,0,0,0},{0,6,0,0},{2,6,0,0},{0,2,6,0},{2,6,0,0},{0,2,6,0},
  {6,0,0,0},{0,6,0,0},{6,0,0,0},{0,6,0,0},{2,6,0,0},{0,2,6,0},{2,6,0,0},{0,2,6,0},
  {4,6,0,0},{0,4,6,0},{4,6,0,0},{0,4,6,0},{2,4,6,0},{0,2,4,6},{2,4,6,0},{0,2,4,6},
  {4,6,0,0},{0,4,6,0},{4,6,0,0},{0,4,6,0},{2,4,6,0},{0,2,4,6},{2,4,6,0},{0,2,4,6},
  {6,0,0,0},{0,6,0,0},{6,0,0,0},{0,6,0,0},{2,6,0,0},{0,2,6,0},{2,6,0,0},{0,2,6,0},
  {6,0,0,0},{0,6,0,0},{6,0,0,0},{0,6,0,0},{2,6,0,0},{0,2,6,0},{2,6,0,0},{0,2,6,0},
  {4,6,0,0},{0,4,6,0},{4,6,0,0},{0,4,6,0},{2,4,6,0},{0,2,4,6},{2,4,6,0},{0,2,4,6},
  {4,6,0,0},{0,4,6,0},{4,6,0,0},{0,4,6,0},{2,4,6,0},{0,2,4,6},{2,4,6,0},{0,2,4,6},
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,0,0},{0,2,0,0},{2,0,0,0},{0,2,0,0},
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,0,0},{0,2,0,0},{2,0,0,0},{0,2,0,0},
  {4,0,0,0},{0,4,0,0},{4,0,0,0},{0,4,0,0},{2,4,0,0},{0,2,4,0},{2,4,0,0},{0,2,4,0},
  {4,0,0,0},{0,4,0,0},{4,0,0,0},{0,4,0,0},{2,4,0,0},{0,2,4,0},{2,4,0,0},{0,2,4,0},
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,0,0},{0,2,0,0},{2,0,0,0},{0,2,0,0},
  {0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{2,0,0,0},{0,2,0,0},{2,0,0,0},{0,2,0,0},
  {4,0,0,0},{0,4,0,0},{4,0,0,0},{0,4,0,0},{2,4,0,0},{0,2,4,0},{2,4,0,0},{0,2,4,0},
  {4,0,0,0},{0,4,0,0},{4,0,0,0},{0,4,0,0},{2,4,0,0},{0,2,4,0},{2,4,0,0},{0,2,4,0},
  {6,0,0,0},{0,6,0,0},{6,0,0,0},{0,6,0,0},{2,6,0,0},{0,2,6,0},{2,6,0,0},{0,2,6,0},
  {6,0,0,0},{0,6,0,0},{6,0,0,0},{0,6,0,0},{2,6,0,0},{0,2,6,0},{2,6,0,0},{0,2,6,0},
  {4,6,0,0},{0,4,6,0},{4,6,0,0},{0,4,6,0},{2,4,6,0},{0,2,4,6},{2,4,6,0},{0,2,4,6},
  {4,6,0,0},{0,4,6,0},{4,6,0,0},{0,4,6,0},{2,4,6,0},{0,2,4,6},{2,4,6,0},{0,2,4,6},
  {6,0,0,0},{0,6,0,0},{6,0,0,0},{0,6,0,0},{2,6,0,0},{0,2,6,0},{2,6,0,0},{0,2,6,0},
  {6,0,0,0},{0,6,0,0},{6,0,0,0},{0,6,0,0},{2,6,0,0},{0,2,6,0},{2,6,0,0},{0,2,6,0},
  {4,6,0,0},{0,4,6,0},{4,6,0,0},{0,4,6,0},{2,4,6,0},{0,2,4,6},{2,4,6,0},{0,2,4,6},
  {4,6,0,0},{0,4,6,0},{4,6,0,0},{0,4,6,0},{2,4,6,0},{0,2,4,6},{2,4,6,0},{0,2,4,6} };

const vxl_byte vdtop_8_neighborhood_mask::m8dir_[256][8] = {
  {0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{1,0,0,0,0,0,0,0},{0,1,0,0,0,0,0,0},
  {2,0,0,0,0,0,0,0},{0,2,0,0,0,0,0,0},{1,2,0,0,0,0,0,0},{0,1,2,0,0,0,0,0},
  {3,0,0,0,0,0,0,0},{0,3,0,0,0,0,0,0},{1,3,0,0,0,0,0,0},{0,1,3,0,0,0,0,0},
  {2,3,0,0,0,0,0,0},{0,2,3,0,0,0,0,0},{1,2,3,0,0,0,0,0},{0,1,2,3,0,0,0,0},
  {4,0,0,0,0,0,0,0},{0,4,0,0,0,0,0,0},{1,4,0,0,0,0,0,0},{0,1,4,0,0,0,0,0},
  {2,4,0,0,0,0,0,0},{0,2,4,0,0,0,0,0},{1,2,4,0,0,0,0,0},{0,1,2,4,0,0,0,0},
  {3,4,0,0,0,0,0,0},{0,3,4,0,0,0,0,0},{1,3,4,0,0,0,0,0},{0,1,3,4,0,0,0,0},
  {2,3,4,0,0,0,0,0},{0,2,3,4,0,0,0,0},{1,2,3,4,0,0,0,0},{0,1,2,3,4,0,0,0},
  {5,0,0,0,0,0,0,0},{0,5,0,0,0,0,0,0},{1,5,0,0,0,0,0,0},{0,1,5,0,0,0,0,0},
  {2,5,0,0,0,0,0,0},{0,2,5,0,0,0,0,0},{1,2,5,0,0,0,0,0},{0,1,2,5,0,0,0,0},
  {3,5,0,0,0,0,0,0},{0,3,5,0,0,0,0,0},{1,3,5,0,0,0,0,0},{0,1,3,5,0,0,0,0},
  {2,3,5,0,0,0,0,0},{0,2,3,5,0,0,0,0},{1,2,3,5,0,0,0,0},{0,1,2,3,5,0,0,0},
  {4,5,0,0,0,0,0,0},{0,4,5,0,0,0,0,0},{1,4,5,0,0,0,0,0},{0,1,4,5,0,0,0,0},
  {2,4,5,0,0,0,0,0},{0,2,4,5,0,0,0,0},{1,2,4,5,0,0,0,0},{0,1,2,4,5,0,0,0},
  {3,4,5,0,0,0,0,0},{0,3,4,5,0,0,0,0},{1,3,4,5,0,0,0,0},{0,1,3,4,5,0,0,0},
  {2,3,4,5,0,0,0,0},{0,2,3,4,5,0,0,0},{1,2,3,4,5,0,0,0},{0,1,2,3,4,5,0,0},
  {6,0,0,0,0,0,0,0},{0,6,0,0,0,0,0,0},{1,6,0,0,0,0,0,0},{0,1,6,0,0,0,0,0},
  {2,6,0,0,0,0,0,0},{0,2,6,0,0,0,0,0},{1,2,6,0,0,0,0,0},{0,1,2,6,0,0,0,0},
  {3,6,0,0,0,0,0,0},{0,3,6,0,0,0,0,0},{1,3,6,0,0,0,0,0},{0,1,3,6,0,0,0,0},
  {2,3,6,0,0,0,0,0},{0,2,3,6,0,0,0,0},{1,2,3,6,0,0,0,0},{0,1,2,3,6,0,0,0},
  {4,6,0,0,0,0,0,0},{0,4,6,0,0,0,0,0},{1,4,6,0,0,0,0,0},{0,1,4,6,0,0,0,0},
  {2,4,6,0,0,0,0,0},{0,2,4,6,0,0,0,0},{1,2,4,6,0,0,0,0},{0,1,2,4,6,0,0,0},
  {3,4,6,0,0,0,0,0},{0,3,4,6,0,0,0,0},{1,3,4,6,0,0,0,0},{0,1,3,4,6,0,0,0},
  {2,3,4,6,0,0,0,0},{0,2,3,4,6,0,0,0},{1,2,3,4,6,0,0,0},{0,1,2,3,4,6,0,0},
  {5,6,0,0,0,0,0,0},{0,5,6,0,0,0,0,0},{1,5,6,0,0,0,0,0},{0,1,5,6,0,0,0,0},
  {2,5,6,0,0,0,0,0},{0,2,5,6,0,0,0,0},{1,2,5,6,0,0,0,0},{0,1,2,5,6,0,0,0},
  {3,5,6,0,0,0,0,0},{0,3,5,6,0,0,0,0},{1,3,5,6,0,0,0,0},{0,1,3,5,6,0,0,0},
  {2,3,5,6,0,0,0,0},{0,2,3,5,6,0,0,0},{1,2,3,5,6,0,0,0},{0,1,2,3,5,6,0,0},
  {4,5,6,0,0,0,0,0},{0,4,5,6,0,0,0,0},{1,4,5,6,0,0,0,0},{0,1,4,5,6,0,0,0},
  {2,4,5,6,0,0,0,0},{0,2,4,5,6,0,0,0},{1,2,4,5,6,0,0,0},{0,1,2,4,5,6,0,0},
  {3,4,5,6,0,0,0,0},{0,3,4,5,6,0,0,0},{1,3,4,5,6,0,0,0},{0,1,3,4,5,6,0,0},
  {2,3,4,5,6,0,0,0},{0,2,3,4,5,6,0,0},{1,2,3,4,5,6,0,0},{0,1,2,3,4,5,6,0},
  {7,0,0,0,0,0,0,0},{0,7,0,0,0,0,0,0},{1,7,0,0,0,0,0,0},{0,1,7,0,0,0,0,0},
  {2,7,0,0,0,0,0,0},{0,2,7,0,0,0,0,0},{1,2,7,0,0,0,0,0},{0,1,2,7,0,0,0,0},
  {3,7,0,0,0,0,0,0},{0,3,7,0,0,0,0,0},{1,3,7,0,0,0,0,0},{0,1,3,7,0,0,0,0},
  {2,3,7,0,0,0,0,0},{0,2,3,7,0,0,0,0},{1,2,3,7,0,0,0,0},{0,1,2,3,7,0,0,0},
  {4,7,0,0,0,0,0,0},{0,4,7,0,0,0,0,0},{1,4,7,0,0,0,0,0},{0,1,4,7,0,0,0,0},
  {2,4,7,0,0,0,0,0},{0,2,4,7,0,0,0,0},{1,2,4,7,0,0,0,0},{0,1,2,4,7,0,0,0},
  {3,4,7,0,0,0,0,0},{0,3,4,7,0,0,0,0},{1,3,4,7,0,0,0,0},{0,1,3,4,7,0,0,0},
  {2,3,4,7,0,0,0,0},{0,2,3,4,7,0,0,0},{1,2,3,4,7,0,0,0},{0,1,2,3,4,7,0,0},
  {5,7,0,0,0,0,0,0},{0,5,7,0,0,0,0,0},{1,5,7,0,0,0,0,0},{0,1,5,7,0,0,0,0},
  {2,5,7,0,0,0,0,0},{0,2,5,7,0,0,0,0},{1,2,5,7,0,0,0,0},{0,1,2,5,7,0,0,0},
  {3,5,7,0,0,0,0,0},{0,3,5,7,0,0,0,0},{1,3,5,7,0,0,0,0},{0,1,3,5,7,0,0,0},
  {2,3,5,7,0,0,0,0},{0,2,3,5,7,0,0,0},{1,2,3,5,7,0,0,0},{0,1,2,3,5,7,0,0},
  {4,5,7,0,0,0,0,0},{0,4,5,7,0,0,0,0},{1,4,5,7,0,0,0,0},{0,1,4,5,7,0,0,0},
  {2,4,5,7,0,0,0,0},{0,2,4,5,7,0,0,0},{1,2,4,5,7,0,0,0},{0,1,2,4,5,7,0,0},
  {3,4,5,7,0,0,0,0},{0,3,4,5,7,0,0,0},{1,3,4,5,7,0,0,0},{0,1,3,4,5,7,0,0},
  {2,3,4,5,7,0,0,0},{0,2,3,4,5,7,0,0},{1,2,3,4,5,7,0,0},{0,1,2,3,4,5,7,0},
  {6,7,0,0,0,0,0,0},{0,6,7,0,0,0,0,0},{1,6,7,0,0,0,0,0},{0,1,6,7,0,0,0,0},
  {2,6,7,0,0,0,0,0},{0,2,6,7,0,0,0,0},{1,2,6,7,0,0,0,0},{0,1,2,6,7,0,0,0},
  {3,6,7,0,0,0,0,0},{0,3,6,7,0,0,0,0},{1,3,6,7,0,0,0,0},{0,1,3,6,7,0,0,0},
  {2,3,6,7,0,0,0,0},{0,2,3,6,7,0,0,0},{1,2,3,6,7,0,0,0},{0,1,2,3,6,7,0,0},
  {4,6,7,0,0,0,0,0},{0,4,6,7,0,0,0,0},{1,4,6,7,0,0,0,0},{0,1,4,6,7,0,0,0},
  {2,4,6,7,0,0,0,0},{0,2,4,6,7,0,0,0},{1,2,4,6,7,0,0,0},{0,1,2,4,6,7,0,0},
  {3,4,6,7,0,0,0,0},{0,3,4,6,7,0,0,0},{1,3,4,6,7,0,0,0},{0,1,3,4,6,7,0,0},
  {2,3,4,6,7,0,0,0},{0,2,3,4,6,7,0,0},{1,2,3,4,6,7,0,0},{0,1,2,3,4,6,7,0},
  {5,6,7,0,0,0,0,0},{0,5,6,7,0,0,0,0},{1,5,6,7,0,0,0,0},{0,1,5,6,7,0,0,0},
  {2,5,6,7,0,0,0,0},{0,2,5,6,7,0,0,0},{1,2,5,6,7,0,0,0},{0,1,2,5,6,7,0,0},
  {3,5,6,7,0,0,0,0},{0,3,5,6,7,0,0,0},{1,3,5,6,7,0,0,0},{0,1,3,5,6,7,0,0},
  {2,3,5,6,7,0,0,0},{0,2,3,5,6,7,0,0},{1,2,3,5,6,7,0,0},{0,1,2,3,5,6,7,0},
  {4,5,6,7,0,0,0,0},{0,4,5,6,7,0,0,0},{1,4,5,6,7,0,0,0},{0,1,4,5,6,7,0,0},
  {2,4,5,6,7,0,0,0},{0,2,4,5,6,7,0,0},{1,2,4,5,6,7,0,0},{0,1,2,4,5,6,7,0},
  {3,4,5,6,7,0,0,0},{0,3,4,5,6,7,0,0},{1,3,4,5,6,7,0,0},{0,1,3,4,5,6,7,0},
  {2,3,4,5,6,7,0,0},{0,2,3,4,5,6,7,0},{1,2,3,4,5,6,7,0},{0,1,2,3,4,5,6,7} };

#if 0
bool vdtop_8_neighborhood_mask::simple_[256] ;
#endif // 0
