// This is vxl/vil/file_formats/vil_bmp_core_header.cxx
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma implementation
#endif
//:
// \file
// \author fsm

#include "vil_bmp_core_header.h"

#include <vcl_iostream.h>

#include <vil/vil_stream.h>
#include <vil/vil_16bit.h>
#include <vil/vil_32bit.h>

vil_bmp_core_header::vil_bmp_core_header()
{
  header_size = disk_size;
  width = 0;
  height = 0;
  planes = 1;
  bitsperpixel = 8;
}

void vil_bmp_core_header::read(vil_stream *s)
{
  header_size  = vil_32bit_read_little_endian(s);
  width        = vil_32bit_read_little_endian(s);
  height       = vil_32bit_read_little_endian(s);
  planes       = vil_16bit_read_little_endian(s);
  bitsperpixel = vil_16bit_read_little_endian(s);
  // allowed values for bitsperpixel are 1 4 8 16 24 32; currently we only support 8 and 24
}

void vil_bmp_core_header::write(vil_stream *s) const
{
  vil_32bit_write_little_endian(s, header_size);
  vil_32bit_write_little_endian(s, width);
  vil_32bit_write_little_endian(s, height);
  vil_16bit_write_little_endian(s, planes);
  vil_16bit_write_little_endian(s, bitsperpixel);
}

void vil_bmp_core_header::print(vcl_ostream &s) const
{
  s << "vil_bmp_core_header:\n"
    << "  header_size  : " << header_size  << vcl_endl
    << "  width        : " << width        << vcl_endl
    << "  height       : " << height       << vcl_endl
    << "  planes       : " << planes       << vcl_endl
    << "  bitsperpixel : " << bitsperpixel << vcl_endl << vcl_endl;
}
