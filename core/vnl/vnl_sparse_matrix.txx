#include <vnl/vnl_sparse_matrix.h>
#include <vcl/vcl_cassert.h>
#include <vcl/vcl_cstdlib.h>
#include <vcl/vcl_iostream.h>
      
// #define DEBUG_SPARSE 1

#ifdef DEBUG_SPARSE
# include <vnl/vnl_matrix.h>
#endif

//------------------------------------------------------------
// -- Construct an empty matrix
template <class T>
vnl_sparse_matrix<T>::vnl_sparse_matrix()
  : rs_(0), cs_(0)
{
}

//------------------------------------------------------------
// -- Construct an empty m*n matrix.  There are m rows and n columns.
template <class T>
vnl_sparse_matrix<T>::vnl_sparse_matrix(unsigned int m, unsigned int n)
  : elements(m), rs_(m), cs_(n)
{
}

//------------------------------------------------------------
// -- Construct an m*n Matrix and copy rhs into it.  
template <class T>
vnl_sparse_matrix<T>::vnl_sparse_matrix(const vnl_sparse_matrix<T>& rhs)
  : elements(rhs.elements), rs_(rhs.rs_), cs_(rhs.cs_)
{
}

//------------------------------------------------------------
// -- Copy another vnl_sparse_matrix<T> into this.
template <class T>
vnl_sparse_matrix<T>& vnl_sparse_matrix<T>::operator=(const vnl_sparse_matrix<T>& rhs)
{
  if (this == &rhs) 
    return *this;

  elements = rhs.elements;
  rs_ = rhs.rs_;
  cs_ = rhs.cs_;
  
  return *this;
}

//------------------------------------------------------------
// -- Multiply this*rhs, another sparse matrix.
template <class T>
void vnl_sparse_matrix<T>::mult(const vnl_sparse_matrix<T>& rhs, vnl_sparse_matrix<T>& result)
{
  assert(rhs.rows() == columns());
  unsigned int result_rows = rows();
  unsigned int result_cols = rhs.columns();
  
  // Clear result matrix.
  result.elements.clear();
  
  // Now give the result matrix enough rows.
  result.elements.resize(result_rows);
  result.rs_ = result_rows;
  result.cs_ = result_cols;
  
  // Now, iterate over non-zero rows of this.
  for (unsigned row_id=0; row_id<elements.size(); ++row_id) {
    // Get the row from this matrix (lhs).
    row& this_row = elements[row_id];
    
    // Skip to next row if empty.
    if (this_row.empty()) 
      continue;
    
    // Get the new row in the result matrix.
    row& result_row = result.elements[row_id];
    
    // Iterate over the row.
    for (row::iterator col_iter = this_row.begin();
	 col_iter != this_row.end();
	 ++col_iter)
      {
	// Get the element from the row.
	vnl_sparse_matrix_pair<T>& entry = *col_iter;
	unsigned const col_id = entry.first;
	
	// So we are at (row_id,col_id) = this_val in lhs matrix (this).
	// This must be multiplied by each entry in row col_id in
	// the rhs matrix, and the result added to result_row[col_id].
	
	// If that row in rhs is empty, there is nothing to do.
	row const & rhs_row = rhs.elements[col_id];
	if (rhs_row.empty()) 
	  continue;
	
	// Else iterate over rhs's row.
	row::iterator result_col_iter = result_row.begin();
	for (row::const_iterator rhs_col_iter = rhs_row.begin();
	     rhs_col_iter != rhs_row.end();
	     ++rhs_col_iter)
	  {
	    const vnl_sparse_matrix_pair<T>& rhs_entry = *rhs_col_iter;
	    unsigned int const dest_col = rhs_entry.first;
	    
	    // Calculate the product.
	    T prod = entry.second * rhs_entry.second;
	    
	    // This must be added into result_row, at column dest_col.
	    while ((result_col_iter != result_row.end()) &&
		   ((*result_col_iter).first < dest_col))
	      ++result_col_iter;
	    
	    if ((result_col_iter == result_row.end()) || 
		((*result_col_iter).first != dest_col)) {
	      // Add new column to the row.
	      result_col_iter =
		result_row.insert(result_col_iter,
				  vnl_sparse_matrix_pair<T>(dest_col,prod));
	    }
	    else {
	      // Else add product to existing contents.
	      (*result_col_iter).second += prod;
	    }
	  }
      }
  }
}

