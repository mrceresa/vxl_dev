// This is tbl/vepl/tests/vepl_test_threshold.cxx

//:
// \file
//  Test of the vepl_threshold templated IP classes
//  vepl_threshold<vil_image,vil_image,T,T>
//  for T in { unsigned char, unsigned short, float }.
//
// \author Peter Vanroose, K.U.Leuven, ESAT/PSI
// \date   12 Sept. 2000

#include <vepl/vepl_threshold.h>
//#include <vil/vil_rgb_byte.h>
#include "test_driver.h"

#define ALL_TESTS(x,v,m) \
  ONE_TEST(x,byte_img,byte_ori,unsigned char,v,m+"_byte"); \
  ONE_TEST(x,shrt_img,shrt_ori,unsigned short,v,m+"_short"); \
/*ONE_TEST(x,flot_img,flot_ori,float,v,m+"_float"); */ \
/*ONE_TEST(x,colr_img,colr_ori,vil_rgb_byte,v,m+"_colour") */

int vepl_test_threshold() {
  vil_image byte_img = CreateTest8bitImage(32,32),  byte_ori = CreateTest8bitImage(32,32);
  vil_image shrt_img = CreateTest16bitImage(32,32), shrt_ori = CreateTest16bitImage(32,32);
//vil_image colr_img = CreateTest24bitImage(32,32), colr_ori = CreateTest24bitImage(32,32);
//vil_image flot_img = CreateTestfloatImage(32,32), flot_ori = CreateTestfloatImage(32,32);

  vcl_string m = "vepl_threshold";
  ALL_TESTS(vepl_threshold,0,m);
  return 0;
}

TESTMAIN(vepl_test_threshold);
