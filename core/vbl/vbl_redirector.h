#ifndef vbl_redirector_h_
#define vbl_redirector_h_
#ifdef __GNUC__
#pragma interface
#endif
// This is vxl/vbl/vbl_redirector.h

//:
// \file
// \brief Contains class for simplified redirection of iostreams
// \author awf@robots.ox.ac.uk
//
// \verbatim
// Modifications
// 21-JUL-00  M.Bacic, Oxford RRG -- fixed 'pubsync' on gcc.
// PDA (Manchester) 21/03/2001: Tidied up the documentation
// \endverbatim

#include <vcl_iostream.h>

struct vbl_redirector_data;

//: Simplified redirection of iostreams
// To implement your own, derive a class from vbl_redirector,
// and implement `putchunk'.
class vbl_redirector {
public:

  //: Attach redirector to vcl_ostream s,
  // so that all future output to s goes through this->putchunk
   vbl_redirector(vcl_ostream& s);

  //: Destroy redirector, restore stream to original.
   virtual ~vbl_redirector();

  //: The filter function
  // Called with `n' characters in `buf', do with as you like.
   virtual int putchunk(char const* buf, int n);

protected:
  //: Put characters to original stream.
  // Useful for derived classes which wish to filter a stream.
   int put_passthru(char const* buf, int n);

  //: Sync original stream.
   int sync_passthru();

private:
  vbl_redirector_data* p;
};

#endif // vbl_redirector_h_
