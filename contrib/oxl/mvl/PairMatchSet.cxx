#ifdef __GNUC__
#pragma implementation
#endif

#include "PairMatchSet.h"

#include <vcl_cstdlib.h>
#include <vcl_cassert.h>
#include <vcl_vector.h>

#include <mvl/MatchSet.h>

PairMatchSet::iterator& PairMatchSet::iterator::operator ++ (int /*postfix*/)
{
  vcl_abort();
  return *this;
}

// -- @{Construct a PairMatchSet for values of $i_1$ between 0 and \argfont{size}.@}
PairMatchSet::PairMatchSet(unsigned size):
  _matches(size)
{
  clear();
}

// -- Destructor
PairMatchSet::~PairMatchSet()
{
}

// @{ OPERATIONS @}
PairMatchSet::PairMatchSet(const PairMatchSet& that):
  _matches(that._matches),
  _match_count(that._match_count)
{
}
  
PairMatchSet& PairMatchSet::operator=(const PairMatchSet& that)
{
  _matches = that._matches;
  _match_count = that._match_count;
  return *this;
}

void PairMatchSet::update_feature_match_data()
{
  assert(false);
}

// -- Add the pair (i1, i2) to the match set.
// If i1 had an existing match it is overwritten.
bool PairMatchSet::add_match(int i1, int i2)
{
  if ((unsigned)i1 >= _matches.size()) {
    vcl_cerr << "PairMatchSet: add_match(" << i1 << ") greater than size " << _matches.size() << vcl_endl;
    abort();
    return false;
  }
  bool had_nomatch = (_matches[i1] == NoMatch);
  bool new_nomatch = (i2 == NoMatch);
  _matches[i1] = i2;
  if (had_nomatch && !new_nomatch)
    ++_match_count;
  return true;
}

// -- Remove any match for i1.  Specifically, remove tuples whose first element
// is i1.
void PairMatchSet::clear_match_1(int i1)
{
  if ((unsigned)i1 >= _matches.size()) {
    vcl_cerr << "PairMatchSet: clear squawwk\n";
    return;
  }
  int& i2 = _matches[i1];
  if (i2 != MatchSet::NoMatch)
    --_match_count;

  i2 = MatchSet::NoMatch;
}

// -- Return any match for i1.  Specifically, return the second element of any
// tuple whose first element is i1.
int PairMatchSet::get_match_12(int i1) const
{
  if (i1 == MatchSet::NoMatch)
    return MatchSet::NoMatch;

  if ((unsigned)i1 >= _matches.size()) {
    vcl_cerr << "PairMatchSet::get_match_12() -- i1 = " << i1 << " >= _matches.size() = " << _matches.size() << vcl_endl;
    abort();
    return -1;
  }
  
  return _matches[i1];
}

// -- Return any match for i2.  Specifically, return the first element of any
// tuple whose second element is i2.  This is currently O(n) in the number of
// matches, consider adding an index to this class if you use it a lot.
int PairMatchSet::get_match_21(int i2) const
{
  for(unsigned i = 0; i < _matches.size(); ++i)
    if (_matches[i] == i2)
      return i;
  return MatchSet::NoMatch;
}

// - Allows selection of a given match.  Use the iterator instead.
bool PairMatchSet::get_match(int c, int* i1, int* i2) const
{
  if (c >= size())
    return false;
  
  *i1 = c;
  *i2 = get_match_12(*i1);

  return true;
}

// -- Empty this match set.
void PairMatchSet::clear()
{
  for(unsigned i = 0; i < _matches.size(); ++i)
    _matches[i] = MatchSet::NoMatch;
  _match_count = 0;
}

// -- Set this match set to contain matches (i,i) for i=0..size
void PairMatchSet::set_identity()
{
  for(unsigned i = 0; i < _matches.size(); ++i)
    _matches[i] = i;
  _match_count = _matches.size();
}

// -- Count the number of matches in this set
int PairMatchSet::compute_match_count()
{
  _match_count = 0;
  for(unsigned i = 0; i < _matches.size(); ++i)
    if (_matches[i] != MatchSet::NoMatch)
      ++_match_count;
  
  return _match_count;
}

// -- Return the maximum allowed value of i1.
int PairMatchSet::size() const
{
  return _matches.size();
}

// -- Set the maximum allowed value of i1 to size.
void PairMatchSet::set_size(unsigned newsize)
{
  if (newsize != _matches.size())
    _matches = vcl_vector<int>(newsize, NoMatch);
}

