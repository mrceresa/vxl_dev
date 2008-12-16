// This is core/vgl/vgl_intersection.txx
#ifndef vgl_intersection_txx_
#define vgl_intersection_txx_
//:
// \file
// \author Gamze Tunali

#include "vgl_intersection.h"

#include <vcl_limits.h>
#include <vcl_cassert.h>
#include <vcl_cmath.h>
#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_line_2d.h>
#include <vgl/vgl_line_3d_2_points.h>
#include <vgl/vgl_line_segment_2d.h>
#include <vgl/vgl_line_segment_3d.h>
#include <vgl/vgl_vector_3d.h>
#include <vgl/vgl_box_2d.h>
#include <vgl/vgl_polygon.h>
#include <vgl/vgl_plane_3d.h>
#include <vgl/vgl_distance.h>
#include <vgl/vgl_tolerance.h>
#include <vgl/vgl_lineseg_test.h>

inline bool vgl_near_zero(double x) { return x < 1e-8 && x > -1e-8; }
inline bool vgl_near_eq(double x, double y) { return vgl_near_zero(x-y); }

//: compute the intersection of an infinite line with *this box.
//  p0 and p1 are the intersection points
template <class Type>
bool vgl_intersection(const vgl_box_2d<Type>& box,
                      const vgl_line_2d<Type>& line,
                      vgl_point_2d<Type>& p0,
                      vgl_point_2d<Type>& p1)
{
  double a = line.a(), b = line.b(), c = line.c();
  double xmin=box.min_x(), xmax=box.max_x();
  double ymin=box.min_y(), ymax=box.max_y();

  //Run through the cases
  //
  if (vgl_near_zero(a))// The line is y = -c/b
  {
    float y0 = static_cast<float>(-c/b);
    // The box edge is collinear with line?
    if (vgl_near_eq(ymin,y0))
    {
      p0.set(static_cast<Type>(xmin), static_cast<Type>(ymin));
      p1.set(static_cast<Type>(xmax), static_cast<Type>(ymin));
      return true;
    }
    if (vgl_near_eq(ymax,y0))
    {
      p0.set(static_cast<Type>(xmin), static_cast<Type>(ymax));
      p1.set(static_cast<Type>(xmax), static_cast<Type>(ymax));
      return true;
    }

    if ((ymin > y0) || (y0 > ymax)) // The line does not intersect the box
      return false;
    else // The line does intersect
    {
      p0.set(static_cast<Type>(xmin), static_cast<Type>(y0));
      p1.set(static_cast<Type>(xmax), static_cast<Type>(y0));
      return true;
    }
  }

  if (vgl_near_zero(b))// The line is x = -c/a
  {
    float x0 = static_cast<float>(-c/a);
    // The box edge is collinar with l?
    if (vgl_near_eq(xmin,x0))
    {
      p0.set(static_cast<Type>(xmin), static_cast<Type>(ymin));
      p1.set(static_cast<Type>(xmin), static_cast<Type>(ymax));
      return true;
    }
    if (vgl_near_eq(xmax,x0))
    {
      p0.set(static_cast<Type>(xmax), static_cast<Type>(ymin));
      p1.set(static_cast<Type>(xmax), static_cast<Type>(ymax));
      return true;
    }

    if ((xmin > x0) || (x0 > xmax)) // The line does not intersect the box
      return false;
    else // The line does intersect
    {
      p0.set(static_cast<Type>(x0), static_cast<Type>(ymin));
      p1.set(static_cast<Type>(x0), static_cast<Type>(ymax));
      return true;
    }
  }

  // The normal case with no degeneracies

//: There are six possible intersection combinations:
// \verbatim
//
//                C01 /    CY     \ C11
//                   /     |       \           .
//       ymax  -----/------|--------\-----
//            |    /       |         \    |
//            |   /        |          \   |
//            |  /         |           \  | \  .
//            | /          |            \ |  \_ Bounding Box
//            |/           |             \|
//            /            |              \    .
//           /|            |              |\   .
//           ---------------------------------- CX
//          \ |            |              /
//           \|            |             /|
//            \            |            / |
//            |\           |           /  |
//            | \          |          /   |
//            |  \         |         /    |
//       xmin  ---\--------|--------/-----   xmax
//       ymin      \       |       /
//              C00 \             / C10
// \endverbatim

  // Intersection with x = xmin
  float y_xmin_int = static_cast<float>(-(c + a*xmin)/b);
  bool inside_xmin = (y_xmin_int >= ymin) && (y_xmin_int <= ymax);

  // Intersection with x = xmax
  float y_xmax_int = static_cast<float>(-(c + a*xmax)/b);
  bool inside_xmax = (y_xmax_int >= ymin) && (y_xmax_int <= ymax);

  // Intersection with y = ymin
  float x_ymin_int = static_cast<float>(-(c + b*ymin)/a);
  bool inside_ymin = (x_ymin_int >= xmin) && (x_ymin_int <= xmax);

  // Intersection with y = ymax
  float x_ymax_int = static_cast<float>(-(c + b*ymax)/a);
  bool inside_ymax = (x_ymax_int >= xmin) && (x_ymax_int <= xmax);

  // Case CX
  if (inside_xmin && inside_xmax &&
      !(vgl_near_eq(y_xmin_int,ymin) && vgl_near_eq(y_xmax_int,ymax)))
  {
    p0.set(static_cast<Type>(xmin), static_cast<Type>(y_xmin_int));
    p1.set(static_cast<Type>(xmax), static_cast<Type>(y_xmax_int));
    return true;
  }

  // Case CY
  if (inside_ymin && inside_ymax &&
      !(vgl_near_eq(x_ymin_int,xmin) && vgl_near_eq(x_ymax_int,xmax)))
  {
    p0.set(static_cast<Type>(x_ymin_int), static_cast<Type>(ymin));
    p1.set(static_cast<Type>(x_ymax_int), static_cast<Type>(ymax));
    return true;
  }

  // Case C00
  if (inside_xmin && inside_ymin &&
      !(inside_xmax && inside_ymax))
  {
    p0.set(static_cast<Type>(xmin), static_cast<Type>(y_xmin_int));
    p1.set(static_cast<Type>(x_ymin_int), static_cast<Type>(ymin));
    return true;
  }

  // Case C01
  if (inside_xmin && inside_ymax &&
      !(inside_xmax && inside_ymin))
  {
    p0.set(static_cast<Type>(xmin), static_cast<Type>(y_xmin_int));
    p1.set(static_cast<Type>(x_ymax_int), static_cast<Type>(ymax));
    return true;
  }

  // Case C10
  if (inside_ymin && inside_xmax &&
      !(inside_xmin && inside_ymax))
  {
    p0.set(static_cast<Type>(x_ymin_int), static_cast<Type>(ymin));
    p1.set(static_cast<Type>(xmax), static_cast<Type>(y_xmax_int));
    return true;
  }

  // Case C11
  if (inside_ymax && inside_xmax &&
      !(inside_xmin && inside_ymin))
  {
    p0.set(static_cast<Type>(x_ymax_int), static_cast<Type>(ymax));
    p1.set(static_cast<Type>(xmax), static_cast<Type>(y_xmax_int));
    return true;
  }
  //Exactly p0ssing through diagonal of BB
  if (inside_xmin && inside_xmax && inside_ymin && inside_ymax)
  {
    if (a>0) // 45 degrees
    {
      p0.set(static_cast<Type>(xmin), static_cast<Type>(ymin));
      p1.set(static_cast<Type>(xmax), static_cast<Type>(ymax));
      return true;
    }
    else // 135 degrees
    {
      p0.set(static_cast<Type>(xmin), static_cast<Type>(ymax));
      p1.set(static_cast<Type>(xmax), static_cast<Type>(ymin));
      return true;
    }
  }
  return false;
}
//: Returns the number of intersections of a line segment with a box, up to two are returned in p0 and p1.
template <class Type>
unsigned vgl_intersection(const vgl_box_2d<Type>& box,
                          const vgl_line_segment_2d<Type>& line_seg,
                          vgl_point_2d<Type>& p0,
                          vgl_point_2d<Type>& p1)
{
  vgl_line_2d<Type> line(line_seg.a(), line_seg.b(), line_seg.c());
  vgl_point_2d<Type> pi0, pi1;
  //if no intersection just return
  if (!vgl_intersection<Type>(box, line, pi0, pi1))
    return 0;
  unsigned nint = 0;
  // check if intersection points are interior to the line segment
  if (vgl_lineseg_test_point<Type>(pi0, line_seg)){
    p0 = pi0;
    nint++;
  }
  if (vgl_lineseg_test_point<Type>(pi1, line_seg)){
    p1 = pi1;
    nint++;
  }
  return nint;
}

