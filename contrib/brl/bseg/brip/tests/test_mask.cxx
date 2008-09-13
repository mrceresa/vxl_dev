#include <testlib/testlib_test.h>
#include <vcl_iostream.h>
#include <vcl_iomanip.h>
#include <brip/brip_rect_mask.h>
static void test_mask()
{
  START ("test mask");
  
  brip_rect_mask m10(brip_rect_mask::r10);
  brip_rect_mask m30(brip_rect_mask::r30);
  brip_rect_mask m31(brip_rect_mask::r31);
  brip_rect_mask m32(brip_rect_mask::r32);
  brip_rect_mask m51(brip_rect_mask::r51);
  brip_rect_mask m52(brip_rect_mask::r52);
  brip_rect_mask c4_90_0(brip_rect_mask::c4_90_0);
  brip_rect_mask c4_45_0(brip_rect_mask::c4_45_0);
  brip_rect_mask c4_45_45(brip_rect_mask::c4_45_45);
  brip_rect_mask e5(brip_rect_mask::e5);
  vcl_cout << "m10\n" << m10 << '\n';
  vcl_cout << "m30\n" << m30 << '\n';
  vcl_cout << "m31\n" << m31 << '\n';
  vcl_cout << "m32\n" << m32 << '\n';
  vcl_cout << "m51\n" << m51 << '\n';
  vcl_cout << "m52\n" << m52 << '\n';
  vcl_cout << "c4_90_0\n" << c4_90_0 << '\n';
  vcl_cout << "c4_45_0\n" << c4_45_0 << '\n';
  vcl_cout << "c4_45_45\n" << c4_45_45 << '\n';
  vcl_cout << "e5\n" << e5 << '\n';
}

TESTMAIN(test_mask);
