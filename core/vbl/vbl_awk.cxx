// <begin copyright notice>
// ---------------------------------------------------------------------------
//
//                   Copyright (c) 1997 TargetJr Consortium
//               GE Corporate Research and Development (GE CRD)
//                             1 Research Circle
//                            Niskayuna, NY 12309
//                            All Rights Reserved
//              Reproduction rights limited as described below.
//                               
//      Permission to use, copy, modify, distribute, and sell this software
//      and its documentation for any purpose is hereby granted without fee,
//      provided that (i) the above copyright notice and this permission
//      notice appear in all copies of the software and related documentation,
//      (ii) the name TargetJr Consortium (represented by GE CRD), may not be
//      used in any advertising or publicity relating to the software without
//      the specific, prior written permission of GE CRD, and (iii) any
//      modifications are clearly marked and summarized in a change history
//      log.
//       
//      THE SOFTWARE IS PROVIDED "AS IS" AND WITHOUT WARRANTY OF ANY KIND,
//      EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
//      WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
//      IN NO EVENT SHALL THE TARGETJR CONSORTIUM BE LIABLE FOR ANY SPECIAL,
//      INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND OR ANY
//      DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
//      WHETHER OR NOT ADVISED OF THE POSSIBILITY OF SUCH DAMAGES, OR ON
//      ANY THEORY OF LIABILITY ARISING OUT OF OR IN CONNECTION WITH THE
//      USE OR PERFORMANCE OF THIS SOFTWARE.
//
// ---------------------------------------------------------------------------
// <end copyright notice>
//-*- c++ -*-------------------------------------------------------------------
#ifdef __GNUC__
#pragma implementation "vbl_awk.h"
#endif
//
// Class: vbl_awk
// Author: Andrew W. Fitzgibbon, Oxford RRG
// Created: 17 May 97
// Modifications:
//   970517 AWF Initial version.
//
//-----------------------------------------------------------------------------

#include "vbl_awk.h"

#include <ctype.h>
#include <vcl/vcl_iostream.h>

// -- Construct from input stream
vbl_awk::vbl_awk(istream& s, ModeFlags mode):
  fd_(s),
  mode_(mode)
{
  done_ = false;
  line_number_ = 0;
  split_line_ = 0;
  
  next();
}

vbl_awk::~vbl_awk() 
{
  delete [] split_line_;
}

void vbl_awk::next()
{
  // Read line -- should be quite fast after the first one.
  line_.erase();
  int l = 0;
  //bool do_backslash_continuations = (int(mode_) & int(backslash_continuations)) != 0;
  //  bool do_strip_comments = (int(mode_) & int(strip_comments)) != 0;
  
  while (1) {
    int c = fd_.get();
    if (c == EOF) {
      done_ = true;
      break;
    }

    if (c == '\n')
      break;

    line_ += c;
    ++l;
  }

  char const* linep = line_.c_str();
  
  // copy string
  delete [] split_line_;
  split_line_ = new char[line_.size() + 1];
  strcpy(split_line_, linep);

  // Chop line up into fields
  fields_.clear();
  char* cp = split_line_;

  for(;;) {
    // Eat white
    while (*cp && isspace(*cp))
      ++cp;
    if (!*cp) break;
  
    // Push
    fields_.push_back(cp);

    // Find nonwhite
    while (*cp && !isspace(*cp))
      ++cp;
    if (!*cp) break;

    // Zap space
    *cp++ = '\0';
  }

  // Increment line number
  ++line_number_;
}

char const* vbl_awk::line_from(int field_number) const
{
  char const *p = line_.c_str();
  if (field_number >= NF())
    field_number = NF() - 1;
  if (field_number < 0) {
    cerr << "vbl_awk::line_from("<< field_number <<") -- ZOIKS\n";
    return line();
  }
  
  return p + (fields_[field_number] - split_line_);
}

void testvbl_awk()
{
  cout << "Start\n";
  for(vbl_awk awk(cin); awk; ++awk) {
    cout << awk.NF() << ":" << awk[2] << endl;
  }
}
