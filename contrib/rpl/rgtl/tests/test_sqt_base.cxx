/* Copyright 2006-2009 Brad King, Chuck Stewart
   Distributed under the Boost Software License, Version 1.0.
   (See accompanying file rgtl_license_1_0.txt or copy at
   http://www.boost.org/LICENSE_1_0.txt) */
#include <testlib/testlib_test.h>

#include <rgtl/rgtl_sqt_base.hxx>
#include <vcl_cmath.h>

static double const test_directions[100][3] =
{{0.9409, 0.1315, -0.9139},
 {-0.0262, 0.2238, -0.2581},
 {0.6349, -0.7940, 0.3866},
 {0.2831, -0.6834, 0.8716},
 {-0.3873, -0.1727, -0.0448},
 {0.3219, 0.1208, -0.7418},
 {-0.2840, -0.4626, -0.0323},
 {0.8764, 0.5685, 0.8912},
 {-0.0247, -0.2243, -0.2645},
 {-0.8180, -0.9380, -0.3430},
 {0.3477, 0.1710, 0.5458},
 {0.0298, 0.1171, -0.4055},
 {-0.5568, -0.5986, -0.6443},
 {0.4500, -0.8252, 0.3816},
 {-0.8635, 0.8665, -0.4721},
 {0.9282, -0.4812, -0.0845},
 {-0.5847, -0.5917, 0.6874},
 {-0.6778, -0.9016, 0.7630},
 {0.2764, 0.2123, 0.4000},
 {-0.9995, 0.0927, 0.5114},
 {-0.3287, -0.8083, 0.9490},
 {-0.4498, 0.2740, -0.1957},
 {-0.9109, -0.1141, -0.7374},
 {-0.8122, -0.8672, 0.4495},
 {-0.1800, -0.2514, 0.7990},
 {0.6338, -0.5018, -0.6586},
 {0.7410, 0.8498, -0.9139},
 {-0.9549, 0.2590, -0.0417},
 {0.4544, 0.7566, -0.8121},
 {0.6960, 0.2833, 0.3001},
 {0.4572, 0.5968, 0.9046},
 {0.9102, -0.1299, -0.0846},
 {0.3127, 0.9623, 0.0738},
 {0.4846, -0.8081, -0.8670},
 {-0.3101, 0.0550, -0.0123},
 {0.7680, 0.0913, -0.1649},
 {-0.3055, -0.4313, -0.4155},
 {-0.8810, -0.2584, -0.4207},
 {0.4368, -0.8706, 0.5077},
 {0.9164, 0.0896, -0.8064},
 {-0.6863, 0.6728, -0.8462},
 {-0.1673, -0.7094, 0.4418},
 {-0.8119, -0.6570, 0.5298},
 {-0.1001, -0.8639, 0.3159},
 {0.7383, 0.6480, 0.6208},
 {-0.2168, -0.7321, -0.2515},
 {-0.4944, 0.7696, -0.3875},
 {-0.2912, 0.0295, -0.2586},
 {0.4860, 0.9273, 0.4135},
 {0.3017, -0.7590, -0.6633},
 {0.8796, -0.9034, 0.6274},
 {0.6656, -0.2397, -0.0675},
 {-0.0600, -0.1744, 0.4446},
 {0.2597, -0.1972, 0.9897},
 {-0.8836, -0.1580, -0.2750},
 {0.0844, -0.2461, 0.4617},
 {-0.0885, 0.8147, 0.2993},
 {0.7262, 0.3403, 0.3627},
 {0.7104, 0.9237, -0.9848},
 {-0.0555, -0.6740, 0.3083},
 {0.5738, 0.4973, 0.8905},
 {0.3120, -0.2519, 0.2265},
 {-0.9999, -0.0915, 0.5659},
 {-0.7375, -0.9229, -0.9937},
 {-0.0103, 0.1249, 0.5939},
 {-0.9233, -0.2554, 0.2836},
 {-0.5451, 0.5856, -0.6430},
 {-0.3442, 0.5905, 0.0588},
 {0.7989, -0.2342, -0.5625},
 {-0.3725, -0.4944, 0.0961},
 {-0.4966, -0.3141, -0.8835},
 {-0.1340, 0.9356, 0.1752},
 {0.6848, -0.0404, -0.1678},
 {-0.6310, -0.2633, -0.6271},
 {0.0164, 0.5291, -0.8722},
 {-0.0955, -0.2457, -0.8504},
 {-0.3488, 0.8006, -0.3799},
 {-0.2398, -0.6331, 0.8882},
 {0.7730, -0.2634, 0.9615},
 {0.5225, 0.8349, 0.1102},
 {0.7675, 0.0318, 0.9771},
 {-0.0852, -0.8194, 0.3831},
 {0.5984, 0.4706, -0.5167},
 {-0.7318, -0.9906, 0.6196},
 {-0.8694, 0.2062, 0.8690},
 {-0.2497, 0.9137, -0.7424},
 {-0.2530, -0.2051, 0.3737},
 {-0.0320, 0.4631, -0.4055},
 {0.9389, 0.3693, 0.2944},
 {-0.3159, 0.9570, -0.0725},
 {-0.4946, -0.5924, 0.8456},
 {0.1698, 0.1866, -0.5166},
 {0.0474, 0.9031, 0.3203},
 {-0.6732, -0.4795, -0.3354},
 {-0.0272, 0.0294, -0.0465},
 {-0.0079, 0.2727, -0.0625},
 {0.6864, -0.1981, 0.4119},
 {0.6124, -0.0268, -0.5201},
 {0.7156, 0.5009, 0.4344},
 {0.2195, -0.7476, 0.7304}};

static double dot_product(double const a[3], double const b[3])
{
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}

#if 0
static void cross_product(double const a[3], double const b[3], double c[3])
{
  c[0] = a[1]*b[2] - a[2]*b[1];
  c[1] = a[2]*b[0] - a[0]*b[2];
  c[2] = a[0]*b[1] - a[1]*b[0];
}
#endif

static void normalize(double d[3])
{
  double mag = vcl_sqrt(dot_product(d,d));
  if(mag > 0)
    {
    d[0] /= mag;
    d[1] /= mag;
    d[2] /= mag;
    }
}

static bool
test_sqt()
{
  int counts[6] = {15, 17, 18, 14, 18, 18};
  for(int i=0; i < 100; ++i)
    {
    rgtl_sqt_base::face_index_type face;
    double u;
    double v;
    double td[3] = {test_directions[i][0],
                    test_directions[i][1],
                    test_directions[i][2]};
    rgtl_sqt_base::direction_to_parameters(td, face, u, v);
    double d[3];
    rgtl_sqt_base::parameters_to_direction(face, u, v, d);
    --counts[face];
    normalize(td);
    normalize(d);
    if(dot_product(td, d) < 0.99999999)
      {
      return false;
      }
    double nu[3];
    double nv[3];
    rgtl_sqt_base::plane_normal_u(face, u, nu);
    rgtl_sqt_base::plane_normal_v(face, v, nv);
    if(dot_product(nu, d) > 1e-10)
      {
      return false;
      }
    if(dot_product(nv, d) > 1e-10)
      {
      return false;
      }
    }

  for(int i=0; i < 6; ++i)
    {
    if(counts[i] != 0)
      {
      return false;
      }
    }

  return true;
}

MAIN( test_sqt_base )
{
  START("rgtl_sqt_base");

  {
  testlib_test_begin("sqt parameter/direction conversion");
  testlib_test_perform(test_sqt());
  }

  SUMMARY();
}
