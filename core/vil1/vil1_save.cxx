//-*- c++ -*-------------------------------------------------------------------
#ifdef __GNUC__
#pragma implementation "vil_save.h"
#endif
//
// Class: vil_save
// Author: Andrew W. Fitzgibbon, Oxford RRG
// Created: 16 Feb 00
// Modifications:
//   000216 AWF Initial version.
//
//-----------------------------------------------------------------------------

#include "vil_save.h"

#include <vcl/vcl_cstring.h>

#include <vil/vil_new.h>
#include <vil/vil_stream_fstream.h>
#include <vil/vil_image.h>
#include <vil/vil_copy.h>

//: Send vil_image to disk.
bool vil_save(vil_image const& i, char const* filename, char const* file_format VCL_DEFAULT_VALUE("pnm"))
{
  vil_stream_fstream* os = new vil_stream_fstream(filename, "w");
  return vil_save(i, os, file_format);
}

//: Send vil_image_impl to output stream.
// The possible file_formats are defined by the subclasses of vil_file_format
// in vil_file_format.cxx
bool vil_save(vil_image const& i, vil_stream* os, char const* file_format VCL_DEFAULT_VALUE("pnm"))
{
  vil_image out = vil_new(os, i.width(), i.height(), i, file_format);
  
  if (!out) {
    cerr << "vil_save: Cannot save to type [" << file_format << "]\n";
    return false;
  }

  vil_copy(i, out);
  
  return true;
}
