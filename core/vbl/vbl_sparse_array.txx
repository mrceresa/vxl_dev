//-*- c++ -*-------------------------------------------------------------------
//
// Class: vbl_sparse_array
// Author: Andrew W. Fitzgibbon, Oxford RRG
// Created: 02 Oct 96
//
//-----------------------------------------------------------------------------

#include "vbl_sparse_array.h"

#include <vcl_cassert.h>
#include <vcl_iostream.h>
#include <vcl_utility.h>

// -- Return contents of (i).  Assertion failure if not yet filled.
template <class T>
T const & vbl_sparse_array<T>::operator [] (unsigned i) const
{
  Map::const_iterator p = storage_.find(i);
  
  assert(p != storage_.end());

  return (*p).second;
}

// -- Return the address of location (i).  0 if not yet filled.
template <class T>
T* vbl_sparse_array<T>::get_addr(unsigned i)
{
  Map::iterator p = storage_.find(i);

  if (p == storage_.end())
    return 0;

  return &(*p).second;
}

// -- Return true if location (i) has been filled.
template <class T>
bool vbl_sparse_array<T>::fullp(unsigned i) const
{
  return storage_.find(i) != storage_.end();
}

// -- Put a value into location (i).
template <class T>
bool vbl_sparse_array<T>::put(unsigned i, const T& t)
{
  vcl_pair<Map::iterator,bool> res = storage_.insert(Map::value_type(i,t));

  return res.second;
} 

// -- Print the Array to a stream in "(i): value" format.
template <class T>
ostream& vbl_sparse_array<T>::print(ostream& out) const
{
  for(const_iterator p = begin(); p != end(); ++p)
    out << "(" << (*p).first << "): " << (*p).second << vcl_endl;
  return out;
}

#undef VBL_SPARSE_ARRAY_INSTANTIATE
#define VBL_SPARSE_ARRAY_INSTANTIATE(T)\
template class vbl_sparse_array<T>; \
VCL_INSTANTIATE_INLINE(ostream& operator << (ostream&, const vbl_sparse_array<T> &))


#ifdef MAIN
int main()
{
  vbl_sparse_array<double> x;

  x[1] = 1.23;
  x[100] = 100.2003;

  vcl_cout << "203 was " << x.put(203, 7) << vcl_endl;
  vcl_cout << "203 is " << *x.get_addr(203) << vcl_endl;

  vcl_cout << "1 = " << x[1] << vcl_endl;
  vcl_cout << "2 = " << x[2] << vcl_endl;

  vcl_cout << "100 = " << x[100] << vcl_endl;

  vcl_cout << "303 is full? " << x.fullp(303) << vcl_endl;
  
  vcl_cout << x;
}
#endif
