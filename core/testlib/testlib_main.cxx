#include "testlib_register.h"

#include <vcl_iostream.h>
#include <vcl_string.h>
#include <vcl_vector.h>

vcl_vector<TestMainFunction> testlib_test_func_;
vcl_vector<vcl_string>       testlib_test_name_;

void
list_test_names( vcl_ostream& ostr )
{
  ostr << "The registered test names are:\n";
  for( unsigned int i = 0; i < testlib_test_name_.size(); ++i ) {
    ostr << "   " << testlib_test_name_[i] << "\n";
  }
}

int
testlib_main( int argc, char* argv[] )
{
  // The caller should already have called register_tests().

  // Assume the index type for vector<string> and
  // vector<TestMainFunction> are the same.
  typedef vcl_vector<vcl_string>::size_type vec_size_t;

  // If a test name is given, try to run it. Otherwise, try to run all
  // the tests. The first argument, if available, is assumed to be a
  // test name.

  if( argc >= 2 ) {
    if( testlib_test_func_.size() != testlib_test_name_.size() ) {
      vcl_cerr << "Error: " << testlib_test_func_.size() << " test functions are registered, but "
               << testlib_test_name_.size() << " test names are registered." << vcl_endl;
      return 1;
    }

    for( vec_size_t i = 0; i < testlib_test_name_.size(); ++i ) {
      if( testlib_test_name_[i] == argv[1] ) {
        return testlib_test_func_[i]( argc-1, argv+1 );
      }
    }

    vcl_cerr << "Test " << argv[1] << " not registered.\n";
    list_test_names( vcl_cerr );
  } else {
    vcl_cout << "Test name not provided. Attempting to run all tests.\n";

    bool all_pass = true;
    for( vec_size_t i = 0; i < testlib_test_name_.size(); ++i ) {
      vcl_cout << "----------------------------------------\n"
               << "Running: " << testlib_test_name_[i] << "\n"
               << "----------------------------------------" << vcl_endl;
      int result = testlib_test_func_[i]( argc, argv );
      vcl_cout << "----------------------------------------\n"
               << testlib_test_name_[i] << " returned " << result << " " << ( result==0?"(PASS)":"(FAIL)") << "\n"
               << "----------------------------------------" << vcl_endl;
      all_pass &= (result == 0);
    }

    vcl_cout << "\n\nCombined result: " << ( all_pass?"PASS":"FAIL" ) << vcl_endl;
    return all_pass ? 0 : 1;
  }

  return 1;
}
