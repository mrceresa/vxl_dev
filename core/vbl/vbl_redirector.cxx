#ifdef __GNUC__
#pragma implementation
#endif

#include <vcl_compiler.h>
#if defined(VCL_SGI_CC)
// urgh! there is no rdbuf() method for *setting* thestream buffer.
// These headers are in the old style intentionally. We *want* to
// include the old SGI headers as they are without going through vcl.
# include <memory.h>         // dont_vxl_filter
# include <stddef.h>         // dont_vxl_filter
# define private public
# define protected public
# include <iostream.h>       // dont_vxl_filter
# undef private
# undef protected
#endif

#include <vbl/vbl_redirector.h>

//----------------------------------------------------------------------
// This class is used as a stream buffer that can 
// redirect ouput from cout, cerr, clog to a CoutWindow class.
class vbl_redirector_streambuf : public vcl_streambuf {
  vbl_redirector_data* p;
public:
  vbl_redirector_streambuf(vbl_redirector_data* p_):p(p_) {}
  int sync ();
  int overflow (int ch);
  int underflow(){return 0;}
  // The libraries which comes with sgi 7.2.1 and
  // SunPro 5.0 take char const *, which is
  // non-standard, but we have to live with it. A
  // better cpp test would be welcome. fsm.
#if defined(VCL_SGI_CC)
# define xsputn_const const
# define xsputn_sizet int
#elif defined(VCL_SUNPRO_CC_50) || defined(VCL_KAI) // RogueWave or ISO?
# define xsputn_const const
# define xsputn_sizet vcl_streamsize
#else
# define xsputn_const /* */
# define xsputn_sizet vcl_streamsize
#endif
  xsputn_sizet xsputn (xsputn_const char* text, xsputn_sizet n);
};

struct vbl_redirector_data {
  vbl_redirector* owner;
  vcl_streambuf* old_cerrbuf;
  vbl_redirector_streambuf* buf;
  vcl_ostream* s;
};

/////////////////////////////////////////////////////////////////////////////
// streambuf stuff

int vbl_redirector_streambuf::sync ()
{ int n = pptr () - pbase ();
  return (n && p->owner->putchunk ( pbase (), n) != n) ? EOF : 0;
}
 
int vbl_redirector_streambuf::overflow (int ch)
{ 
  int n = pptr () - pbase ();
  if (n && sync ())
    return EOF;
  if (ch != EOF)
    {
      char cbuf[1];
      cbuf[0] = ch;
      if (p->owner->putchunk ( cbuf, 1) != 1)
        return EOF;
    }
  pbump (-n);  // Reset pptr().
  return 0;
}
 
xsputn_sizet vbl_redirector_streambuf::xsputn (xsputn_const char* text, xsputn_sizet n)
{ 
  return sync () == EOF ? 0 : p->owner->putchunk ( text, n);
}

//////////////// Data for debugging


vbl_redirector::vbl_redirector(vcl_ostream& s):
  p(new vbl_redirector_data)
{
  p->owner = this;
  p->buf = new vbl_redirector_streambuf(p);
  p->old_cerrbuf = s.rdbuf();
#if defined(VCL_SGI_CC)
  s.bp = p->buf;
  s.clear();
#else
  s.rdbuf(p->buf);
#endif
  p->s = &s;
}

vbl_redirector::~vbl_redirector()
{
#if defined(VCL_SGI_CC)
  // see above
  p->s->bp = p->old_cerrbuf;
  p->s->clear();
#else
  p->s->rdbuf(p->old_cerrbuf);
#endif
  delete p->buf;
  delete p;
}

int vbl_redirector::sync_passthru() 
{ 
#if defined(VCL_GCC) && !defined(GNU_LIBSTDCXX_V3)
  // The default libraries these compilers come with are non-standard
  // since they have no pubsync() method. According to standard, though,
  // pubsync() just returns sync(), so :
  return p->old_cerrbuf->sync();
#elif defined(VCL_SGI_CC)
  // ditto
  return p->old_cerrbuf->sync();
#else
  return p->old_cerrbuf->pubsync();
#endif
}

int vbl_redirector::put_passthru(char const* buf, int n)
{
  return p->old_cerrbuf->sputn(buf, n);
}

// -- Default action is just to pass text on the the old stream.
int vbl_redirector::putchunk(char const* buf, int n)
{
  return put_passthru(buf, n);
}
