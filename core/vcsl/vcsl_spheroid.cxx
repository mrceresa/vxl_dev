#include <vcsl/vcsl_spheroid.h>

#include <vcl/vcl_cassert.h>

//***************************************************************************
// Constructors/Destructor
//***************************************************************************

//---------------------------------------------------------------------------
// Default constructor: Clark_1866 spheroid
//---------------------------------------------------------------------------
vcsl_spheroid::vcsl_spheroid(void)
{
  set_from_std(clarke_1866);
}

//---------------------------------------------------------------------------
// Constructor from a standard spheroid
//---------------------------------------------------------------------------
vcsl_spheroid::vcsl_spheroid(const vcsl_std_spheroid new_std_spheroid)
{
  set_from_std(new_std_spheroid);
}

//---------------------------------------------------------------------------
// Copy constructor
//---------------------------------------------------------------------------
vcsl_spheroid::vcsl_spheroid(const vcsl_spheroid &other)
{
  _a=other._a;
  _b=other._b;
  _e=other._e;
  _f=other._f;
}

//---------------------------------------------------------------------------
// Destructor
//---------------------------------------------------------------------------
vcsl_spheroid::~vcsl_spheroid()
{
}

//***************************************************************************
// Status report
//***************************************************************************

//---------------------------------------------------------------------------
// Return the major axis of spheroid
//---------------------------------------------------------------------------
double vcsl_spheroid::a(void) const
{
  return _a;
}

//---------------------------------------------------------------------------
// Return the minor axis of spheroid
//---------------------------------------------------------------------------
double vcsl_spheroid::b(void) const
{
  return _b;
}

//---------------------------------------------------------------------------
// Return the eccentricity of spheroid
//---------------------------------------------------------------------------
double vcsl_spheroid::e(void) const
{
  return _e;
}

//---------------------------------------------------------------------------
// Return the flattening of spheroid
//---------------------------------------------------------------------------
double vcsl_spheroid::f(void) const
{
  return _f;
}

//***************************************************************************
// Status setting
//***************************************************************************

//---------------------------------------------------------------------------
// Set from a standard spheroid
//---------------------------------------------------------------------------
void vcsl_spheroid::set_from_std(const vcsl_std_spheroid new_std_spheroid)
{
   switch(new_std_spheroid)
    {
    case airy_1830:
      _a=6377563.396;
      _b=6356256.910;
      break;
    case australian_national:
    case south_american_1969:
      _a=6378160;
      _b=6356774.7192;
      break;
    case bessel_1841:
      _a=6377397.155;
      _b=6356078.9629;
      break;
    case clarke_1866:
      _a=6378206.4;
      _b=6356583.8;
      break;
    case clarke_1880:
      _a=6378249.145;
      _b=6356514.8696;
      break;
    case everest_1830:
      _a=6377276.34518;
      _b=6356075.41511;
      break;
    case grs_1980:
      _a=6378137;
      _b=6356752.3141;
      break;
    case international:
      _a=6378388;
      _b=6356911.9462;
      break;
    case modified_airy:
      _a=6377340.189;
      _b=6356034.446;
      break;
    case modified_everest:
      _a=6377304.063;
      _b=6356103.039;
      break;
    case wgs_1972:
      _a=6378135;
      _b=6356750.5;
      break;
    case wgs_1984:
      _a=6378137;
      _b=6356752.3142;
      break;
    default:
      assert(false); // Impossible
    }

  _f=(_a-_b)/_a;
  _e=sqrt(2*_f-_f*_f);
}

//---------------------------------------------------------------------------
// Set the major axis of spheroid
//---------------------------------------------------------------------------
void vcsl_spheroid::set_a(const double new_a)
{
  _a=new_a;
}

//---------------------------------------------------------------------------
// Set the minor axis of spheroid
//---------------------------------------------------------------------------
void vcsl_spheroid::set_b(const double new_b)
{
  _b=new_b;
}

//---------------------------------------------------------------------------
// Set the eccentricity of spheroid
//---------------------------------------------------------------------------
void vcsl_spheroid::set_e(const double new_e)
{
  _e=new_e;
}

//---------------------------------------------------------------------------
// Set the flattening of spheroid
//---------------------------------------------------------------------------
void vcsl_spheroid::set_f(const double new_f)
{
  _f=new_f;
}

//***************************************************************************
// Comparison
//***************************************************************************

//---------------------------------------------------------------------------
// Is `this' equal to `other' ?
//---------------------------------------------------------------------------
bool vcsl_spheroid::operator==(const vcsl_spheroid &other) const
{
  bool result;
  result=this==&other;
  if(!result)
    result=(_a==other._a)&&(_b==other._b)&&(_e==other._e)&&(_f==other._f);
  return result;
}

//***************************************************************************
// Duplication
//***************************************************************************

//---------------------------------------------------------------------------
// Assignement
//---------------------------------------------------------------------------
vcsl_spheroid &vcsl_spheroid::operator=(const vcsl_spheroid &other)
{
  if(this!=&other)
    {
      _a=other._a;
      _b=other._b;
      _e=other._e;
      _f=other._f;
    }
  return *this;
}
