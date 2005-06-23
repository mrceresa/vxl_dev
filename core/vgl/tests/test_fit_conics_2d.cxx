//:
// \file
// \author Joseph Mundy
// \date  March 28, 2003

#include <testlib/testlib_test.h>
#include <vcl_cmath.h>
#include <vcl_vector.h>
#include <vcl_iostream.h>
#include <vgl/vgl_vector_2d.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_conic.h>
#include <vgl/vgl_conic_segment_2d.h>
#include <vgl/algo/vgl_conic_2d_regression.h>
#include <vgl/algo/vgl_fit_conics_2d.h>
#if 0
static void test_conic_segment_methods()
{
  vgl_point_2d<double> p0(0.0,0.0), p1(1.0,2.0);
  vgl_conic<double> l(1.0,-1.0,0.0);
  vcl_cout << "line angle " << l.slope_degrees() << '\n';
  TEST_NEAR("line angle", l.slope_degrees(), 45, 1e-04);
  vgl_conic_segment_2d<double> seg(p0,p1);
  double angle = seg.slope_degrees();
  TEST_NEAR("line angle", seg.slope_degrees(),63.43495 , 1e-04);
  vgl_vector_2d<double> dir = seg.direction(), norm = seg.normal();
  vcl_cout << "segment angle = " << angle << '\n'
           << "direction " << dir << '\n'
           << "normal " << norm << '\n';
}
#endif

static void test_conic_regression()
{
  vcl_cout << "Testing conic regression (unit circle) \n";
  vgl_point_2d<double> p0(1.0, 0), p1(0.707, 0.707), p2(0.0, 1.0),
	  p3(-0.707, 0.707), p4(-1.0, 0.0), p5(-0.707, -0.707), p6(0.0, -1.0),
	  p7(0.707, -0.707);
  vgl_conic_2d_regression<double> reg;
  reg.add_point(p0);
  reg.add_point(p1);
  reg.add_point(p2);
  reg.add_point(p3);
  reg.add_point(p4);
  reg.add_point(p5);
  reg.add_point(p6);
  reg.add_point(p7);
 

  reg.fit();
  vcl_cout << "algebraic fitting error " << reg.get_rms_algebraic_error() << '\n';
  vcl_cout << "sampson fitting error " << reg.get_rms_sampson_error() << '\n'
           << "fitted conic " << reg.conic() << '\n';

  TEST_NEAR("unit circle", reg.get_rms_sampson_error(), 0.0, 1e-04);

   
  vgl_point_2d<double> q0(1.414, 1.414), q1(-0.707, 0.707),
    q2(-1.414, -1.414), q3(0.707, -0.707), q4(0, 1.26472), q5(0, -1.26472),
    q6(1.26472, 0), q7(-1.26472, 0);
  reg.clear_points();
  reg.add_point(q0);
  reg.add_point(q1);
  reg.add_point(q2);
  reg.add_point(q3);
  reg.add_point(q4);
  reg.add_point(q5);
  reg.add_point(q6);
  reg.add_point(q7);
  reg.fit();
  vcl_cout << "algebraic fitting error " << reg.get_rms_algebraic_error() << '\n';
  vcl_cout << "sampson fitting error " << reg.get_rms_sampson_error() << '\n'
           << "fitted conic " << reg.conic() << '\n';
  TEST_NEAR("2:1 at 45 deg", reg.get_rms_sampson_error(), 0.0, 1e-06);
  reg.clear_points();

  reg.add_point(vgl_point_2d<double>(3.52074e-012,0.0));
  reg.add_point(vgl_point_2d<double>(0.0151927,0.173651));
  reg.add_point(vgl_point_2d<double>(0.0603082,0.342022));
  reg.add_point(vgl_point_2d<double>(0.133976,0.500002));
  reg.add_point(vgl_point_2d<double>(0.233957,0.642789));
  reg.add_point(vgl_point_2d<double>(0.357214,0.766046));
  reg.add_point(vgl_point_2d<double>(0.500002,0.866026));
  reg.add_point(vgl_point_2d<double>(0.657981,0.939693));
  reg.add_point(vgl_point_2d<double>(0.826353,0.984808));
  reg.add_point(vgl_point_2d<double>(1,1));
  reg.add_point(vgl_point_2d<double>(1.17365,0.984808));
  reg.add_point(vgl_point_2d<double>(1.34202,0.939692));
  reg.add_point(vgl_point_2d<double>(1.5,0.866025));
  reg.add_point(vgl_point_2d<double>(1.64279,0.766044));
  reg.add_point(vgl_point_2d<double>(1.76604,0.642787));
  reg.add_point(vgl_point_2d<double>(1.86603,0.5));
  reg.add_point(vgl_point_2d<double>(1.93969,0.34202));
  reg.add_point(vgl_point_2d<double>(1.98481,0.173648));
  reg.add_point(vgl_point_2d<double>(2,0.0));
  reg.fit();
  vcl_cout << "algebraic fitting error " << reg.get_rms_algebraic_error() << '\n';
  vcl_cout << "sampson fitting error " << reg.get_rms_sampson_error() << '\n'
           << "fitted conic " << reg.conic() << '\n';
  TEST_NEAR("semi-circle centered on (1,0) ", reg.get_rms_sampson_error(), 0.0, 1e-05);
  reg.clear_points();

}

//A parametric form of the unit circle for testing ( s = 10 degree intervals)
static void unit_circle(double s, double x0, double y0, double& x, double& y)
{
  double theta = static_cast<double>(s)*3.14159/18;
  double c = vcl_cos(theta), si = vcl_sin(theta);
  x = x0 + c; y = y0 + si;
}

