#ifndef VSOL_CURVE_3D_H
#define VSOL_CURVE_3D_H
//*****************************************************************************
//:
// \file
// \brief Abstract curve in a 3D space
//
// \author
// Fran�ois BERTEL
//
// \verbatim
//  Modifications
//   2000/05/03 Fran�ois BERTEL Creation
//   2000/06/17 Peter Vanroose  Implemented all operator==()s and type info
//   2004/10/09 Peter Vanroose  Inlined all 1-line methods in class decl
//   2004/09/17 MingChing Chang  Add cast_to stuffs
// \endverbatim
//*****************************************************************************

#include <vsol/vsol_spatial_object_3d.h>
#include <vsol/vsol_point_3d_sptr.h>

class vsol_line_3d;
class vsol_conic_3d;
class vsol_polyline_3d;
class vsol_digital_curve_3d;
class vdgl_digital_curve;

class vsol_curve_3d : public vsol_spatial_object_3d
{
 public:
  enum vsol_curve_3d_type
  { CURVE_NO_TYPE=0,
    LINE,
    CONIC,
    POLYLINE,
    DIGITAL_CURVE,
    NUM_REGION_TYPES
  };

  //***************************************************************************
  // Initialization
  //***************************************************************************
 public:
  //---------------------------------------------------------------------------
  //: Destructor
  //---------------------------------------------------------------------------
  virtual ~vsol_curve_3d();

  //---------------------------------------------------------------------------
  //: Return the spatial type
  //---------------------------------------------------------------------------
  vsol_spatial_object_3d_type spatial_type(void) const { return vsol_spatial_object_3d::CURVE; }

  //---------------------------------------------------------------------------
  //: Return the curve type
  //---------------------------------------------------------------------------
  virtual vsol_curve_3d_type curve_type(void) const { return vsol_curve_3d::CURVE_NO_TYPE; }

  //***************************************************************************
  // Access
  //***************************************************************************

  //---------------------------------------------------------------------------
  //: Return the first point of `this'
  //---------------------------------------------------------------------------
  virtual vsol_point_3d_sptr p0(void) const=0;

  //---------------------------------------------------------------------------
  //: Return the last point of `this'
  //---------------------------------------------------------------------------
  virtual vsol_point_3d_sptr p1(void) const=0;

  //***************************************************************************
  // Replaces dynamic_cast<T>
  //***************************************************************************

  //---------------------------------------------------------------------------
  //: Return `this' if `this' is a curve, 0 otherwise
  //---------------------------------------------------------------------------
  virtual vsol_curve_3d *cast_to_curve(void) {return this;}
  virtual const vsol_curve_3d *cast_to_curve(void) const {return this;}
  
  //---------------------------------------------------------------------------
  //: Return `this' if `this' is an line, 0 otherwise
  //---------------------------------------------------------------------------
  virtual vsol_line_3d const*cast_to_line(void)const{return 0;}
  virtual vsol_line_3d *cast_to_line(void) {return 0;}

  //---------------------------------------------------------------------------
  //: Return `this' if `this' is an conic, 0 otherwise
  //---------------------------------------------------------------------------
  virtual vsol_conic_3d const*cast_to_conic(void)const{return 0;}
  virtual vsol_conic_3d *cast_to_conic(void) {return 0;}

  //---------------------------------------------------------------------------
  //: Return `this' if `this' is an polyline, 0 otherwise
  //---------------------------------------------------------------------------
  virtual vsol_polyline_3d const*cast_to_polyline(void)const{return 0;}
  virtual vsol_polyline_3d *cast_to_polyline(void) {return 0;}

  //---------------------------------------------------------------------------
  //: Return `this' if `this' is an digital_curve_3d, 0 otherwise
  //---------------------------------------------------------------------------
  virtual vsol_digital_curve_3d const*cast_to_digital_curve(void)const{return 0;}
  virtual vsol_digital_curve_3d *cast_to_digital_curve(void) {return 0;}

  //***************************************************************************
  // Status report
  //***************************************************************************

  //---------------------------------------------------------------------------
  //: Return the length of `this'
  //---------------------------------------------------------------------------
  virtual double length(void) const=0;

  //***************************************************************************
  // Status setting
  //***************************************************************************

  //---------------------------------------------------------------------------
  //: Set the first point of the curve
  //---------------------------------------------------------------------------
  virtual void set_p0(const vsol_point_3d_sptr &new_p0)=0;

  //---------------------------------------------------------------------------
  //: Set the last point of the curve
  //---------------------------------------------------------------------------
  virtual void set_p1(const vsol_point_3d_sptr &new_p1)=0;

protected:
  bool endpoints_equal(const vsol_curve_3d &other) const;
};

#endif // VSOL_CURVE_3D_H
