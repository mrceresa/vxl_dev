#include <vcl_iostream.h>

#include <vpl/vpl.h>
#include <vul/vul_temp_filename.h>

#include <vil/vil_memory_image.h>
#include <vil/vil_save.h>

void p(vil_image const& m)
{
  for(int y = 0; y < m.height(); ++y) {
    for(int x = 0; x < m.width(); ++x) {
      unsigned char val = 0;
      m.get_section(&val, x, y, 1, 1);
      vcl_cout << "\t" << (int)val;
    }
    vcl_cout << vcl_endl;
  }
}

int main()
{
  vcl_string tmp_nam = vul_temp_filename();
  char const *file1 = tmp_nam!="" ? tmp_nam.c_str() : "vil_test_memory_image_1.pgm";
  tmp_nam = vul_temp_filename();
  char const *file2 = tmp_nam!="" ? tmp_nam.c_str() : "vil_test_memory_image_2.pgm";

  {
    vil_memory_image m(3, 2, VIL_BYTE);

    unsigned char data[] = {
      1, 2, 3,
      4, 5, 6
    };
    m.put_section(data, 0, 0, 3, 2);
    p(m);

    vil_save(m, file1, "pnm");
#ifdef LEAVE_IMAGES_BEHIND
      vpl_chmod(file1, 0666); // -rw-rw-rw-
#endif

    vil_memory_image n(data, 3, 2, VIL_BYTE);
    p(n);

    vil_save(n, file2, "pnm");
#ifdef LEAVE_IMAGES_BEHIND
      vpl_chmod(file2, 0666); // -rw-rw-rw-
#endif
  }

  // don't leave garbage behind by default.
#ifndef LEAVE_IMAGES_BEHIND
  vpl_unlink(file1);
  vpl_unlink(file2);
#endif
  return 0;
}
