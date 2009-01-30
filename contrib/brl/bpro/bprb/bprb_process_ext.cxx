// This is brl/bpro/bprb/bprb_process_ext.cxx
#include "bprb_process_ext.h"
//:
// \file

#include <bprb/bprb_parameters.h>
#include <vcl_iostream.h>


bprb_process_ext::bprb_process_ext()
: bprb_process()
{
}

bool bprb_process_ext::set_output_types(vcl_vector<vcl_string> const& types)
{
  output_types_ = types;
  output_data_.resize(types.size());
  return true;
}

bool bprb_process_ext::set_output(unsigned i, brdb_value_sptr val)
{
  if (output_data_.size() < i)
    return false;

  output_data_[i] = val;
  return true;
}

bool bprb_process_ext::set_input_types(vcl_vector<vcl_string> const& types)
{
  input_types_ = types;
  input_data_.resize(types.size());
  return true;
}

//: set a particular input, resize array if necessary
bool bprb_process_ext::set_input(unsigned i, brdb_value_sptr const& value)
{
  if (!value){
    vcl_cout << "bprb_process_ext::set_input() - null value\n";
    return false;
  }

  if(input_data_.size()<=i)
    input_data_.resize(i+1);
  input_data_[i]=value;
  return true;
}

//: Set all the inputs at once
bool bprb_process_ext::set_input_data(vcl_vector<brdb_value_sptr> const& inputs)
{
  for (unsigned i = 0; i<inputs.size(); ++i)
    if (!set_input(i, inputs[i]))
      return false;
  return true;
}
