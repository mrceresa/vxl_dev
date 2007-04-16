// This is brl/bbas/bgui/tests/test_histogram.cxx
#include <testlib/testlib_test.h>
#include <vil/vil_pixel_traits.h>
#include <vil/vil_load.h>
#include <vil/vil_image_view.h>
#include <vil/algo/vil_histogram.h>

// create a 16 bit test image
static vil_image_view<unsigned short> CreateTest16bitImage(int wd, int ht)
{
  vil_image_view<unsigned short> image(wd, ht);
  for (int j = 0; j < ht; j++)
    for (int i = 0; i < wd; i++)
      image(i,j) = ((i-wd/2)*(j-ht/2)/16) & 0xffff;
  return image;
}

static void test_histogram(int argc, char* argv[])
{
  vil_image_view<unsigned short> img;
  if (argc > 1) {
    vcl_cout << "Loading image " << argv[1] << " ... ";
    img = vil_load(argv[1]);
    if (!img) vcl_cout << "failed!!\n";
    else      vcl_cout << "done.\n";
  }
  if (!img)
    img = CreateTest16bitImage(1000,1000);

  vcl_cout << img.ni() << 'x' << img.nj() << '\n'
           << img(455,544) << '\n';
  vcl_vector<double> histogram;
  int nbins = 1<< vil_pixel_traits<unsigned short>::num_bits();
  vil_histogram(img,histogram, vil_pixel_traits<unsigned short>::minval(),
                vil_pixel_traits<unsigned short>::maxval(), nbins);

  for (int i =0; i<nbins; i+=200)
    vcl_cout << "H[" << i << "]=" << histogram[i] << '\n';

    vcl_cout << "short [" << vil_pixel_traits<short>::minval()
             << ' ' << vil_pixel_traits<short>::maxval()  << "]\n"

             << "unsigned short[" << vil_pixel_traits<unsigned short>::minval()
             << ' ' << vil_pixel_traits<unsigned short>::maxval()  << "]\n"

             << "int [" << vil_pixel_traits<int>::minval()
             << ' ' << vil_pixel_traits<int>::maxval()  << "]\n"

             << "unsigned int [" << vil_pixel_traits<unsigned int>::minval()
             << ' ' << vil_pixel_traits<unsigned int>::maxval()  << "]\n"

             << "long [" << vil_pixel_traits< long>::minval()
             << ' ' << vil_pixel_traits< long>::maxval()  << "]\n"

             << "unsigned  long [" << vil_pixel_traits<unsigned  long>::minval()
             << ' ' << vil_pixel_traits<unsigned  long>::maxval()  << "]\n"

             << "float [" << vil_pixel_traits< float>::minval()
             << ' ' << vil_pixel_traits< float>::maxval()  << "]\n"

             << "double [" << vil_pixel_traits< double>::minval()
             << ' ' << vil_pixel_traits< double>::maxval()  << "]\n"

             << "long double [" << vil_pixel_traits< long double>::minval()
             << ' ' << vil_pixel_traits< long double>::maxval()  << "]\n";
}

TESTMAIN_ARGS(test_histogram);
