// This is mul/vil2/vil2_load.cxx
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma implementation
#endif

#include "vil2_load.h"

#include <vcl_iostream.h>

#include <vil/vil_open.h>
#include <vil2/vil2_file_format.h>
#include <vil/vil_stream.h>
#include <vil/vil_property.h>
#include <vil/vil_flipud.h>
#include <vil/vil_flip_components.h>
#include <vil2/vil2_image_data.h>


vil2_image_data_sptr vil2_load_raw(vil_stream *is)
{
  for (vil2_file_format** p = vil2_file_format::all(); *p; ++p) {
#if 0 // debugging
    vcl_cerr << __FILE__ " : trying \'" << (*p)->tag() << "\'\n";
#endif
    is->seek(0);
    vil2_image_data_sptr i = (*p)->make_input_image(is);
    if (i)
      return i;
  }

  // failed.
  vcl_cerr << __FILE__ ": Tried";
  for (vil2_file_format** p = vil2_file_format::all(); *p; ++p)
    // 'flush' in case of segfault next time through loop. Else, we
    // will not see those printed tags still in the stream buffer.
    vcl_cerr << " \'" << (*p)->tag() << "\'" << vcl_flush;
  vcl_cerr << vcl_endl;

  return 0;
}

vil2_image_data_sptr vil2_load_raw(char const* filename)
{
  vil_stream *is = vil_open(filename, "r");
  if (is)
    return vil2_load_raw(is);
  else {
    vcl_cerr << __FILE__ ": Failed to load [" << filename << "]\n";
    return vil2_image_data_sptr(0);
  }
}

vil2_image_data_sptr vil2_load(char const* filename)
{
  vil2_image_data_sptr i = vil2_load_raw(filename);
  if (!i) return i; // leave early if it hasn't loaded.

  bool top_first=true, bgr=false;
  i->get_property(vil_property_top_row_first, &top_first);
  if (i->nplanes() == 3)
    i->get_property(vil_property_component_order_is_BGR,&bgr);
#ifdef VIL2_TO_BE_FIXED
  if (!top_first)
    i = vil_flipud(i);
  if (bgr)
    i = vil_flip_components(i);
#endif// VIL2_TO_BE_FIXED
  return i;
}
