#ifndef VSOL_CURVE_2D_H
#define VSOL_CURVE_2D_H
//*****************************************************************************
//:
//  \file
// \brief Abstract curve in a 2D space
//
// \author
// Fran�ois BERTEL
//
// \verbatim
// Modifications
//  2000/06/17 Peter Vanroose  Implemented all operator==()s and type info
//  2000/04/27 Fran�ois BERTEL Creation
// \endverbatim
//*****************************************************************************

//*****************************************************************************
// External declarations for values
//*****************************************************************************
#include <vsol/vsol_spatial_object_2d.h>
#include <vsol/vsol_point_2d_sptr.h>
class vdgl_digital_curve;
class vsol_line_2d;
class vsol_curve_2d
  :public vsol_spatial_object_2d
{
public:
  enum vsol_curve_2d_type
  { CURVE_NO_TYPE=0,
    LINE,
    CONIC,
    DIGITAL_CURVE,
    NUM_CURVE_TYPES
  };

  //***************************************************************************
  // Initialization
  //***************************************************************************
public:
  //---------------------------------------------------------------------------
  //: Destructor
  //---------------------------------------------------------------------------
  virtual ~vsol_curve_2d();

  //---------------------------------------------------------------------------
  //: return the spatial type
  //---------------------------------------------------------------------------
  vsol_spatial_object_2d_type spatial_type(void) const { return vsol_spatial_object_2d::CURVE; }

  //---------------------------------------------------------------------------
  //: return the curve type
  //---------------------------------------------------------------------------
  virtual vsol_curve_2d_type curve_type(void) const { return vsol_curve_2d::CURVE_NO_TYPE; }

  //***************************************************************************
  // Access
  //***************************************************************************

  //***************************************************************************
  // Replaces dynamic_cast<T>
  //***************************************************************************

  //---------------------------------------------------------------------------
  //: Return `this' if `this' is a curve, 0 otherwise
  //---------------------------------------------------------------------------
  virtual vsol_curve_2d *cast_to_curve(void) {return this;}
  virtual const vsol_curve_2d *cast_to_curve(void) const {return this;}

  //---------------------------------------------------------------------------
  //: Return `this' if `this' is an digital_curve, 0 otherwise
  //---------------------------------------------------------------------------
  virtual vdgl_digital_curve const*cast_to_digital_curve(void)const{return 0;}
  virtual vdgl_digital_curve *cast_to_digital_curve(void) {return 0;}

  //---------------------------------------------------------------------------
  //: Return `this' if `this' is a line_2d, 0 otherwise
  //---------------------------------------------------------------------------
  virtual vsol_line_2d const*cast_to_line_2d(void)const{return 0;}
  virtual vsol_line_2d *cast_to_line_2d(void) {return 0;}

  //---------------------------------------------------------------------------
  //: Return the first point of `this'
  //---------------------------------------------------------------------------
  virtual vsol_point_2d_sptr p0(void) const=0;

  //---------------------------------------------------------------------------
  //: Return the last point of `this'
  //---------------------------------------------------------------------------
  virtual vsol_point_2d_sptr p1(void) const=0;

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
  virtual void set_p0(const vsol_point_2d_sptr &new_p0)=0;

  //---------------------------------------------------------------------------
  //: Set the last point of the curve
  //---------------------------------------------------------------------------
  virtual void set_p1(const vsol_point_2d_sptr &new_p1)=0;
};

#endif // #ifndef VSOL_CURVE_2D_H
