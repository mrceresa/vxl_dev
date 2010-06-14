//:
// \file
// \brief Parse list of strings
// \author Tim Cootes

#include <mbl/mbl_parse_keyword_list.h>

#include <mbl/mbl_exception.h>
#include <mbl/mbl_parse_block.h>
#include <vcl_sstream.h>


//: Read in data from a stream, assumed to be a list of items
// Assumes list of objects separated by a keyword.
//  keyword is always the same word, defined in the input variable.
// Expects format of data:
// \verbatim
// {
//   keyword: string1
//   keyword: { stuff in braces }
//   keyword: string3
// }
// \endverbatim
void mbl_parse_keyword_list(vcl_istream& is, const vcl_string& keyword,
                vcl_vector<vcl_string>& items)
{
  vcl_string s = mbl_parse_block(is);
  vcl_istringstream ss(s);
  char c;
  ss>>c;  // Remove opening brace
  if (c!='{')
  {
    throw mbl_exception_parse_error("Expected '{' in mbl_parse_keyword_list");
  }
  
  items.resize(0);
  vcl_string label;
  while (!ss.eof())
  {
    ss >> label;         // Next follows the parameters
    if (label == "}") continue;
    if (label!=keyword)
    {
      vcl_string error_msg = "Expected keyword: '";
      error_msg+=keyword;
      error_msg+="' Got '";
      error_msg+=label;
      error_msg+="'";
      throw mbl_exception_parse_error(error_msg);
    }

    items.push_back(mbl_parse_block(ss));
  }
}