//: Return the intersection point of two concurrent lines
template <class T>
vgl_point_3d<T> vgl_intersection(vgl_line_3d_2_points<T> const& l1,
                                 vgl_line_3d_2_points<T> const& l2)
{
  assert(concurrent(l1,l2));
  T a0=l1.point1().x(),a1=l1.point2().x(),a2=l2.point1().x(),a3=l2.point2().x(),
    b0=l1.point1().y(),b1=l1.point2().y(),b2=l2.point1().y(),b3=l2.point2().y(),
    c0=l1.point1().z(),c1=l1.point2().z(),c2=l2.point1().z(),c3=l2.point2().z();
  T t1 = (b3-b2)*(a1-a0)-(a3-a2)*(b1-b0), t2 = (b0-b2)*(a1-a0)-(a0-a2)*(b1-b0);
  if (t1 == 0)
    t1 = (c3-c2)*(a1-a0)-(a3-a2)*(c1-c0), t2 = (c0-c2)*(a1-a0)-(a0-a2)*(c1-c0);
  if (t1 == 0)
    t1 = (c3-c2)*(b1-b0)-(b3-b2)*(c1-c0), t2 = (c0-c2)*(b1-b0)-(b0-b2)*(c1-c0);
  return vgl_point_3d<T>(((t1-t2)*a2+t2*a3)/t1,
                         ((t1-t2)*b2+t2*b3)/t1,
                         ((t1-t2)*c2+t2*c3)/t1);
}

