//:
// \file
// \brief Base for classes which locate feature points
// \author Tim Cootes


#include <mfpf/mfpf_point_finder.h>
#include <vsl/vsl_indent.h>
#include <vsl/vsl_binary_loader.h>
#include <vcl_cassert.h>

#include <mbl/mbl_parse_block.h>
#include <mbl/mbl_read_props.h>
#include <mbl/mbl_cloneables_factory.h>

//=======================================================================
// Dflt ctor
//=======================================================================

mfpf_point_finder::mfpf_point_finder()
{
}

//=======================================================================
// Destructor
//=======================================================================

mfpf_point_finder::~mfpf_point_finder()
{
}

//: Initialise from a string stream 
bool mfpf_point_finder::set_from_stream(vcl_istream &is)
{
  // Cycle through string and produce a map of properties
  vcl_string s = mbl_parse_block(is);  
  vcl_istringstream ss(s);
  mbl_read_props_type props = mbl_read_props_ws(ss);

  if (props.size()!=0)
  {
    vcl_cerr<<is_a()<<" does not expect any extra arguments."<<vcl_endl;
    mbl_read_props_look_for_unused_props(
      "mfpf_point_finder::set_from_stream", props, mbl_read_props_type());
  }
  return true;
}

//: Create a concrete object, from a text specification.
vcl_auto_ptr<mfpf_point_finder> mfpf_point_finder::
  create_from_stream(vcl_istream &is)
{
  vcl_string name;
  is >> name;
  vcl_auto_ptr<mfpf_point_finder> opt;
  try {
    opt = mbl_cloneables_factory<mfpf_point_finder>::get_clone(name);
  }
  catch (const mbl_exception_no_name_in_factory & e)
  {
    throw (mbl_exception_parse_error( e.what() ));
  }
  opt->set_from_stream(is);
  return opt;
}


//=======================================================================
// Method: version_no
//=======================================================================

short mfpf_point_finder::version_no() const
{
  return 1;
}

//=======================================================================
// Method: is_a
//=======================================================================

vcl_string mfpf_point_finder::is_a() const
{
  return vcl_string("mfpf_point_finder");
}

//: Allows derived class to be loaded by base-class pointer
void vsl_add_to_binary_loader(const mfpf_point_finder& b)
{
  vsl_binary_loader<mfpf_point_finder>::instance().add(b);
}

//=======================================================================
// Associated function: operator<<
//=======================================================================

void vsl_b_write(vsl_b_ostream& bfs, const mfpf_point_finder& b)
{
    b.b_write(bfs);
    
}

//=======================================================================
// Associated function: operator>>
//=======================================================================

void vsl_b_read(vsl_b_istream& bfs, mfpf_point_finder& b)
{
    b.b_read(bfs);
    
}

//=======================================================================
// Associated function: operator<< 
//=======================================================================

vcl_ostream& operator<<(vcl_ostream& os,const mfpf_point_finder& b)
{
  os << b.is_a() << ": ";
  vsl_indent_inc(os);
  b.print_summary(os);
  vsl_indent_dec(os);
  return os;
}

//=======================================================================
// Associated function: operator<< 
//=======================================================================

vcl_ostream& operator<<(vcl_ostream& os,const mfpf_point_finder* b)
{
    if (b)  
    return os << *b;
    else      
    return os << "No mfpf_point_finder defined.";
}
