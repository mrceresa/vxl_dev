#ifndef vbl_base_array_3dh
#define vbl_base_array_3dh
#ifdef __GNUG__
#pragma interface
#endif
//
// .LIBRARY vbl
// .HEADER Basics Package
// .INCLUDE vbl/vbl_base_array_3d.h
// .FILE vbl/vbl_base_array_3d.cxx
//
// .SECTION Author:
//             Paul Beardsley, 29.03.96
//             Oxford University, UK
//
// .SECTION Modifications:
//             960926 AWF Converted to non-fascist C++ :-)
//-------------------------------------------------------------------------------

//: Base class for 3-dimensional arrays
// vbl_base_array_3d allocates and deletes 3D arrays
//
class vbl_base_array_3d {
  // PUBLIC INTERFACE----------------------------------------------------------
public:
  // Constructors/Initializers/Destructors-------------------------------------
  
  vbl_base_array_3d(unsigned int n1, unsigned int n2, unsigned int n3);

  int get_row1_count () const { return _row1_count; }
  int get_row2_count () const { return _row2_count; }
  int get_row3_count () const { return _row3_count; }
  
  // INTERNALS-----------------------------------------------------------------
public:
  unsigned int _row1_count;
  unsigned int _row2_count;
  unsigned int _row3_count;
};

#endif // _vbl_base_array_3d_h