static void test_fit_simple_chain()
{

  vcl_vector<vgl_point_2d<double> > curve;

  // Two segments from a unit circle forming a kind of sine wave

  //first segment above horizontal axis centered at (1,0) scan -pi to 0
  vgl_fit_conics_2d<double> fitter;
  for (int i = 18; i>=0; --i)
    {
      double x, y;
      unit_circle(i, 1.0, 0.0, x, y);
      vgl_point_2d<double> p(x, y);
      fitter.add_point(p);
    }
  //Second segment below horizontal axis centered at (3,0) scan pi to 360
  
  for (int i = 19; i<=36; ++i)
    {
      double x, y;
      unit_circle(i, 3.0, 0.0, x, y);
      vgl_point_2d<double> p(x, y);
      fitter.add_point(p);
    }

  fitter.fit();
  vcl_vector<vgl_conic_segment_2d<double> >& segs = fitter.get_conic_segs();
  vcl_cout << "\nCurve fit Produced the following conic segments\n";
  for (vcl_vector<vgl_conic_segment_2d<double> >::iterator sit = segs.begin();
       sit != segs.end(); sit++)
    vcl_cout << *sit << '\n';

  TEST("Number of conic segments ", segs.size(), 2);

  //Test a real chain
  vgl_fit_conics_2d<double> f(10, 1);
  
  f.add_point(vgl_point_2d<double>(149.032, 103.331));
  f.add_point(vgl_point_2d<double>(150.013, 103.277));
  f.add_point(vgl_point_2d<double>(151.004, 103.262));
  f.add_point(vgl_point_2d<double>(152.008, 103.251));
  f.add_point(vgl_point_2d<double>(153.016, 103.205));
  f.add_point(vgl_point_2d<double>(154.013, 103.12));
  f.add_point(vgl_point_2d<double>(155.003, 103.04));
  f.add_point(vgl_point_2d<double>(156, 103.001));
  f.add_point(vgl_point_2d<double>(157, 102.992));
  f.add_point(vgl_point_2d<double>(157.999, 102.978));
  f.add_point(vgl_point_2d<double>(158.992, 102.921));
  f.add_point(vgl_point_2d<double>(159.984, 102.832));
  f.add_point(vgl_point_2d<double>(160.99, 102.77));
  f.add_point(vgl_point_2d<double>(161.998, 102.755));
  f.add_point(vgl_point_2d<double>(163, 102.754));
  f.add_point(vgl_point_2d<double>(164, 102.754));
  f.add_point(vgl_point_2d<double>(165, 102.754));
  f.add_point(vgl_point_2d<double>(166, 102.754));
  f.add_point(vgl_point_2d<double>(167, 102.754));
  f.add_point(vgl_point_2d<double>(168, 102.754));
  f.add_point(vgl_point_2d<double>(169, 102.754));
  f.add_point(vgl_point_2d<double>(170, 102.754));
  f.add_point(vgl_point_2d<double>(171, 102.754));
  f.add_point(vgl_point_2d<double>(172, 102.754));
  f.add_point(vgl_point_2d<double>(173, 102.754));
  f.add_point(vgl_point_2d<double>(174, 102.754));
  f.add_point(vgl_point_2d<double>(175, 102.754));
  f.add_point(vgl_point_2d<double>(176, 102.754));
  f.add_point(vgl_point_2d<double>(177, 102.754));
  f.add_point(vgl_point_2d<double>(178, 102.754));
  f.add_point(vgl_point_2d<double>(179.002, 102.754));
  f.add_point(vgl_point_2d<double>(180.008, 102.766));
  f.add_point(vgl_point_2d<double>(181.015, 102.817));
  f.add_point(vgl_point_2d<double>(182.01, 102.9));
  f.add_point(vgl_point_2d<double>(183.002, 102.965));
  f.add_point(vgl_point_2d<double>(184, 102.989));
  f.add_point(vgl_point_2d<double>(185, 103));
  f.add_point(vgl_point_2d<double>(185.998, 103.033));
  f.add_point(vgl_point_2d<double>(186.988, 103.11));
  f.add_point(vgl_point_2d<double>(187.983, 103.198));
  f.add_point(vgl_point_2d<double>(188.991, 103.249));
  f.add_point(vgl_point_2d<double>(189.996, 103.261));
  f.add_point(vgl_point_2d<double>(190.988, 103.274));
  f.add_point(vgl_point_2d<double>(191.969, 103.326));
  f.add_point(vgl_point_2d<double>(192.958, 103.416));
  f.add_point(vgl_point_2d<double>(193.964, 103.497));
  f.add_point(vgl_point_2d<double>(194.037, 103.506));
  f.add_point(vgl_point_2d<double>(195.044, 103.59));
  f.add_point(vgl_point_2d<double>(196.034, 103.688));
  f.add_point(vgl_point_2d<double>(197.022, 103.758));
  f.add_point(vgl_point_2d<double>(198.018, 103.831));
  f.add_point(vgl_point_2d<double>(199.008, 103.923));
  f.add_point(vgl_point_2d<double>(200, 103.995));
  f.add_point(vgl_point_2d<double>(200.992, 104.072));
  f.add_point(vgl_point_2d<double>(201.978, 104.176));
  f.add_point(vgl_point_2d<double>(202.971, 104.267));
  f.add_point(vgl_point_2d<double>(203.959, 104.353));
  f.add_point(vgl_point_2d<double>(204.943, 104.464));
  f.add_point(vgl_point_2d<double>(206.052, 104.615));
  f.add_point(vgl_point_2d<double>(207.038, 104.729));
  f.add_point(vgl_point_2d<double>(208.024, 104.839));
  f.add_point(vgl_point_2d<double>(209.007, 104.953));
  f.add_point(vgl_point_2d<double>(209.989, 105.069));
  f.add_point(vgl_point_2d<double>(210.971, 105.187));
  f.add_point(vgl_point_2d<double>(211.953, 105.305));
  f.add_point(vgl_point_2d<double>(212.935, 105.44));
  f.add_point(vgl_point_2d<double>(214.061, 105.625));
  f.add_point(vgl_point_2d<double>(215.042, 105.769));
  f.add_point(vgl_point_2d<double>(216.017, 105.909));
  f.add_point(vgl_point_2d<double>(216.99, 106.051));
  f.add_point(vgl_point_2d<double>(217.961, 106.201));
  f.add_point(vgl_point_2d<double>(218.939, 106.35));
  f.add_point(vgl_point_2d<double>(220.076, 106.555));
  f.add_point(vgl_point_2d<double>(221.053, 106.721));
  f.add_point(vgl_point_2d<double>(222.027, 106.87));
  f.add_point(vgl_point_2d<double>(222.993, 107.028));
  f.add_point(vgl_point_2d<double>(223.953, 107.2));
  f.add_point(vgl_point_2d<double>(224.921, 107.388));
  f.add_point(vgl_point_2d<double>(226.074, 107.641));
  f.add_point(vgl_point_2d<double>(227.042, 107.822));
  f.add_point(vgl_point_2d<double>(228.001, 107.996));
  f.add_point(vgl_point_2d<double>(228.96, 108.17));
  f.add_point(vgl_point_2d<double>(229.926, 108.349));
  f.add_point(vgl_point_2d<double>(231.085, 108.623));
  f.add_point(vgl_point_2d<double>(232.046, 108.827));
  f.add_point(vgl_point_2d<double>(232.994, 109.02));
  f.add_point(vgl_point_2d<double>(233.939, 109.218));
  f.add_point(vgl_point_2d<double>(234.883, 109.459));
  f.add_point(vgl_point_2d<double>(235.115, 109.551));
  f.add_point(vgl_point_2d<double>(236.059, 109.791));
  f.add_point(vgl_point_2d<double>(237.003, 109.991));
  f.add_point(vgl_point_2d<double>(237.946, 110.189));
  f.add_point(vgl_point_2d<double>(238.887, 110.424));
  f.add_point(vgl_point_2d<double>(239.127, 110.519));
  f.add_point(vgl_point_2d<double>(240.066, 110.782));
  f.add_point(vgl_point_2d<double>(240.999, 111.004));
  f.add_point(vgl_point_2d<double>(241.922, 111.233));
  f.add_point(vgl_point_2d<double>(243.108, 111.659));
  f.add_point(vgl_point_2d<double>(244.032, 111.905));
  f.add_point(vgl_point_2d<double>(244.959, 112.125));
  f.add_point(vgl_point_2d<double>(245.882, 112.377));
  f.add_point(vgl_point_2d<double>(247.069, 112.807));
  f.add_point(vgl_point_2d<double>(247.981, 113.05));
  f.add_point(vgl_point_2d<double>(248.879, 113.321));
  f.add_point(vgl_point_2d<double>(250.07, 113.824));
  f.add_point(vgl_point_2d<double>(250.969, 114.076));
  f.add_point(vgl_point_2d<double>(251.86, 114.367));
  f.add_point(vgl_point_2d<double>(252.172, 114.549));
  f.add_point(vgl_point_2d<double>(253.053, 114.873));
  f.add_point(vgl_point_2d<double>(253.944, 115.133));
  f.add_point(vgl_point_2d<double>(254.798, 115.477));
  f.add_point(vgl_point_2d<double>(255.136, 115.691));
  f.add_point(vgl_point_2d<double>(256.003, 115.993));
  f.add_point(vgl_point_2d<double>(256.865, 116.292));
  f.add_point(vgl_point_2d<double>(257.215, 116.529));
  f.add_point(vgl_point_2d<double>(258.05, 116.896));
  f.add_point(vgl_point_2d<double>(258.914, 117.18));
  f.add_point(vgl_point_2d<double>(260.095, 117.806));
  f.add_point(vgl_point_2d<double>(260.946, 118.103));
  f.add_point(vgl_point_2d<double>(262.114, 118.798));
  f.add_point(vgl_point_2d<double>(262.936, 119.111));
  f.add_point(vgl_point_2d<double>(264.094, 119.836));
  f.add_point(vgl_point_2d<double>(264.915, 120.149));
  f.add_point(vgl_point_2d<double>(266.067, 120.889));
  f.add_point(vgl_point_2d<double>(266.859, 121.217));
  f.add_point(vgl_point_2d<double>(267.25, 121.629));
  f.add_point(vgl_point_2d<double>(267.987, 122.018));
  f.add_point(vgl_point_2d<double>(268.692, 122.44));
  f.add_point(vgl_point_2d<double>(269.103, 122.854));
  f.add_point(vgl_point_2d<double>(269.861, 123.196));
  f.add_point(vgl_point_2d<double>(270.249, 123.66));
  f.add_point(vgl_point_2d<double>(270.961, 124.049));
  f.add_point(vgl_point_2d<double>(272.029, 124.967));
  f.add_point(vgl_point_2d<double>(272.663, 125.379));
  f.add_point(vgl_point_2d<double>(273.075, 125.918));
  f.add_point(vgl_point_2d<double>(273.724, 126.299));
  f.add_point(vgl_point_2d<double>(274.104, 126.893));
  f.add_point(vgl_point_2d<double>(274.725, 127.268));
  f.add_point(vgl_point_2d<double>(275.098, 127.911));
  f.add_point(vgl_point_2d<double>(275.642, 128.309));
  f.add_point(vgl_point_2d<double>(276.043, 128.964));
  f.add_point(vgl_point_2d<double>(276.934, 130.049));
  f.add_point(vgl_point_2d<double>(277.297, 130.81));
  f.add_point(vgl_point_2d<double>(277.688, 131.185));
  f.add_point(vgl_point_2d<double>(278.025, 131.986));
  f.add_point(vgl_point_2d<double>(278.382, 132.817));
  f.add_point(vgl_point_2d<double>(278.637, 133.162));
  f.add_point(vgl_point_2d<double>(278.92, 134.029));
  f.add_point(vgl_point_2d<double>(279.132, 134.966));
  f.add_point(vgl_point_2d<double>(279.306, 135.945));
  f.add_point(vgl_point_2d<double>(279.449, 136.96));
  f.add_point(vgl_point_2d<double>(279.483, 138.018));
  f.add_point(vgl_point_2d<double>(279.373, 139.054));
  f.add_point(vgl_point_2d<double>(279.206, 140.048));
  f.add_point(vgl_point_2d<double>(278.997, 140.999));
  f.add_point(vgl_point_2d<double>(278.746, 141.894));
  f.add_point(vgl_point_2d<double>(278.146, 143.073));
  f.add_point(vgl_point_2d<double>(277.842, 143.91));
  f.add_point(vgl_point_2d<double>(277.063, 145.045));
  f.add_point(vgl_point_2d<double>(276.702, 145.767));
  f.add_point(vgl_point_2d<double>(276.18, 146.149));
  f.add_point(vgl_point_2d<double>(275.828, 146.852));
  f.add_point(vgl_point_2d<double>(275.253, 147.226));
  f.add_point(vgl_point_2d<double>(274.883, 147.889));
  f.add_point(vgl_point_2d<double>(274.266, 148.268));
  f.add_point(vgl_point_2d<double>(273.888, 148.882));
  f.add_point(vgl_point_2d<double>(273.231, 149.252));
  f.add_point(vgl_point_2d<double>(272.857, 149.841));
  f.add_point(vgl_point_2d<double>(272.173, 150.197));
  f.add_point(vgl_point_2d<double>(271.798, 150.762));
  f.add_point(vgl_point_2d<double>(271.095, 151.118));
  f.add_point(vgl_point_2d<double>(270.687, 151.591));
  f.add_point(vgl_point_2d<double>(270.004, 152.005));
  f.add_point(vgl_point_2d<double>(269.304, 152.431));
  f.add_point(vgl_point_2d<double>(268.895, 152.849));
  f.add_point(vgl_point_2d<double>(268.128, 153.187));
  f.add_point(vgl_point_2d<double>(267.733, 153.601));
  f.add_point(vgl_point_2d<double>(266.995, 153.992));
  f.add_point(vgl_point_2d<double>(266.226, 154.371));
  f.add_point(vgl_point_2d<double>(265.846, 154.742));
  f.add_point(vgl_point_2d<double>(265.042, 155.072));
  f.add_point(vgl_point_2d<double>(264.28, 155.493));
  f.add_point(vgl_point_2d<double>(263.886, 155.801));
  f.add_point(vgl_point_2d<double>(263.065, 156.115));
  f.add_point(vgl_point_2d<double>(261.904, 156.825));
  f.add_point(vgl_point_2d<double>(261.067, 157.127));
  f.add_point(vgl_point_2d<double>(259.892, 157.778));
  f.add_point(vgl_point_2d<double>(259.034, 158.07));
  f.add_point(vgl_point_2d<double>(258.192, 158.416));
  f.add_point(vgl_point_2d<double>(257.846, 158.667));
  f.add_point(vgl_point_2d<double>(256.99, 158.98));
  f.add_point(vgl_point_2d<double>(256.127, 159.277));
  f.add_point(vgl_point_2d<double>(254.939, 159.865));
  f.add_point(vgl_point_2d<double>(254.056, 160.131));
  f.add_point(vgl_point_2d<double>(253.18, 160.467));
  f.add_point(vgl_point_2d<double>(252.865, 160.652));
  f.add_point(vgl_point_2d<double>(251.975, 160.939));
  f.add_point(vgl_point_2d<double>(251.079, 161.197));
  f.add_point(vgl_point_2d<double>(249.889, 161.713));
  f.add_point(vgl_point_2d<double>(248.992, 161.981));
  f.add_point(vgl_point_2d<double>(248.085, 162.234));
  f.add_point(vgl_point_2d<double>(246.897, 162.69));
  f.add_point(vgl_point_2d<double>(245.978, 162.938));
  f.add_point(vgl_point_2d<double>(245.058, 163.167));
  f.add_point(vgl_point_2d<double>(244.142, 163.452));
  f.add_point(vgl_point_2d<double>(243.87, 163.584));
  f.add_point(vgl_point_2d<double>(242.95, 163.85));
  f.add_point(vgl_point_2d<double>(242.024, 164.071));
  f.add_point(vgl_point_2d<double>(241.096, 164.309));
  f.add_point(vgl_point_2d<double>(239.913, 164.707));
  f.add_point(vgl_point_2d<double>(238.977, 164.924));
  f.add_point(vgl_point_2d<double>(238.035, 165.123));
  f.add_point(vgl_point_2d<double>(237.088, 165.335));
  f.add_point(vgl_point_2d<double>(235.914, 165.679));
  f.add_point(vgl_point_2d<double>(234.969, 165.89));
  f.add_point(vgl_point_2d<double>(234.023, 166.083));
  f.add_point(vgl_point_2d<double>(233.077, 166.287));
  f.add_point(vgl_point_2d<double>(231.907, 166.615));
  f.add_point(vgl_point_2d<double>(230.955, 166.818));
  f.add_point(vgl_point_2d<double>(229.999, 166.994));
  f.add_point(vgl_point_2d<double>(229.038, 167.164));
  f.add_point(vgl_point_2d<double>(228.072, 167.34));
  f.add_point(vgl_point_2d<double>(226.916, 167.605));
  f.add_point(vgl_point_2d<double>(225.951, 167.799));
  f.add_point(vgl_point_2d<double>(224.994, 167.975));
  f.add_point(vgl_point_2d<double>(224.031, 168.141));
  f.add_point(vgl_point_2d<double>(223.061, 168.303));
  f.add_point(vgl_point_2d<double>(222.089, 168.49));
  f.add_point(vgl_point_2d<double>(221.915, 168.537));
  f.add_point(vgl_point_2d<double>(220.945, 168.711));
  f.add_point(vgl_point_2d<double>(219.972, 168.857));
  f.add_point(vgl_point_2d<double>(219, 169.002));
  f.add_point(vgl_point_2d<double>(218.028, 169.147));
  f.add_point(vgl_point_2d<double>(217.053, 169.288));
  f.add_point(vgl_point_2d<double>(216.074, 169.448));
  f.add_point(vgl_point_2d<double>(214.939, 169.651));
  f.add_point(vgl_point_2d<double>(213.962, 169.799));
  f.add_point(vgl_point_2d<double>(212.989, 169.937));
  f.add_point(vgl_point_2d<double>(212.009, 170.059));
  f.add_point(vgl_point_2d<double>(211.026, 170.175));
  f.add_point(vgl_point_2d<double>(210.041, 170.286));
  f.add_point(vgl_point_2d<double>(209.054, 170.405));
  f.add_point(vgl_point_2d<double>(207.944, 170.556));
  f.add_point(vgl_point_2d<double>(206.955, 170.675));
  f.add_point(vgl_point_2d<double>(205.969, 170.785));
  f.add_point(vgl_point_2d<double>(204.985, 170.897));
  f.add_point(vgl_point_2d<double>(204.001, 171.008));
  f.add_point(vgl_point_2d<double>(203.014, 171.112));
  f.add_point(vgl_point_2d<double>(202.021, 171.202));
  f.add_point(vgl_point_2d<double>(201.028, 171.277));
  f.add_point(vgl_point_2d<double>(200.039, 171.361));
  f.add_point(vgl_point_2d<double>(199.041, 171.453));
  f.add_point(vgl_point_2d<double>(197.96, 171.546));
  f.add_point(vgl_point_2d<double>(196.962, 171.634));
  f.add_point(vgl_point_2d<double>(195.972, 171.714));
  f.add_point(vgl_point_2d<double>(194.978, 171.79));
  f.add_point(vgl_point_2d<double>(193.987, 171.88));
  f.add_point(vgl_point_2d<double>(192.997, 171.96));
  f.add_point(vgl_point_2d<double>(192, 172.004));
  f.add_point(vgl_point_2d<double>(191.003, 172.041));
  f.add_point(vgl_point_2d<double>(190.011, 172.11));
  f.add_point(vgl_point_2d<double>(189.015, 172.19));
  f.add_point(vgl_point_2d<double>(188.008, 172.236));
  f.add_point(vgl_point_2d<double>(187.004, 172.247));
  f.add_point(vgl_point_2d<double>(186.012, 172.261));
  f.add_point(vgl_point_2d<double>(185.032, 172.315));
  f.add_point(vgl_point_2d<double>(184.041, 172.41));
  f.add_point(vgl_point_2d<double>(183.022, 172.481));
  f.add_point(vgl_point_2d<double>(182.005, 172.499));
  f.add_point(vgl_point_2d<double>(181, 172.5));
  f.add_point(vgl_point_2d<double>(180, 172.5));
  f.add_point(vgl_point_2d<double>(179, 172.5));
  f.add_point(vgl_point_2d<double>(178, 172.5));
  f.add_point(vgl_point_2d<double>(176.996, 172.501));
  f.add_point(vgl_point_2d<double>(175.98, 172.516));
  f.add_point(vgl_point_2d<double>(174.961, 172.58));
  f.add_point(vgl_point_2d<double>(173.968, 172.669));
  f.add_point(vgl_point_2d<double>(172.987, 172.723));
  f.add_point(vgl_point_2d<double>(171.997, 172.738));
  f.add_point(vgl_point_2d<double>(171, 172.739));
  f.add_point(vgl_point_2d<double>(170.001, 172.739));
  f.add_point(vgl_point_2d<double>(169.005, 172.735));
  f.add_point(vgl_point_2d<double>(168.017, 172.71));
  f.add_point(vgl_point_2d<double>(167.034, 172.648));
  f.add_point(vgl_point_2d<double>(166.035, 172.564));
  f.add_point(vgl_point_2d<double>(165.016, 172.512));
  f.add_point(vgl_point_2d<double>(164.003, 172.501));
  f.add_point(vgl_point_2d<double>(163, 172.5));
  f.add_point(vgl_point_2d<double>(162, 172.5));
  f.add_point(vgl_point_2d<double>(161, 172.5));
  f.add_point(vgl_point_2d<double>(159.998, 172.5));
  f.add_point(vgl_point_2d<double>(158.99, 172.494));
  f.add_point(vgl_point_2d<double>(157.971, 172.462));
  f.add_point(vgl_point_2d<double>(156.961, 172.384));
  f.add_point(vgl_point_2d<double>(155.974, 172.301));
  f.add_point(vgl_point_2d<double>(154.991, 172.257));
  f.add_point(vgl_point_2d<double>(153.996, 172.246));
  f.add_point(vgl_point_2d<double>(152.99, 172.231));
  f.add_point(vgl_point_2d<double>(151.984, 172.172));
  f.add_point(vgl_point_2d<double>(150.992, 172.084));
  f.add_point(vgl_point_2d<double>(149.999, 172.024));
  f.add_point(vgl_point_2d<double>(149, 171.998));
  f.add_point(vgl_point_2d<double>(148.004, 171.95));
  f.add_point(vgl_point_2d<double>(147.015, 171.864));
  f.add_point(vgl_point_2d<double>(146.022, 171.777));
  f.add_point(vgl_point_2d<double>(145.029, 171.705));
  f.add_point(vgl_point_2d<double>(144.039, 171.622));
  f.add_point(vgl_point_2d<double>(143.04, 171.536));
  f.add_point(vgl_point_2d<double>(141.957, 171.439));
  f.add_point(vgl_point_2d<double>(140.962, 171.345));
  f.add_point(vgl_point_2d<double>(139.974, 171.266));
  f.add_point(vgl_point_2d<double>(138.98, 171.193));
  f.add_point(vgl_point_2d<double>(137.987, 171.097));
  f.add_point(vgl_point_2d<double>(137.002, 170.985));
  f.add_point(vgl_point_2d<double>(136.02, 170.867));
  f.add_point(vgl_point_2d<double>(135.036, 170.752));
  f.add_point(vgl_point_2d<double>(134.049, 170.64));
  f.add_point(vgl_point_2d<double>(133.058, 170.517));
  f.add_point(vgl_point_2d<double>(132.941, 170.496));
  f.add_point(vgl_point_2d<double>(131.95, 170.37));
  f.add_point(vgl_point_2d<double>(130.963, 170.257));
  f.add_point(vgl_point_2d<double>(129.978, 170.147));
  f.add_point(vgl_point_2d<double>(128.995, 170.03));
  f.add_point(vgl_point_2d<double>(128.017, 169.906));
  f.add_point(vgl_point_2d<double>(127.043, 169.769));
  f.add_point(vgl_point_2d<double>(126.066, 169.617));
  f.add_point(vgl_point_2d<double>(124.929, 169.408));
  f.add_point(vgl_point_2d<double>(123.953, 169.252));
  f.add_point(vgl_point_2d<double>(122.979, 169.113));
  f.add_point(vgl_point_2d<double>(122.005, 168.976));
  f.add_point(vgl_point_2d<double>(121.033, 168.831));
  f.add_point(vgl_point_2d<double>(120.059, 168.681));
  f.add_point(vgl_point_2d<double>(119.087, 168.506));
  f.add_point(vgl_point_2d<double>(118.915, 168.459));
  f.add_point(vgl_point_2d<double>(117.941, 168.272));
  f.add_point(vgl_point_2d<double>(116.976, 168.104));
  f.add_point(vgl_point_2d<double>(116.015, 167.938));
  f.add_point(vgl_point_2d<double>(115.057, 167.763));
  f.add_point(vgl_point_2d<double>(114.093, 167.56));
  f.add_point(vgl_point_2d<double>(112.936, 167.301));
  f.add_point(vgl_point_2d<double>(111.969, 167.134));
  f.add_point(vgl_point_2d<double>(111.009, 166.963));
  f.add_point(vgl_point_2d<double>(110.057, 166.781));
  f.add_point(vgl_point_2d<double>(109.107, 166.559));
  f.add_point(vgl_point_2d<double>(108.885, 166.478));
  f.add_point(vgl_point_2d<double>(107.935, 166.241));
  f.add_point(vgl_point_2d<double>(106.988, 166.043));
  f.add_point(vgl_point_2d<double>(106.042, 165.85));
  f.add_point(vgl_point_2d<double>(105.096, 165.633));
  f.add_point(vgl_point_2d<double>(103.922, 165.289));
  f.add_point(vgl_point_2d<double>(102.977, 165.083));
  f.add_point(vgl_point_2d<double>(102.033, 164.887));
  f.add_point(vgl_point_2d<double>(101.096, 164.667));
  f.add_point(vgl_point_2d<double>(99.9141, 164.268));
  f.add_point(vgl_point_2d<double>(98.9892, 164.031));
  f.add_point(vgl_point_2d<double>(98.0693, 163.799));
  f.add_point(vgl_point_2d<double>(96.883, 163.376));
  f.add_point(vgl_point_2d<double>(95.9592, 163.124));
  f.add_point(vgl_point_2d<double>(95.0332, 162.902));
  f.add_point(vgl_point_2d<double>(94.1199, 162.646));
  f.add_point(vgl_point_2d<double>(92.9313, 162.181));
  f.add_point(vgl_point_2d<double>(92.0271, 161.931));
  f.add_point(vgl_point_2d<double>(91.1347, 161.648));
  f.add_point(vgl_point_2d<double>(90.8222, 161.469));
  f.add_point(vgl_point_2d<double>(89.9427, 161.141));
  f.add_point(vgl_point_2d<double>(89.0457, 160.887));
  f.add_point(vgl_point_2d<double>(88.1631, 160.576));
  f.add_point(vgl_point_2d<double>(87.8489, 160.385));
  f.add_point(vgl_point_2d<double>(86.9667, 160.077));
  f.add_point(vgl_point_2d<double>(86.0851, 159.808));
  f.add_point(vgl_point_2d<double>(84.9005, 159.214));
  f.add_point(vgl_point_2d<double>(84.0347, 158.928));
  f.add_point(vgl_point_2d<double>(83.1921, 158.583));
  f.add_point(vgl_point_2d<double>(82.846, 158.332));
  f.add_point(vgl_point_2d<double>(81.9921, 158.016));
  f.add_point(vgl_point_2d<double>(81.1414, 157.711));
  f.add_point(vgl_point_2d<double>(80.7813, 157.441));
  f.add_point(vgl_point_2d<double>(79.9653, 157.064));
  f.add_point(vgl_point_2d<double>(79.1363, 156.752));
  f.add_point(vgl_point_2d<double>(78.7595, 156.439));
  f.add_point(vgl_point_2d<double>(77.9707, 156.051));
  f.add_point(vgl_point_2d<double>(77.1579, 155.723));
  f.add_point(vgl_point_2d<double>(76.7839, 155.379));
  f.add_point(vgl_point_2d<double>(75.9932, 155.011));
  f.add_point(vgl_point_2d<double>(75.2072, 154.655));
  f.add_point(vgl_point_2d<double>(74.8302, 154.272));
  f.add_point(vgl_point_2d<double>(74.0497, 153.926));
  f.add_point(vgl_point_2d<double>(72.9285, 153.102));
  f.add_point(vgl_point_2d<double>(72.1714, 152.756));
  f.add_point(vgl_point_2d<double>(71.79, 152.299));
  f.add_point(vgl_point_2d<double>(71.0488, 151.933));
  f.add_point(vgl_point_2d<double>(69.9574, 151.053));
  f.add_point(vgl_point_2d<double>(69.281, 150.67));
  f.add_point(vgl_point_2d<double>(68.89, 150.125));
  f.add_point(vgl_point_2d<double>(68.2109, 149.765));
  f.add_point(vgl_point_2d<double>(67.8426, 149.173));
  f.add_point(vgl_point_2d<double>(67.1732, 148.815));
  f.add_point(vgl_point_2d<double>(66.8125, 148.189));
  f.add_point(vgl_point_2d<double>(66.1867, 147.825));
  f.add_point(vgl_point_2d<double>(65.8302, 147.147));
  f.add_point(vgl_point_2d<double>(65.2751, 146.773));
  f.add_point(vgl_point_2d<double>(64.9094, 146.073));
  f.add_point(vgl_point_2d<double>(64.4737, 145.64));
  f.add_point(vgl_point_2d<double>(64.0381, 144.973));
  f.add_point(vgl_point_2d<double>(63.6652, 144.202));
  f.add_point(vgl_point_2d<double>(63.3015, 143.829));
  f.add_point(vgl_point_2d<double>(62.9756, 143.013));
  f.add_point(vgl_point_2d<double>(62.6351, 142.16));
  f.add_point(vgl_point_2d<double>(62.4203, 141.833));
  f.add_point(vgl_point_2d<double>(62.1458, 140.955));
  f.add_point(vgl_point_2d<double>(61.9596, 140.009));
  f.add_point(vgl_point_2d<double>(61.8119, 139.028));
  f.add_point(vgl_point_2d<double>(61.7148, 138.012));
  f.add_point(vgl_point_2d<double>(61.7382, 136.979));
  f.add_point(vgl_point_2d<double>(61.8565, 135.974));
  f.add_point(vgl_point_2d<double>(62.0251, 135.007));
  f.add_point(vgl_point_2d<double>(62.2444, 134.087));
  f.add_point(vgl_point_2d<double>(62.7842, 132.902));
  f.add_point(vgl_point_2d<double>(63.0812, 132.042));
  f.add_point(vgl_point_2d<double>(63.4686, 131.271));
  f.add_point(vgl_point_2d<double>(63.8174, 130.882));
  f.add_point(vgl_point_2d<double>(64.1529, 130.112));
  f.add_point(vgl_point_2d<double>(64.624, 129.706));
  f.add_point(vgl_point_2d<double>(65.0317, 129.026));
  f.add_point(vgl_point_2d<double>(65.9686, 127.972));
  f.add_point(vgl_point_2d<double>(66.3932, 127.375));
  f.add_point(vgl_point_2d<double>(66.954, 126.953));
  f.add_point(vgl_point_2d<double>(67.3831, 126.417));
  f.add_point(vgl_point_2d<double>(67.9802, 125.978));
  f.add_point(vgl_point_2d<double>(69.0222, 125.025));
  f.add_point(vgl_point_2d<double>(69.6581, 124.597));
  f.add_point(vgl_point_2d<double>(70.0821, 124.102));
  f.add_point(vgl_point_2d<double>(70.8039, 123.739));
  f.add_point(vgl_point_2d<double>(71.1821, 123.252));
  f.add_point(vgl_point_2d<double>(71.9295, 122.901));
  f.add_point(vgl_point_2d<double>(73.0451, 122.065));
  f.add_point(vgl_point_2d<double>(73.8079, 121.71));
  f.add_point(vgl_point_2d<double>(74.1882, 121.292));
  f.add_point(vgl_point_2d<double>(74.9661, 120.946));
  f.add_point(vgl_point_2d<double>(75.7214, 120.522));
  f.add_point(vgl_point_2d<double>(76.1162, 120.201));
  f.add_point(vgl_point_2d<double>(76.9346, 119.885));
  f.add_point(vgl_point_2d<double>(78.0937, 119.166));
  f.add_point(vgl_point_2d<double>(78.9195, 118.856));
  f.add_point(vgl_point_2d<double>(80.0847, 118.16));
  f.add_point(vgl_point_2d<double>(80.9322, 117.865));
  f.add_point(vgl_point_2d<double>(82.1109, 117.232));
  f.add_point(vgl_point_2d<double>(82.9709, 116.94));
  f.add_point(vgl_point_2d<double>(83.8174, 116.602));
  f.add_point(vgl_point_2d<double>(84.1611, 116.353));
  f.add_point(vgl_point_2d<double>(85.0173, 116.037));
  f.add_point(vgl_point_2d<double>(85.8913, 115.752));
  f.add_point(vgl_point_2d<double>(87.0793, 115.19));
  f.add_point(vgl_point_2d<double>(87.971, 114.929));
  f.add_point(vgl_point_2d<double>(88.8597, 114.636));
  f.add_point(vgl_point_2d<double>(89.1734, 114.452));
  f.add_point(vgl_point_2d<double>(90.0528, 114.128));
  f.add_point(vgl_point_2d<double>(90.9476, 113.873));
  f.add_point(vgl_point_2d<double>(91.8318, 113.551));
  f.add_point(vgl_point_2d<double>(92.1394, 113.376));
  f.add_point(vgl_point_2d<double>(93.0355, 113.095));
  f.add_point(vgl_point_2d<double>(93.949, 112.858));
  f.add_point(vgl_point_2d<double>(94.8605, 112.574));
  f.add_point(vgl_point_2d<double>(95.1407, 112.433));
  f.add_point(vgl_point_2d<double>(96.053, 112.155));
  f.add_point(vgl_point_2d<double>(96.9765, 111.931));
  f.add_point(vgl_point_2d<double>(97.9023, 111.69));
  f.add_point(vgl_point_2d<double>(99.0873, 111.277));
  f.add_point(vgl_point_2d<double>(100.015, 111.047));
  f.add_point(vgl_point_2d<double>(100.946, 110.829));
  f.add_point(vgl_point_2d<double>(101.881, 110.573));
  f.add_point(vgl_point_2d<double>(102.127, 110.469));
  f.add_point(vgl_point_2d<double>(103.062, 110.22));
  f.add_point(vgl_point_2d<double>(104.006, 110.019));
  f.add_point(vgl_point_2d<double>(104.951, 109.823));
  f.add_point(vgl_point_2d<double>(105.898, 109.601));
  f.add_point(vgl_point_2d<double>(107.072, 109.261));
  f.add_point(vgl_point_2d<double>(108.017, 109.059));
  f.add_point(vgl_point_2d<double>(108.966, 108.871));
  f.add_point(vgl_point_2d<double>(109.925, 108.677));
  f.add_point(vgl_point_2d<double>(111.086, 108.396));
  f.add_point(vgl_point_2d<double>(112.049, 108.199));
  f.add_point(vgl_point_2d<double>(113.005, 108.019));
  f.add_point(vgl_point_2d<double>(113.966, 107.849));
  f.add_point(vgl_point_2d<double>(114.935, 107.683));
  f.add_point(vgl_point_2d<double>(116.09, 107.423));
  f.add_point(vgl_point_2d<double>(117.053, 107.222));
  f.add_point(vgl_point_2d<double>(118.012, 107.051));
  f.add_point(vgl_point_2d<double>(118.979, 106.898));
  f.add_point(vgl_point_2d<double>(119.952, 106.752));
  f.add_point(vgl_point_2d<double>(120.929, 106.592));
  f.add_point(vgl_point_2d<double>(122.066, 106.383));
  f.add_point(vgl_point_2d<double>(123.044, 106.229));
  f.add_point(vgl_point_2d<double>(124.017, 106.085));
  f.add_point(vgl_point_2d<double>(124.989, 105.946));
  f.add_point(vgl_point_2d<double>(125.962, 105.802));
  f.add_point(vgl_point_2d<double>(126.941, 105.655));
  f.add_point(vgl_point_2d<double>(128.068, 105.467));
  f.add_point(vgl_point_2d<double>(129.049, 105.329));
  f.add_point(vgl_point_2d<double>(130.032, 105.215));
  f.add_point(vgl_point_2d<double>(131.015, 105.1));
  f.add_point(vgl_point_2d<double>(131.997, 104.982));
  f.add_point(vgl_point_2d<double>(132.98, 104.867));
  f.add_point(vgl_point_2d<double>(133.965, 104.756));
  f.add_point(vgl_point_2d<double>(134.951, 104.644));
  f.add_point(vgl_point_2d<double>(135.942, 104.52));
  f.add_point(vgl_point_2d<double>(136.058, 104.5));
  f.add_point(vgl_point_2d<double>(137.047, 104.382));
  f.add_point(vgl_point_2d<double>(138.035, 104.283));
  f.add_point(vgl_point_2d<double>(139.024, 104.186));
  f.add_point(vgl_point_2d<double>(140.01, 104.082));
  f.add_point(vgl_point_2d<double>(141, 104.003));
  f.add_point(vgl_point_2d<double>(141.993, 103.932));
  f.add_point(vgl_point_2d<double>(142.983, 103.842));
  f.add_point(vgl_point_2d<double>(143.978, 103.767));
  f.add_point(vgl_point_2d<double>(144.967, 103.696));
  f.add_point(vgl_point_2d<double>(145.957, 103.6));
  f.add_point(vgl_point_2d<double>(146.962, 103.514));
  f.add_point(vgl_point_2d<double>(147.354, 103.646));
  f.add_point(vgl_point_2d<double>(148.042, 103.422));
  f.fit();
  vcl_vector<vgl_conic_segment_2d<double > > temp = f.get_conic_segs();

  for(vcl_vector<vgl_conic_segment_2d<double > >::iterator cit = temp.begin(); cit != temp.end(); ++cit)
    vcl_cout << *cit << '\n';;
  TEST("Number of conic segments ", temp.size(), 1);  
}

static void test_fit_conics_2d()
{
  //  test_conic_segment_methods();
  test_conic_regression();
  test_fit_simple_chain();
}

TESTMAIN(test_fit_conics_2d);