//------------------------------------------------------------
// -- Multiply this*p, a fortran order matrix.  The matrix p has n
// rows and m columns, and is in fortran order, ie. columns first.
template <class T>
void vnl_sparse_matrix<T>::mult(unsigned int prows, unsigned int pcols,
				T const* p, T* q)
{
  assert(prows == columns());

  // Clear q matrix.
  int size = prows*pcols;
  for (int temp=0; temp<size; temp++)
    q[temp] = T(0);

#ifdef DEBUG_SPARSE
  vnl_matrix<double> md(rows(),columns());
  for (int rr = 0; rr<rows(); rr++)
    for (int cc = 0; cc<columns(); cc++)
      md(rr,cc) = (*this)(rr,cc);

  vnl_matrix<double> pd(prows,pcols);
  for (int rr = 0; rr<prows; rr++)
    for (int cc = 0; cc<pcols; cc++)
      pd(rr,cc) = p[rr + cc*prows];

  cout << "Initial p: " << endl;
  for (int rr = 0; rr<prows; rr++) {
    for (int cc = 0; cc<pcols; cc++) {
      T pval = p[rr + cc*prows];
      cout << pval << " ";
    }
    cout << endl;
  }
#endif

  // Now, iterate over non-zero rows of this.
  for (unsigned row_id=0; row_id<elements.size(); ++row_id) {
    // Get the row from this matrix (lhs).
    row& this_row = elements[row_id];
    
    // Skip to next row if empty.
    if (this_row.empty()) 
      continue;
    
    // Iterate over the row.
    for (row::iterator col_iter = this_row.begin();
	 col_iter != this_row.end();
	 ++col_iter)
      {
	// Get the element from the row.
	vnl_sparse_matrix_pair<T>& entry = *col_iter;
	unsigned const col_id = entry.first;
	
	// So we are at (row_id,col_id) = this_val in lhs matrix
	// (this).  This must be multiplied by each entry in row
	// col_id in the p matrix, and the result added to
	// (row_id,p_col_id) in the q matrix.
	
	// Iterate over p's row.
	for (unsigned int p_col_id = 0; p_col_id < pcols; p_col_id++) {
	  // Get the correct position from p.
	  T pval = p[col_id + p_col_id*prows];
	  
	  // Calculate the product.
	  T prod = entry.second * pval;
	  
	  // Add the product into the correct position in q.
	  q[row_id + p_col_id*prows] += prod;
	}
      }
  }
  
#ifdef DEBUG_SPARSE
  cout << "Final q: " << endl;
  for (int rr = 0; rr<prows; rr++) {
    for (int cc = 0; cc<pcols; cc++) {
      T pval = q[rr + cc*prows];
      cout << pval << " ";
    }
    cout << endl;
  }
  cout << "nonsparse: " << md*pd << endl;
#endif
}


//------------------------------------------------------------
// -- Multiply this*rhs, a vector.
template <class T>
void vnl_sparse_matrix<T>::mult(const vnl_vector<T>& , vnl_vector<T>& )
{
  cerr << "Warning, vnl_sparse_matrix::mult not implemented...." << endl;
  abort();
}

//------------------------------------------------------------
// -- Multiply lhs*this, where lhs is a vector
template <class T>
void vnl_sparse_matrix<T>::pre_mult(const vnl_vector<T>& lhs, vnl_vector<T>& result)
{
  assert(lhs.size() == rows());

  // Resize and clear result vector
  vnl_vector<T> tmp( columns() );
  result = tmp;  // makes length same as tmp
  result = 0;    // initializes result to the zero vector
 
  // Now, iterate over lhs values and rows of rhs
  unsigned lhs_col_id = 0;
  for ( vcl_vector<row>::iterator rhs_row_iter = elements.begin(); 
        rhs_row_iter != elements.end();
        ++rhs_row_iter, lhs_col_id++ )
    {
      // Get the row from rhs matrix.
      row& rhs_row = *rhs_row_iter;
      
      // Skip to next row if empty.
      if (rhs_row.empty()) continue;
      
      // Iterate over values in rhs row
      for (row::iterator rhs_col_iter = rhs_row.begin();
	   rhs_col_iter != rhs_row.end();
	   ++rhs_col_iter)
	{
	  // Get the element from the row.
	  vnl_sparse_matrix_pair<T>& entry = *rhs_col_iter;
	  unsigned const rhs_col_id = entry.first;

          result[ rhs_col_id ] += lhs[ lhs_col_id ] * entry.second;
	}
    }
}

//------------------------------------------------------------
// -- Add rhs to this.
template <class T>
void vnl_sparse_matrix<T>::add(const vnl_sparse_matrix<T>& rhs,
			  vnl_sparse_matrix<T>& result)
{
  assert((rhs.rows() == rows()) && (rhs.columns() == columns()));
  
  // Clear result matrix.
  result.elements.clear();
  
  // Now give the result matrix enough rows.
  result.elements.resize(rows());
  result.rs_ = rows();
  result.cs_ = columns();
  
  // Now, iterate over non-zero rows of this.
  unsigned int row_id = 0;
  for (vcl_vector<row>::iterator row_iter = elements.begin(); 
       row_iter != elements.end();
       ++row_iter, ++row_id)
    {
      // Get the row from this matrix (lhs).
      row& this_row = *row_iter;

      // Get the new row in the result matrix.
      row& result_row = result.elements[row_id];

      // Store this into result row.
      result_row = this_row;
      
      // If rhs row is empty, we are done.
      if (rhs.empty_row(row_id))
	continue;

      // Get the rhs row.
      row const& rhs_row = rhs.elements[row_id];
      
      // Iterate over the rhs row.
      for (row::const_iterator col_iter = rhs_row.begin();
	   col_iter != rhs_row.end();
	   ++col_iter)
	{
	  // Get the element from the row.
	  vnl_sparse_matrix_pair<T> const& entry = *col_iter;
	  unsigned const col_id = entry.first;

	  // So we are at (row_id,col_id) in rhs matrix.
	  result(row_id,col_id) += entry.second;
	}
    }
}

