#ifdef __GNUC__
#pragma implementation
#endif

#include "oxp_vidl_moviefile.h"

#include <vcl_fstream.h>
#include <vcl_strstream.h>
#include <vcl_cstdio.h>
#include <vcl_cstring.h>

#include <vxl_config.h>

#include <vul/vul_printf.h>

#include <vidl/vidl_movie_sptr.h>
#include <vidl/vidl_movie.h>
#include <vidl/vidl_io.h>

#ifdef VCL_WIN32
#include <vidl/vidl_avicodec.h>
#endif
#if HAS_MPEG
#include <oxp/oxp_vidl_mpeg_codec.h>
#endif

struct oxp_vidl_moviefile_privates {
  vidl_movie_sptr m;
};

/////////////////////////////////////////////////////////////////////////////

static bool init = false;

void ensure_initialized()
{
  if (init) return;

  // Register video codec
#ifdef VCL_WIN32
  vidl_io::register_codec(new vidl_avicodec);
#endif
#ifdef HAS_MPEG
  vidl_io::register_codec(new oxp_vidl_mpeg_codec);
#endif
  init = true;
}

/////////////////////////////////////////////////////////////////////////////


oxp_vidl_moviefile::oxp_vidl_moviefile(char const* f)
{
  ensure_initialized();

  p = new oxp_vidl_moviefile_privates;
  p->m = vidl_io::load_movie(f, 0, 9999, 1);
}

oxp_vidl_moviefile::~oxp_vidl_moviefile()
{
  delete p;
}

int oxp_vidl_moviefile::GetLength()
{
  return p->m->length();
}

vil_image oxp_vidl_moviefile::GetImage(int frame)
{
  return p->m->get_frame(frame)->get_image();
}


int oxp_vidl_moviefile::GetSizeX(int frame)
{
  return p->m->get_frame(frame)->width();
}

int oxp_vidl_moviefile::GetSizeY(int frame)
{
  return p->m->get_frame(frame)->height();
}

int oxp_vidl_moviefile::GetBitsPixel()
{
  return p->m->get_frame(0)->get_bits_pixel();
}

bool oxp_vidl_moviefile::IsInterlaced()
{
  assert(0);
#if 0
  return p->m->interlaced != 0;
#else
  return 0;
#endif
}

bool oxp_vidl_moviefile::HasFrame(int frame_index)
{
  return (0 <= frame_index) && (frame_index < GetLength());
}

bool oxp_vidl_moviefile::GetFrame(int frame_index, void* buffer)
{
  vidl_frame_sptr f = p->m->get_frame(frame_index);
  return f->get_section(buffer, 0,0, f->width(), f->height());
}

bool oxp_vidl_moviefile::GetField(int field_index, void* buffer)
{
  return false;
}

