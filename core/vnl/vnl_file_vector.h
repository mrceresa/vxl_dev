// This is core/vnl/vnl_file_vector.h
#ifndef vnl_file_vector_h_
#define vnl_file_vector_h_
#ifdef VCL_NEEDS_PRAGMA_INTERFACE
#pragma interface
#endif
//:
// \file
// \brief Load vnl_vector<T> from file
// \author Andrew W. Fitzgibbon, Oxford RRG
// \date   23 Dec 96
//
// \verbatim
//  Modifications
//   fsm created by modifying class FileMatrix
//   LSB (Manchester) 23/3/01 Tidied documentation
// \endverbatim
//
//-----------------------------------------------------------------------------

#include <vnl/vnl_vector.h>

//: Templated class to load a vector from a file.
template <class T>
class vnl_file_vector : public vnl_vector<T>
{
  //: Helper types for safe boolean conversion.
  struct safe_bool_dummy { void dummy() {} };
  typedef void (safe_bool_dummy::* safe_bool)();
 public:
  vnl_file_vector(char const* filename);

  operator safe_bool () const
    { return (ok_)? &safe_bool_dummy::dummy : 0; }
  safe_bool operator!() const
    { return (ok_)? 0 : &safe_bool_dummy::dummy; }

 private:
  bool ok_;
};

#endif // vnl_file_vector_h_
