#include <vcsl/vcsl_cartesian_2d.h>

#include <vcsl/vcsl_axis.h>

//***************************************************************************
// Constructors/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
// Default constructor
//---------------------------------------------------------------------------
vcsl_cartesian_2d::vcsl_cartesian_2d(void)
{
  vcsl_axis_ref a;
  a=new vcsl_axis;
  _axes.push_back(a);
  a=new vcsl_axis(*(a.ptr()));
  _axes.push_back(a);
  _right_handed=true;
}

//---------------------------------------------------------------------------
// Destructor
//---------------------------------------------------------------------------
vcsl_cartesian_2d::~vcsl_cartesian_2d()
{
}

//***************************************************************************
// Because VXL does not use dynamic_cast<> :-(
//***************************************************************************

const vcsl_cartesian_2d *vcsl_cartesian_2d::cast_to_cartesian_2d(void) const
{
  return this;
}

//***************************************************************************
// Status report
//***************************************************************************

//---------------------------------------------------------------------------
// Are the axes of `this' right handed ?
//---------------------------------------------------------------------------
bool vcsl_cartesian_2d::is_right_handed(void) const
{
  return _right_handed;
}

//***************************************************************************
// Status setting
//***************************************************************************

//---------------------------------------------------------------------------
// Set whether the coordinate system is right handed or not
//---------------------------------------------------------------------------
void vcsl_cartesian_2d::set_right_handed(const bool new_right_handed)
{
  _right_handed=new_right_handed;
}