//: Return the intersection point of segments of two concurrent lines
// \relates vgl_line_segment_3d
template <class T>
bool vgl_intersection(vgl_line_segment_3d<T> const& l1,
                      vgl_line_segment_3d<T> const& l2,
                      vgl_point_3d<T>& i_pnt)
{
  i_pnt = vgl_intersection(vgl_line_3d_2_points<T>(l1.point1(),l1.point2()),
                           vgl_line_3d_2_points<T>(l2.point1(),l2.point2()));

  double l1_len =   length(l1.point1() - l1.point2());
  double l1_idist = length(l1.point1() - i_pnt) + length(l1.point2() - i_pnt);
  double l2_len =   length(l2.point1() - l2.point2());
  double l2_idist = length(l2.point1() - i_pnt) + length(l2.point2() - i_pnt);

  return vgl_near_zero(l1_len - l1_idist) && vgl_near_zero(l2_len - l2_idist);
}

//: Return the intersection point of segments of two concurrent lines
// \relates vgl_line_segment_3d
template <class T>
bool vgl_intersection(vgl_line_3d_2_points<T> const& l1,
                      vgl_line_segment_3d<T> const& l2,
                      vgl_point_3d<T>& i_pnt)
{
  i_pnt = vgl_intersection(l1,
                           vgl_line_3d_2_points<T>(l2.point1(),l2.point2()));

  double l2_len =   length(l2.point1() - l2.point2());
  double l2_idist = length(l2.point1() - i_pnt) + length(l2.point2() - i_pnt);

  return vgl_near_zero(l2_len - l2_idist);
}

//: Return the intersection point of a line and a plane.
// \relates vgl_line_3d_2_points
// \relates vgl_plane_3d
template <class T>
vgl_point_3d<T> vgl_intersection(vgl_line_3d_2_points<T> const& line,
                                 vgl_plane_3d<T> const& plane)
{
  vgl_vector_3d<T> dir = line.direction();

  vgl_point_3d<T> pt;

  double denom = plane.a()*(dir.x()) +
                 plane.b()*(dir.y()) +
                 plane.c()*(dir.z());

  if (denom == 0)
  {
    const T inf = vcl_numeric_limits<T>::infinity();
    // Line is either parallel or coplanar
    // If the distance from a line endpoint to the plane is zero, coplanar
    if (vgl_distance(line.point1(), plane)==0.0)
      pt.set(inf,inf,inf);
    else
      pt.set(inf,0,0);
  }
  else
  {
    // Infinite line intersects plane
    double numer = -(plane.a()*line.point1().x() +
                     plane.b()*line.point1().y() +
                     plane.c()*line.point1().z() +
                     plane.d());

    dir *= numer/denom;
    pt = line.point1() + dir;
  }

  return pt;
}

