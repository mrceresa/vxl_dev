#ifndef vnl_transpose_h_
#define vnl_transpose_h_
#ifdef __GNUC__
#pragma interface
#endif
// .NAME        vnl_transpose - Efficient matrix transpose.
// .LIBRARY     vnl
// .HEADER	Numerics Package
// .INCLUDE     vnl/vnl_transpose.h
// .FILE        vnl/vnl_transpose.cxx
//
// .SECTION Description
//    vnl_transpose is an efficient way to write C = vnl_transpose(A) * B.
//    The vnl_transpose class holds a reference to the original matrix
//    and when involved in an operation for which it has been specialized,
//    performs the operation without copying.
//
//    If the operation has not been specialized, the vnl_transpose performs
//    a copying conversion to a matrix, printing a message to stdout.
//    At that stage, the user may choose to implement the particular operation
//    or use vnl_transpose::asMatrix() to clear the warning.
//
//    NOTE: This is a reference class, so should be shorted-lived than the
//    matrix to which it refers.
//
// Author: Andrew W. Fitzgibbon, Oxford RRG, 23 Dec 96

#include <vcl/vcl_iostream.h>
#include <vnl/vnl_matops.h>
#include <vnl/vnl_fastops.h>

class vnl_transpose {
  const vnl_matrix<double>& M_;
public:

// -- Make a vnl_transpose object referring to matrix M
  vnl_transpose(const vnl_matrix<double>& M): M_(M) {}
  
// -- Noisily convert a vnl_transpose to a matrix
  operator vnl_matrix<double> () const {
    cerr << "vnl_transpose being converted to matrix -- help! I don't wanna go!\n";
    return M_.transpose();
  }

// -- Quietly convert a vnl_transpose to a matrix
  vnl_matrix<double> asMatrix () const {
    return M_.transpose();
  }

// -- Return M' * O
  vnl_matrix<double> operator* (const vnl_matrix<double>& O) {
    vnl_matrix<double> ret(M_.columns(), O.columns());
    vnl_fastops::AtB(M_, O, &ret);
    return ret;
  }

// -- Return M' * O
  vnl_vector<double> operator* (const vnl_vector<double>& O) {
    vnl_vector<double> ret(M_.columns());
    vnl_fastops::AtB(M_, O, &ret);
    return ret;
  }

// -- Return A * B'
  friend vnl_matrix<double> operator* (const vnl_matrix<double>& A, const vnl_transpose& B) {
    vnl_matrix<double> ret(A.rows(), B.M_.rows());
    vnl_fastops::ABt(A, B.M_, &ret);
    return ret;
  }

};

#endif   // DO NOT ADD CODE AFTER THIS LINE! END OF DEFINITION FOR CLASS vnl_transpose.