void PairMatchSet::update(const vcl_vector<bool>& inliers)
{
  if (inliers.size() != count()) {
    vcl_cerr << "PairMatchSet::update() -- This matchset is not the same length as the inliers\n";
    abort();
  }

  int n = 0;
  for(PairMatchSet::iterator match (*this); match; match.next(), ++n)
    if (!inliers[n])
      clear_match_1(match.get_i1());
}
  
// Computations--------------------------------------------------------------

// Data Access---------------------------------------------------------------

// @{ INPUT/OUTPUT @}

// -- Write matches in ASCII to stream.
void PairMatchSet::write_ascii(vcl_ostream& s) const
{
  for (unsigned i = 0; i < _matches.size(); ++i) {
    int to_index = _matches[i];
    if (to_index != NoMatch)
      s << i << " " << to_index << vcl_endl;
  }
}

vcl_ostream& operator<<(vcl_ostream& s, const PairMatchSet& cc) {
  cc.write_ascii(s);
  return s;
}

bool PairMatchSet::read_ascii(vcl_istream& s)
{
  clear();
  for(;;) {
    // Read 2 ints
    int i1, i2;
    s >> i1 >> i2;
    // Break if that failed
    if (!s.good())
      break;
    // Eat any whitespace
    s >> vcl_ws;

    // Sanity check
    if (i1 < 0 || i2 < 0 || i1 >= (int)_matches.size()) {
      vcl_cerr << "PairMatchSet::read_ascii -- Pair " << i1 << "-" << i2 << " is outside the valid range." << vcl_endl;
      clear();
      return false;
    }
    
    // More sanity checking
    if (_matches[i1] != NoMatch) {
      vcl_cerr << "PairMatchSet::read_ascii() -- Warning:\n";
      vcl_cerr << "Duplicate matches for " << i1 << ": " << _matches[i1] << " and " << i2 << vcl_endl;
      return false;
    }

    _matches[i1] = i2;
  }

  return compute_match_count() > 0;
}

vcl_istream& operator>>(vcl_istream& s, PairMatchSet& cc) {
  cc.read_ascii(s);
  return s;
}


// -- Summarize matches on stream
void PairMatchSet::print_brief(vcl_ostream& s) const
{
  s << "PairMatchSet: ";
  for (unsigned i = 0; i < _matches.size(); i++)
    s << _matches[i] << " ";
  s << vcl_endl;
}

// -- Summarize matches on cout.
void PairMatchSet::print_brief() const
{
  unsigned n = _matches.size();
  char const *c = "";
  if (n > 30) {
    n = 30;
    c = "...";
  }

  for(unsigned i = 0; i < n; ++i)
    if (_matches[i] != NoMatch)
      vcl_cout << " " << _matches[i];
  vcl_cout << c << vcl_endl;
}

// Data Control--------------------------------------------------------------
// -----------------------------------------------------------------------------


// @{ ITERATOR [class PairMatchSet::iterator] @}

// -- Construct an empty iterator.
PairMatchSet::iterator::iterator(bool full_only):
  _c(0),
  _match_index(0),
  _full_only(full_only)
{
}

// -- Construct an iterator which will enumerate the matches in PairMatchSet cc.
PairMatchSet::iterator::iterator(const PairMatchSet& cc, bool full_only):
  _c(&cc),
  _match_index(0),
  _full_only(full_only)
{
  _match_index = -1;
  next();
}

// -- Set an iterator to enumerate the matches in PairMatchSet cc.
PairMatchSet::iterator& PairMatchSet::iterator::operator =(const PairMatchSet& cc)
{
  _c = &cc;
  _match_index = -1;
  next();
  return *this;
}

// -- Advance the iterator, returning false if all matches have been enumerated.
bool PairMatchSet::iterator::next()
{
  if (_full_only) {
    while (_c->get_match(++_match_index, &i1, &i2))
      if (isfull())
	return true;
    return false;
  }
  return _c->get_match(++_match_index, &i1, &i2);
}

/* insert these here for documentation purposes
  
// -- Return the first component of the match currently "pointed to" by
// the match iterator.
int PairMatchSet::iterator::get_i1() const
{
  return i1;
}

// -- Return the second component of the match currently "pointed to" by
// the match iterator.
int PairMatchSet::iterator::get_i2() const
{
  return i2;
}
*/

// -- Return true if neither of the indices of the match pointed to by the
// iterator is NoMatch.  [Users should not need this]
bool PairMatchSet::iterator::isfull() const
{
  return (i1 != NoMatch && i2 != NoMatch);
}

// -- Return true if the iterator has not yet enumerated all matches.
PairMatchSet::iterator::operator bool () const
{
  return _match_index < _c->size();
}