template <class T>
bool vgl_intersection( const vgl_line_2d<T> &line0,
                       const vgl_line_2d<T> &line1,
                       vgl_point_2d<T>      &intersection_point )
{
  T a0, b0, c0,  a1, b1, c1;
  a0 = line0.a(); b0 = line0.b(); c0 = line0.c();
  a1 = line1.a(); b1 = line1.b(); c1 = line1.c();

  T delta, delta_x, delta_y, x, y;
  delta = a0*b1 - a1*b0;
  if ( vcl_abs(delta) <= vgl_tolerance<T>::position ) // Lines are parallel
    return false;
  delta_x = -c0*b1 + b0*c1; delta_y = -a0*c1 + a1*c0;
  x = delta_x / delta; y = delta_y / delta;

//   intersection_point.set( (Type)x, (Type)y );
  intersection_point.set( x, y );
  return true;
}

//: Return the intersection point of three planes.
// \relates vgl_plane_3d
template <class T>
vgl_point_3d<T> vgl_intersection(const vgl_plane_3d<T>& p1,
                                 const vgl_plane_3d<T>& p2,
                                 const vgl_plane_3d<T>& p3)
{
  vgl_point_3d<T> p(p1, p2, p3);
  return p;
}

//: Return true if any point on [p1,p2] is within tol of [q1,q2]
//  Tests two line segments for intersection or near intersection
//  (within given tolerance).
// \author Dan jackson
template <class T>
bool vgl_intersection(vgl_point_2d<T> const& p1,
                      vgl_point_2d<T> const& p2,
                      vgl_point_2d<T> const& q1,
                      vgl_point_2d<T> const& q2,
                      double tol)
{
  vgl_vector_2d<T> u = p2 - p1;
  double L2 = u.x()*u.x() + u.y()*u.y();  // Square of length p1-p2
  vgl_vector_2d<T> v(-u.y(),u.x());

  double uq1 = dot_product(q1 - p1,u);
  double vq1 = dot_product(q1 - p1,v);
  double tol2 = tol*tol;

  // Check if q1 is in central band (either side of line p1-p2
  if (uq1 > 0 && uq1 < L2)
  {
    // Check if q1 is within tol of the line, ie |vq1/L| < tol
    if (vq1*vq1 <=tol2*L2) return true;
  }
  else
  {
    // Check if q1 is within tol of either end of line
    if ( (q1-p1).sqr_length() <= tol2 || (q1-p2).sqr_length() <= tol2 )
      return true;
  }

  // Repeat test for q2
  double uq2 = dot_product(q2 - p1,u);
  double vq2 = dot_product(q2 - p1,v);

  // Check if q2 is in central band (either side of line p1-p2
  if (uq2 > 0 && uq2 < L2)
  {
    // Check if q1 is within tol of the line, ie |vq1/L| < tol
    if (vq2*vq2 <=tol2*L2)
      return true;
  }
  else
  {
    // Check if q1 is within tol of either end of line
    if ( (q2-p1).sqr_length() <= tol2 || (q2-p2).sqr_length() <= tol2 )
      return true;
  }

  // The points q1 and q2 do not lie within the tolerance region
  // around line segment (p1,p2)
  // Now repeat the test the other way around,
  // testing whether points p1 and p2 lie in tolerance region
  // of line (q1,q2)

  u = q2 - q1;
  L2 = u.x()*u.x() + u.y()*u.y();  // Square of length p1-p2
  v.set(-u.y(),u.x());

  double up1 = dot_product(p1 - q1,u);
  double vp1 = dot_product(p1 - q1,v);

  // Check if p1 is in central band either side of line q1-q2
  if (up1 > 0 && up1 < L2)
  {
    // Check if p1 is within tol of the line, ie |vp1/L| < tol
    if (vp1*vp1 <=tol2*L2)
      return true;
  }
  else
  {
    // Check if p1 is within tol of either end of line
    if ( (p1-q1).sqr_length() <= tol2 || (p1-q2).sqr_length() <= tol2 )
      return true;
  }

  double up2 = dot_product(p2 - q1,u);
  double vp2 = dot_product(p2 - q1,v);

  // Check if p2 is in central band either side of line q1-q2
  if (up2 > 0 && up2 < L2)
  {
    // Check if p1 is within tol of the line, ie |vp1/L| < tol
    if (vp2*vp2 <=tol2*L2)
      return true;
  }
  else
  {
    // Check if p2 is within tol of either end of line
    if ( (p2-q1).sqr_length() <= tol2 || (p2-q2).sqr_length() <= tol2)
      return true;
  }

  // Now check for actual intersection
  if (vq1*vq2 >= 0)
    return false;
  else if (vp1*vp2 >= 0)
    return false;
  else
    return true;
}