//------------------------------------------------------------
// -- Subtract rhs from this.
template <class T>
void vnl_sparse_matrix<T>::subtract(const vnl_sparse_matrix<T>& rhs,
			       vnl_sparse_matrix<T>& result)
{
  assert((rhs.rows() == rows()) && (rhs.columns() == columns()));
  
  // Clear result matrix.
  result.elements.clear();
  
  // Now give the result matrix enough rows.
  result.elements.resize(rows());
  result.rs_ = rows();
  result.cs_ = columns();
  
  // Now, iterate over non-zero rows of this.
  unsigned int row_id = 0;
  for (vcl_vector<row>::iterator row_iter = elements.begin(); 
       row_iter != elements.end();
       ++row_iter, ++row_id)
    {
      // Get the row from this matrix (lhs).
      row& this_row = *row_iter;

      // Get the new row in the result matrix.
      row& result_row = result.elements[row_id];

      // Store this into result row.
      result_row = this_row;
      
      // If rhs row is empty, we are done.
      if (rhs.empty_row(row_id))
	continue;

      // Get the rhs row.
      row const& rhs_row = rhs.elements[row_id];
      
      // Iterate over the rhs row.
      for (row::const_iterator col_iter = rhs_row.begin();
	   col_iter != rhs_row.end();
	   ++col_iter)
	{
	  // Get the element from the row.
	  vnl_sparse_matrix_pair<T> const& entry = *col_iter;
	  unsigned const col_id = entry.first;

	  // So we are at (row_id,col_id) in rhs matrix.
	  result(row_id,col_id) -= entry.second;
	}
    }
}

//------------------------------------------------------------
// -- Get a reference to an entry in the matrix.
template <class T>
T& vnl_sparse_matrix<T>::operator()(unsigned int r, unsigned int c)
{
  assert((r < rows()) && (c < columns()));
  row& rw = elements[r];
  row::iterator ri;
  for (ri = rw.begin(); (ri != rw.end()) && ((*ri).first < c); ++ri);
  
  if ((ri == rw.end()) || ((*ri).first != c)) {
    // Add new column to the row.
    ri = rw.insert(ri, vnl_sparse_matrix_pair<T>(c,T(0)));
  }
  
  return (*ri).second;
}

//------------------------------------------------------------
// -- This is occasionally useful.  Sums a row of the matrix
// efficiently.
template <class T>
double vnl_sparse_matrix<T>::sum_row(unsigned int r)
{
  assert(r < rows());
  row & rw = elements[r];
  double sum = 0.0;
  for (row::iterator ri = rw.begin(); ri != rw.end(); ++ri)
    sum += (*ri).second;
  
  return sum;
}

//------------------------------------------------------------
// -- Resizes the matrix so that it has r rows and c columns.
//    Currently not implemented.
//   
template <class T>
void vnl_sparse_matrix<T>::resize( int /*r*/, int /*c*/)
{
  cerr << "Warning: vnl_sparse_matrix::resize not implemented." << endl;
  abort();
}

//------------------------------------------------------------
// -- Resets the internal iterator 
//
template <class T>
void vnl_sparse_matrix<T>::reset()
{
    itr_isreset = true;
    itr_row = 0;
}

//------------------------------------------------------------
// -- Moves the internal iterator to next non-zero entry in matrix.
// Returns true if there is another value, false otherwise. Use
// in combination with methods reset, getrow, getcolumn, and value.
//
template <class T>
bool vnl_sparse_matrix<T>::next()
{
  if ( itr_row >= rows() ) 
    return false;

  if ( itr_isreset ) {
    // itr_cur is not pointing to a entry
    itr_row = 0;
    itr_isreset = false;
  } else {              
    // itr_cur is pointing to an entry.
    // Try to move to next entry in current row.
    itr_cur++;        
    if ( itr_cur != elements[itr_row].end() )
      return true;  // found next entry in current row
    else
      itr_row++;
  }

  // search for next entry starting at row itr_row
  while ( itr_row < rows() ) {
    itr_cur = elements[itr_row].begin();
    if ( itr_cur != elements[itr_row].end() )
      return true;
    else 
      itr_row++;
  }    

  return itr_row < rows();
}
 
//------------------------------------------------------------
// -- Returns the row of the entry pointed to by internal iterator.
//
template <class T>
int vnl_sparse_matrix<T>::getrow()
{
    return itr_row;
}

//------------------------------------------------------------
// -- Returns the column of the entry pointed to by internal iterator.
//
template <class T>
int vnl_sparse_matrix<T>::getcolumn()
{
    return (*itr_cur).first;
}

//------------------------------------------------------------
// -- Returns the value pointed to by the internal iterator.
//
template <class T>
T vnl_sparse_matrix<T>::value()
{
    return (*itr_cur).second;
}
