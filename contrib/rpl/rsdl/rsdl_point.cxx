
#include <vcl_cassert.h>
#include <vcl_iostream.h>
#include <vnl/vnl_vector.h>

#include <rsdl/rsdl_point.h>

rsdl_point::rsdl_point( )
  : Nc_(0), Na_(0), data_(0)
{
}

rsdl_point::rsdl_point( unsigned int Nc, unsigned int Na )
  : Nc_(Nc), Na_(Na), data_( new double[ Nc_ + Na_ ] )
{
}

rsdl_point::rsdl_point( const_iterator c_begin, const_iterator c_end,
                        const_iterator a_begin, const_iterator a_end )
  : Nc_( c_end - c_begin ), Na_( a_end - a_begin ), data_( new double[ Nc_ + Na_ ] )
{
  for ( unsigned int i=0; i<Nc_; ++i ) data_[ i ] = * ( c_begin + i );
  for ( unsigned int j=0; j<Na_; ++j ) data_[ Nc_ +j ]= * ( a_begin + j );
}

 
rsdl_point::rsdl_point( const vnl_vector<double>& c, const vnl_vector<double>& a )
  : Nc_( c.size() ), Na_( a.size() ),  data_( new double[ Nc_ + Na_ ] )
{
  for ( unsigned int i=0; i<Nc_; ++i ) data_[ i ] = c[i];
  for ( unsigned int j=0; j<Na_; ++j ) data_[ Nc_ + j ]= a[j];
}

 
rsdl_point::rsdl_point( const rsdl_point& old )
  : Nc_( old.Nc_ ), Na_( old.Na_ ), data_( new double[ Nc_ + Na_ ] )
{
  for ( unsigned int i=0; i< Nc_ + Na_ ; ++i ) data_[i] = old.data_[i];
}


rsdl_point::rsdl_point( const vnl_vector<double>& all, unsigned int Na )
  : Nc_( all.size() - Na ), Na_( Na )
{
  assert( Nc_ >= 0 );
  data_ = new double[ Nc_ + Na_ ];
  for ( unsigned int i=0; i< Nc_ + Na_; ++i )
    data_[i] = all[i];
}

rsdl_point::~rsdl_point()
{
  delete [] data_;
}

 
void 
rsdl_point::set_cartesian( const vnl_vector<double>& c )
{
  assert ( c.size() == Nc_ );
  for ( unsigned int i=0; i<Nc_; ++i )
    data_[i] = c[i];
}


void 
rsdl_point::set_cartesian( const_iterator c )
{
  for ( unsigned int i=0; i<Nc_; ++i, ++c )
    data_[i] = *c;
}


void 
rsdl_point::set_angular( const vnl_vector<double>& a )
{
  assert ( a.size() == Na_ );
  for ( unsigned int i=0; i<Na_; ++i )
    data_[ Nc_ + i ] = a[i];
}

//: \brief  Establish the cartesian and angular values from a vnl_vector.  Size is checked.
void 
rsdl_point::set_all( const vnl_vector<double>& all )
{
   assert ( all.size() == Nc_ + Na_ );
   for ( unsigned int i=0; i < Nc_ + Na_; ++i )
     data_[i] = all[i];
}


void 
rsdl_point::set_angular( const_iterator a )
{
  for ( unsigned int i=0; i<Na_; ++i, ++a )
    data_[Nc_+i] = *a;
}


rsdl_point &
rsdl_point::operator= ( const rsdl_point& old )
{
  if ( this != &old ) {
    if ( Nc_ + Na_ != old.Nc_ + old.Na_ ) {
      delete [] data_;
      data_ = new double[ old.Nc_ + old.Na_ ];
    }
    Nc_ = old.Nc_;
    Na_ = old.Na_;
    for ( unsigned int i=0; i < Nc_ + Na_; ++i ) data_[i] = old.data_[i];
  }
  return *this;
}

void
rsdl_point::resize( unsigned int Nc, unsigned int Na )
{
  if ( Nc_ != Nc || Na_ != Na ) {
    double* buf = new double[ Nc + Na ];
    unsigned int min_c = ( Nc < Nc_ ? Nc : Nc_ );
    unsigned int min_a = ( Na < Na_ ? Na : Na_ );
    for( unsigned int i=0; i < min_c; ++i )  buf[i] = data_[i];
    for( unsigned int i=0; i < min_a; ++i )  buf[Nc+i] = data_[Nc_+i];
    delete[] data_;
    data_ = buf;
    Nc_ = Nc;
    Na_ = Na;
  }
}

vcl_ostream& operator<< ( vcl_ostream& ostr, const rsdl_point& pt )
{
  int Nc = pt.num_cartesian();
  int Na = pt.num_angular();
  int i;
  vcl_cout << "Cartesian: [";
  for ( i=0; i<Nc; ++i ) {
    vcl_cout << pt.cartesian(i);
    if ( i<Nc-1 ) vcl_cout << ",";
  }
  vcl_cout << "];  Angular: [";
  for ( i=0; i<Na; ++i ) {
    vcl_cout << pt.angular(i);
    if ( i<Na-1 ) vcl_cout << ",";
  }
  vcl_cout << "]";
  return ostr;
}

