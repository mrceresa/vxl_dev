// This is tbl/vepl2/tests/vepl2_test_median.cxx

//:
// \file
//  Test of the vepl2_median function.
//
// \author Peter Vanroose, K.U.Leuven, ESAT/PSI
// \date   7 October 2002, from vepl/tests

#include <vepl2/vepl2_median.h>
#include <vcl_iostream.h>
#include <vcl_string.h>
#include "test_driver.h"

int vepl2_test_median() {
  vcl_cout << "Starting vepl2_median tests\n";
  vcl_cout << "Creating test and output images ...";
  vil2_image_view_base_sptr byte_img = CreateTest8bitImage(32,32),  byte_ori = CreateTest8bitImage(32,32);
  vil2_image_view_base_sptr shrt_img = CreateTest16bitImage(32,32), shrt_ori = CreateTest16bitImage(32,32);
  vil2_image_view_base_sptr flot_img = CreateTestfloatImage(32,32), flot_ori = CreateTestfloatImage(32,32);
  vcl_cout << " done\n";

  vcl_string m = "vepl2_median";
#define args ,5
  ONE_TEST(vepl2_median,byte_img,byte_ori,unsigned char,2632,m+"_byte",args);
  ONE_TEST(vepl2_median,shrt_img,shrt_ori,unsigned short,524872,m+"_short",args);
  ONE_TEST(vepl2_median,flot_img,flot_ori,float,51,m+"_float",args);

  return 0;
}

TESTMAIN(vepl2_test_median);