template <class T>
bool vgl_intersection(const vgl_box_2d<T>& b,
                      const vgl_polygon<T>& poly)
{
  // easy checks first
  //check if any poly vertices are inside the box
  unsigned ns = poly.num_sheets();
  bool hit = false;
  for ( unsigned s = 0; s<ns&&!hit; ++s){
    unsigned n = poly[s].size();
    for (unsigned i = 0; i<n&&!hit; ++i){
      vgl_point_2d<T> p = poly[s][i];
      hit = b.contains(p.x(), p.y());
    }
  }
  if (hit) return true;
  //check if any box vertices are inside the polygon
  T minx = b.min_x(), maxx = b.max_x();
  T miny = b.min_y(), maxy = b.max_y();
  hit = poly.contains(minx, miny) || poly.contains(maxx, maxy) ||
    poly.contains(minx, maxy) || poly.contains(maxx, miny);
  if (hit) return true;
  //check if any polygon edges intersect the box
  for (unsigned s = 0; s<ns&&!hit; ++s)
  {
    unsigned n = poly[s].size();
    vgl_point_2d<T> ia, ib;
    vgl_point_2d<T> last = poly[s][0];
    for (unsigned i = 1; i<n&&!hit; ++i)
    {
      vgl_point_2d<T> p = poly[s][i];
      vgl_line_segment_2d<T> l(last, p);
      hit = vgl_intersection<T>(b, l, ia, ib)>0;
      last = p;
    }
    if (!hit) {
      vgl_point_2d<T> start = poly[s][0];
      vgl_line_segment_2d<T> ll(last, start);
      hit = vgl_intersection<T>(b,ll, ia, ib)>0;
    }
  }
  return hit;
}

#undef VGL_INTERSECTION_INSTANTIATE
#define VGL_INTERSECTION_INSTANTIATE(T) \
template vgl_point_3d<T > vgl_intersection(vgl_line_3d_2_points<T > const&,vgl_line_3d_2_points<T > const&); \
template bool vgl_intersection(vgl_line_segment_3d<T > const&,vgl_line_segment_3d<T > const&,vgl_point_3d<T >&); \
template bool vgl_intersection(vgl_line_3d_2_points<T > const&,vgl_line_segment_3d<T > const&,vgl_point_3d<T >&); \
template vgl_point_3d<T > vgl_intersection(vgl_line_3d_2_points<T > const&,vgl_plane_3d<T > const&); \
template vgl_point_3d<T > vgl_intersection(const vgl_plane_3d<T >&,const vgl_plane_3d<T >&,const vgl_plane_3d<T >&); \
template bool vgl_intersection(const vgl_box_2d<T >&, const vgl_line_2d<T >& line, vgl_point_2d<T >& p0, vgl_point_2d<T >&); \
template unsigned vgl_intersection(const vgl_box_2d<T >& , const vgl_line_segment_2d<T >& , vgl_point_2d<T >& , vgl_point_2d<T >& ); \
template bool vgl_intersection(const vgl_line_2d<T >&, const vgl_line_2d<T >&, vgl_point_2d<T >&); \
template bool vgl_intersection(vgl_point_2d<T > const&,vgl_point_2d<T > const&,vgl_point_2d<T > const&,vgl_point_2d<T > const&,double); \
template bool vgl_intersection(vgl_box_2d<T > const&, vgl_polygon<T > const&)

//: Instantiate only functions suitable for integer instantiation.
#undef VGL_INTERSECTION_BOX_INSTANTIATE
#define VGL_INTERSECTION_BOX_INSTANTIATE(T) \
template bool vgl_intersection(const vgl_box_2d<T >&, const vgl_line_2d<T >& line, vgl_point_2d<T >& p0, vgl_point_2d<T >&)

#endif // vgl_intersection_txx_
