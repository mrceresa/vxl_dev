#ifdef __GNUC__
#pragma implementation
#endif
#include "vil_file_image.h"
#include <vcl/vcl_iostream.h>

#include <vil/vil_load.h>

vil_file_image::vil_file_image(char const* filename, verbosity v VCL_DEFAULT_VALUE(verbose)):
  vil_image_inhibit_derivation(1)
{
  load(filename, v);
}

vil_file_image::vil_file_image(vcl_string const& filename, verbosity v VCL_DEFAULT_VALUE(verbose)):
  vil_image_inhibit_derivation(1)
{
  load(filename, v);
}

vil_file_image::vil_file_image():
  vil_image_inhibit_derivation(1)
{
}

bool vil_file_image::load(vcl_string const& filename, verbosity VCL_DEFAULT_VALUE(verbose))
{
  return this->load(filename.c_str());
}

bool vil_file_image::load(char const* filename, verbosity v VCL_DEFAULT_VALUE(verbose))
{
  vil_image i = vil_load(filename);
  if (v == verbose) {
    cerr << "vil_file_image: Loaded [" << filename << "]\n";
  }
  if (!i && v != silent) {
    cerr << "vil_file_image: Could not load [" << filename << "]\n";
  }
  
  vil_image::operator= (i);
  return operator bool ();
}
