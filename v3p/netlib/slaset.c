/* slaset.f -- translated by f2c (version of 4 June 1993  1:43:59).
   You must link the resulting object file with the libraries:
	-lf2c -lm   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int slaset_(char *uplo, integer *m, integer *n, real *alpha, 
	real *beta, real *a, integer *lda, ftnlen uplo_len)
{
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3;

    /* Local variables */
    static integer i, j;
    extern logical lsame_(char *, char *, ftnlen, ftnlen);


/*  -- LAPACK auxiliary routine (version 2.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     October 31, 1992 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  SLASET initializes an m-by-n matrix A to BETA on the diagonal and */
/*  ALPHA on the offdiagonals. */

/*  Arguments */
/*  ========= */

/*  UPLO    (input) CHARACTER*1 */
/*          Specifies the part of the matrix A to be set. */
/*          = 'U':      Upper triangular part is set; the strictly lower 
*/
/*                      triangular part of A is not changed. */
/*          = 'L':      Lower triangular part is set; the strictly upper 
*/
/*                      triangular part of A is not changed. */
/*          Otherwise:  All of the matrix A is set. */

/*  M       (input) INTEGER */
/*          The number of rows of the matrix A.  M >= 0. */

/*  N       (input) INTEGER */
/*          The number of columns of the matrix A.  N >= 0. */

/*  ALPHA   (input) REAL */
/*          The constant to which the offdiagonal elements are to be set. 
*/

/*  BETA    (input) REAL */
/*          The constant to which the diagonal elements are to be set. */

/*  A       (input/output) REAL array, dimension (LDA,N) */
/*          On exit, the leading m-by-n submatrix of A is set as follows: 
*/

/*          if UPLO = 'U', A(i,j) = ALPHA, 1<=i<=j-1, 1<=j<=n, */
/*          if UPLO = 'L', A(i,j) = ALPHA, j+1<=i<=m, 1<=j<=n, */
/*          otherwise,     A(i,j) = ALPHA, 1<=i<=m, 1<=j<=n, i.ne.j, */

/*          and, for all UPLO, A(i,i) = BETA, 1<=i<=min(m,n). */

/*  LDA     (input) INTEGER */
/*          The leading dimension of the array A.  LDA >= max(1,M). */

/* ===================================================================== 
*/

/*     .. Local Scalars .. */
/*     .. */
/*     .. External Functions .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    a_dim1 = *lda;
    a_offset = a_dim1 + 1;
    a -= a_offset;

    /* Function Body */
    if (lsame_(uplo, "U", 1L, 1L)) {

/*        Set the strictly upper triangular or trapezoidal part of the
 */
/*        array to ALPHA. */

	i__1 = *n;
	for (j = 2; j <= i__1; ++j) {
/* Computing MIN */
	    i__3 = j - 1;
	    i__2 = min(i__3,*m);
	    for (i = 1; i <= i__2; ++i) {
		a[i + j * a_dim1] = *alpha;
/* L10: */
	    }
/* L20: */
	}

    } else if (lsame_(uplo, "L", 1L, 1L)) {

/*        Set the strictly lower triangular or trapezoidal part of the
 */
/*        array to ALPHA. */

	i__1 = min(*m,*n);
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *m;
	    for (i = j + 1; i <= i__2; ++i) {
		a[i + j * a_dim1] = *alpha;
/* L30: */
	    }
/* L40: */
	}

    } else {

/*        Set the leading m-by-n submatrix to ALPHA. */

	i__1 = *n;
	for (j = 1; j <= i__1; ++j) {
	    i__2 = *m;
	    for (i = 1; i <= i__2; ++i) {
		a[i + j * a_dim1] = *alpha;
/* L50: */
	    }
/* L60: */
	}
    }

/*     Set the first min(M,N) diagonal elements to BETA. */

    i__1 = min(*m,*n);
    for (i = 1; i <= i__1; ++i) {
	a[i + i * a_dim1] = *beta;
/* L70: */
    }

    return 0;

/*     End of SLASET */

} /* slaset_ */

