
#include <vcl/vcl_cstdlib.h>
#include <vcl/vcl_cstring.h>
#include <vcl/vcl_string.h>
#include <vbl/vbl_string.h>
#include <vbl/vbl_reg_exp.h>


#define TO_LOWER tolower                        // use ANSI functions
#define TO_UPPER toupper

#ifndef END_OF_STRING                           // If END_OF_STRING not defined
#define END_OF_STRING (0)
#endif

// Converts all alphabetical characters to uppercase.
char* c_upcase (char* s) {      // Convert entire string to upper case
  char* p = s;                  // Point to beginning of string
  while (*p) {                  // While there are still valid characters
    if (islower (*p))           // if this is lower case
      *p = TO_UPPER (*p);       // convert to uppercase
    p++;                        // Advance pointer
  }
  return s;                     // Return reference to modified string
}

// Converts all alphabetical characters to lowercase.
char* c_downcase (char* s) {    // Convert entire string to lower case
  char* p = s;                  // Point to beginning of string
  while (*p) {                  // While there are still valid characters
    if (isupper (*p))           // if this is upper case
      *p = TO_LOWER (*p);       // convert to lowercase
    p++;                        // Advance pointer
  }
  return s;                     // Return reference to modified string
}

// Capitalizes all words in a string. A word is defined as
// a sequence of characters separated by non-alphanumerics.
char* c_capitalize (char* s) {          // Capitalize each word in string
  char* p = s;                          // Point to beginning of string
  while (true) {                        // Infinite loop
    for (; *p && !isalnum(*p); p++);    // Skip to first alphanumeric
    if (*p == END_OF_STRING)            // If end of string
      return s;                         // Return string
    *p = TO_UPPER(*p);                  // Convert character
    while(*++p && isalnum (*p));        // Search for next word
  }
}

// Removes any occurrence of the string rem from string str,
// and returns the modified string str.
char* c_trim (char* str, const char* rem) {     // Trim characters from string
  char* s = str;
  char* result = str;
  register char c;
  while ((c=*s++) != END_OF_STRING) {
    register const char* r = rem;
    register char t;
    while ((t=*r++) != END_OF_STRING && t != c); // Scan for match
    if (t == END_OF_STRING)                      // If no match found
      *result++ = c;
  }
  *result = END_OF_STRING;                      // NULL terminate string
  return str;                                   // Return pointer to string
}

// Removes any prefix occurrence of the string rem from
// the first string str, and returns the modified string str.
char* c_left_trim (char* str, const char* rem) { // Trim prefix from string
  char* result = str;
  char* s;
  register char c;
  for (s=str; (c=*s) != END_OF_STRING; s++) {
    register const char* r = rem;
    register char t;
    while ((t=*r++) != END_OF_STRING && t != c); // Scan for match
    if (t == END_OF_STRING)                      // If no match found
      break;
  }
  if (s != result)                                // when characters trimed
    while ((*result++ = *s++) != END_OF_STRING); // shift string down
  return str;                                     // Return pointer to string
}

// Removes any suffix occurrence of the string rem
// from the first string str, and returns the modified string str.
char* c_right_trim (char* str, const char* rem) { // Trim suffix from string
  char* s = str + strlen(str) - 1;                // last character of str
  for (; s >= str; s--) {
    register const char* r = rem;
    register char t;
    register char c = *s;
    while ((t=*r++) != END_OF_STRING && t != c); // Scan for match
    if (t == END_OF_STRING)                      // If no match found
      break;
  }
  *(s+1) = END_OF_STRING;
  return str;                                     // Return pointer to string
}

// Reverses the order of the characters in char*.
void c_reverse (char* c) {                // Reverse the order of characters
  int length = strlen (c);              // Number of characters in string
  char temp;
 
  for (int i = 0, j = length-1;         // Counting from front and rear
       i < length / 2; i++, j--) {      // until we reach the middle
    temp = c[i];                        // Save front character
    c[i] = c[j];                        // Switch with rear character
    c[j] = temp;                        // Copy new rear character
  }
}





// Converts all alphabetical characters in string s to uppercase.
vcl_string& upcase(vcl_string& s)
{
  c_upcase(/*const_cast*/(char *)(s.c_str()));
  return s;                     // Return reference to modified string
}

// Converts all alphabetical characters in string s to lowercase.
vcl_string& downcase(vcl_string& s)
{
  c_downcase(/*const_cast*/(char *)(s.c_str()));
  return s;                     // Return reference to modified string
}

// Capitalizes all words in string s.
vcl_string& capitalize(vcl_string& s)
{
  c_capitalize(/*const_cast*/(char *)(s.c_str()));
  return s;                     // Return reference to modified string
}

// Removes any occurrence of the character string rem
// from the string sr, and returns the modified string sr.
vcl_string& trim(vcl_string& sr, const char* rem)
{
  int l = strlen(rem);
  for (;;) {
    vcl_string::size_type loc = sr.find(rem);
    if (loc == vcl_string::npos)
      break;
    sr.erase(loc, l);
  }
  return sr;
}

// Removes any prefix occurrence of the character string rem
// from the string sr, and returns the modified string sr.
vcl_string& left_trim(vcl_string& sr, const char* rem)
{
  int l = strlen(rem);
  if (strncmp(sr.c_str(), rem, l) == 0)
    sr.erase(0, l);
  return sr;
}

// Removes any suffix occurrence of the character string rem
// from the string sr, and returns the modified string sr.
vcl_string& right_trim(vcl_string& sr, const char* rem)
{
  int l = strlen(rem);
  int lsr = sr.length();
  if (strncmp(sr.c_str() + lsr - l, rem, l) == 0)
    sr.erase(lsr - l, l);
  return sr;
}

// // compile -- Compiles a given regular expression s so that it is
// // ready for pattern matching. Remember to recompile if the string is modified.
// void vbl_string_compile(const char* s) {
//   if (rgexp == NULL)                      // If no regexp object
//     rgexp = new vbl_reg_exp;              // Allocate storage for regexp
//   rgexp->compile (s);                     // Compile regular expression
//   rgexp_index = 0;                        // Reset search index position
// }
// 
// // find -- Searches the string for the previously compiled regular expression.
// // Returns true if a match is found.
// bool vbl_string_find (vcl_string& s) {
// #if ERROR_CHECKING
//   if (rgexp == NULL || !rgexp->is_valid()) // Valid regexp?
//     //this->find_error ();                        // Raise exception
// #endif
//   rgexp_index += rgexp->end ();     // Use index from last search
//   if (rgexp_index >= s.length())        // If at end of search
//     return false;                               // Indicate failure
//   bool result = rgexp->find (s.c_str() + rgexp_index);
//   return result;                                // Return match result
// }
// 
// long vbl_string_start () {
//   return (rgexp_index + rgexp->start ());
// }
// 
// // Returns the zero-relative star/end index of the last string
// // which matches the regular expression..
// long vbl_string_end () {
//   return (rgexp_index + rgexp->end ());
// }

int vbl_string_atoi(vcl_string const& s)
{
  return atoi(s.c_str());
}
