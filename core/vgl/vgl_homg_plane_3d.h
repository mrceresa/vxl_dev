#ifndef vgl_homg_plane_3d_h
#define vgl_homg_plane_3d_h
#ifdef __GNUC__
#pragma interface
#endif

// .NAME vgl_homg_plane_3d
// .INCLUDE vgl/vgl_homg_plane_3d.h
// .FILE vgl_homg_plane_3d.txx
// .SECTION Author
//    Don Hamilton, Peter Tu
// Created: Feb 15 2000
// .SECTION Modifications:
//  Peter Vanroose, 29 Feb 2000: several minor fixes and additions

#include <vcl_iostream.h>
#include <vcl_algorithm.h>
#include <vcl_cmath.h>   // vcl_abs()
#include <vcl_cstdlib.h> // vcl_abs()

template <class Type>
class vgl_plane_3d;

template <class Type>
class vgl_homg_point_3d;

//: Represents a homogeneous 3D plane
template <class Type>
class vgl_homg_plane_3d {
public:
  vgl_homg_plane_3d () {}
  
  // -- Construct from four Types.
  vgl_homg_plane_3d (Type nx, Type ny, Type nz, Type d) { set(nx,ny,nz,d); }
  
  // -- Construct from 4-vector.
  vgl_homg_plane_3d (const Type v[4]) { set(v[0],v[1],v[2],v[3]); }
  
  // -- Construct from Normal and d
  vgl_homg_plane_3d (const Type normal[3], Type d) {
    set(normal[0],normal[1],normal[2],d);
  }

  // -- Construct from Normal and a point
  vgl_homg_plane_3d (const Type normal[3], const vgl_homg_point_3d<Type>& p);

  // Data Access-------------------------------------------------------------
  
  inline Type nx() const {return data_[0];}
  inline Type ny() const {return data_[1];}
  inline Type nz() const {return data_[2];}
  inline Type d() const {return data_[3];}

  // iterators.
  typedef Type*       iterator;
  typedef Type const* const_iterator;
  iterator begin() { return (Type*)data_; }
  iterator end() { return (Type*)(data_+4); }
  const_iterator begin() const { return (Type const*)data_; }
  const_iterator end() const { return (Type const*)(data_+4); }
  
  // -- Set nx ny nz d
  inline void set (Type nx, Type ny, Type nz, Type d){
    data_[0] = nx;
    data_[1] = ny;
    data_[2] = nz;
    data_[3] = d;
  }

  // -- Return true iff the plane is the plane at infinity.
  // The method checks that max(|nx|,|ny|,|nz|) < tol * |d|
  bool ideal(Type tol) const {
    return vcl_max(vcl_max(vcl_abs(nx()),vcl_abs(ny())),vcl_abs(nz())) < tol * vcl_abs(d());
  }

protected:
  // the four homogenenous coordinates of the point.
  Type data_[4];
};

// stream operators 

template <class Type>
inline vcl_ostream&  operator<<(vcl_ostream& s, const vgl_homg_plane_3d<Type>& p) {
  return s << " <vgl_homg_plane_3d "
           << p.data_[0] << " x + "
           << p.data_[1] << " y + "
           << p.data_[2] << " z + "
           << p.data_[3] << " = 0 >";
}

template <class Type>
vcl_istream&  operator>>(vcl_istream& is, vgl_homg_plane_3d<Type>& p) {
  return is >> p.data_[0] >> p.data_[1] >> p.data_[2] >> p.data_[3];
}

#endif // vgl_homg_plane_3d_h
