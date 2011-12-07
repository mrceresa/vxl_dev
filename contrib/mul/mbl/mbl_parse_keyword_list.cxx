#include "mbl_parse_keyword_list.h"
//:
// \file
// \brief Parse list of strings
// \author Tim Cootes

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
                            vcl_vector<vcl_string>& items,
                            bool discard_comments /* = false */)
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

    if (label == "}") 
      continue;

    else if ( discard_comments && (label.substr(0,2) == "//") )
    {
      // Comment line, so read to end
      vcl_string comment_string;
      vcl_getline(ss, comment_string);
      continue;
    }

    else if (label!=keyword)
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

//: Read in data from a stream, assumed to be a list of items
// Assumes list of objects separated by a keyword.
//  keyword is always the same word, defined in the input variable.
// Expects format of data:
// \verbatim
// {
//   keyword: string1 { stuff in braces }
// }
// \endverbatim
void mbl_parse_keyword_list2(vcl_istream& is, const vcl_string& keyword,
                             vcl_vector<vcl_string>& items,
                             bool discard_comments /* = false */)
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
  vcl_string label,string1,string2;
  while (!ss.eof())
  {
    ss >> label;         // Next follows the parameters

    if (label == "}") 
      continue;

    else if ( discard_comments && (label.substr(0,2) == "//") )
    {
      // Comment line, so read to end
      vcl_string comment_string;
      vcl_getline(ss, comment_string);
      continue;
    }

    if (label!=keyword)
    {
      vcl_string error_msg = "Expected keyword: '";
      error_msg+=keyword;
      error_msg+="' Got '";
      error_msg+=label;
      error_msg+="'";
      throw mbl_exception_parse_error(error_msg);
    }

    ss>>string1;
    string2=string1 + " " + mbl_parse_block(ss);

    items.push_back(string2);
  }
}


