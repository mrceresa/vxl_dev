#include <vcl_cstdlib.h>
#include <vcl_iostream.h>
#include <vcl_iomanip.h>
#include <vcl_cmath.h>
#include <vcl_ctime.h>

#include <vnl/vnl_test.h>
#include <vnl/algo/vnl_chi_squared.h>

//:
// \file
//
// Test function results for chi-squared cumulative density function.
// The values are those calculated from this function on solaris, and
// agree (to 3sf) with those from a mathematical table.

// Each row is for a different dof, from 1 through 96 in increments of
// 5 (20 rows).

// Each column is for a different chi-squared, from 0 though 180 in
// increments of 20 (10 columns).

// For readability reasons, the rows are split in 2x 5 rows, with an extra
// indentation for the second half of each row.

double cdf_baseline[] =
{
  0.0, 0.9999922488859869, 0.9999999997449120, 0.9999999999999905, 1.0,
     1.0, 1.0, 1.0, 1.0, 1.0,
  0.0, 0.9972306042844884, 0.9999995444850495, 0.9999999999549898, 0.9999999999999964,
     1.0, 1.0, 1.0, 1.0, 1.0,
  0.0, 0.9546593186683575, 0.9999642248743472, 0.9999999907278384, 0.9999999999985243,
     0.9999999999999998, 1.0, 1.0, 1.0, 1.0,
  0.0, 0.7797793533983011, 0.9992214099174926, 0.9999994766265833, 0.9999999998335990,
     0.9999999999999654, 1.0, 1.0, 1.0, 1.0,
  0.0, 0.4787387426184652, 0.9925632227010702, 0.9999872283117348, 0.9999999919251898,
     0.9999999999971140, 0.9999999999999993, 1.0, 1.0, 1.0,
  0.0, 0.2084435236051257, 0.9609880071450072, 0.9998323023504486, 0.9999997903577246,
     0.9999999998716507, 0.9999999999999505, 1.0, 1.0, 1.0,
  0.0, 0.0641963723612628, 0.8709596508694845, 0.9986502140214748, 0.9999966874714471,
     0.9999999965356298, 0.9999999999979206, 0.9999999999999991, 1.0, 1.0,
  0.0, 0.0142776135970496, 0.7029716020753263, 0.9927297837948102, 0.9999652953758086,
     0.9999999382046935, 0.9999999999423534, 0.9999999999999658, 1.0, 1.0,
  0.0, 0.0023551011658856, 0.4850483796984979, 0.9720590475678458, 0.9997436372936986,
     0.9999992259610407, 0.9999999988798406, 0.9999999999990349, 0.9999999999999994, 1.0,
  0.0, 0.0002957368080061, 0.2793886568739744, 0.9194309789050203, 0.9986010626587202,
     0.9999928735024521, 0.9999999840351245, 0.9999999999800138, 0.9999999999999838, 1.0,
  0.0, 0.0000289361257476, 0.1331225579748673, 0.8182400707526587, 0.9941486753062340,
     0.9999500186862800, 0.9999998271108139, 0.9999999996858998, 0.9999999999996467, 0.9999999999999998,
  0.0, 0.0000022535340508, 0.0524807132282665, 0.6671309159544766, 0.9806610209037141,
     0.9997251552759223, 0.9999985364764852, 0.9999999961469799, 0.9999999999940054, 0.9999999999999938,
  0.0, 0.0000001352148233, 0.0172069984403675, 0.4878147556882595, 0.9481646213453065,
     0.9987862811130113, 0.9999900872908165, 0.9999999622464486, 0.9999999999188310, 0.9999999999998879,
  0.0, 0.0000000073722077, 0.0047274255385307, 0.3154587502878544, 0.8846964150660444,
     0.9956070677677689, 0.9999452153818948, 0.9999996987525844, 0.999999999105923, 0.9999999999983556,
  0.0, 0.0, 0.0010974342908522, 0.1790828046260967, 0.7825800096649158,
     0.9867347062121498, 0.9997487615314837, 0.9999980101056154, 0.9999999918570519, 0.9999999999800666,
  0.0, 0.0000000000116404, 0.0002170786956947, 0.0890129923177431, 0.6453492639226499,
     0.9660451059031501, 0.9990298866149123, 0.9999889634183597, 0.9999999378147725, 0.9999999997975395,
  0.0, 0.0, 0.0000368857904673, 0.0387583535447800, 0.4894571445703249,
     0.9252463664134718, 0.9968048573119753, 0.9999479616312844, 0.9999995969280049, 0.9999999982561550,
  0.0, 0.0000000000000097, 0.0000054252284960, 0.0148195154453572, 0.3381826989258588,
     0.8564977678812210, 0.9909185509763581, 0.9997891030141784, 0.9999977585673097, 0.9999999871266845,
  0.0, 0.0, 0.0000006956282422, 0.0049930250351178, 0.2116235676678424,
     0.7565873548899793, 0.9774854404134016, 0.9992580809522048, 0.9999892041010620, 0.9999999177818010,
  0.0, 0.0000000000000002, 0.0000000782693028, 0.0014883025723802, 0.1195827092276646,
     0.6303318279959211, 0.9508233262755118, 0.9977140253664275, 0.9999545743054137, 0.9999995418724694
};

int test_functions()
{
  int n;
  double chisq;
  int idx = 0;
  for (n=1; n<100; n+=5)
  {
    for (chisq = 0; chisq < 200; chisq+=20)
    {
      double cdf = vnl_chi_squared_cumulative(chisq,n);
      double err = vcl_fabs(cdf - cdf_baseline[idx++]);
      vcl_cout << "vnl_chi_squared_cumulative(" << chisq << ',' << n << ')';
      vnl_test_assert(" CDF", err < 2e-15);
      if (err >= 2e-15)
        vcl_cout << "Error = " << vcl_setprecision(16) << err << vcl_endl;
    }
  }

  vcl_cout << "cdf(7.88,1) = " << vnl_chi_squared_cumulative(7.88,1)
           << " should be about 0.995 \n";
  vcl_cout << "cdf(14.8,12) = " << vnl_chi_squared_cumulative(14.8,12)
           << " should be about 0.75 \n";
  vcl_cout << "cdf(10.1,19) = " << vnl_chi_squared_cumulative(10.1,19)
           << " should be about 0.05 \n";
  vcl_cout << "cdf(39.3,40) = " << vnl_chi_squared_cumulative(39.3,40)
           << " should be about 0.50 \n";
  vcl_cout << "cdf(109.1,100) = " << vnl_chi_squared_cumulative(109.1,100)
           << " should be about 0.75 \n";

  // rand() is not always a good random number generator,
  // so use the following congruential random number generator - PVr
  static unsigned long sample_seed = vcl_time(0);

  double hist1[20];
  for (int i=0; i<20; i++)
  {
    sample_seed = (sample_seed*16807)%2147483647L;
    double u = double(sample_seed)/0x7fffffff; // 0x7fffffff == 2147483711L
    hist1[i] = 10.0+20.0*(u-0.5); // uniform in the interval 0 - 20
  }
  chisq = 0;
  for (int i=0; i<20; i++)
  {
    vcl_cout << i << " " << hist1[i] << vcl_endl;
    double delta = hist1[i] - 10.0;
    chisq += delta*delta/(hist1[i] + 10.0);
  }
  vcl_cout << "cdf(" << chisq << ",20) = "
           << vnl_chi_squared_cumulative(chisq,20)
           << " so P(same dist) = " << (1.0 - vnl_chi_squared_cumulative(chisq,20))
           << vcl_endl;
  return 0;
}

TESTMAIN(test_functions)
