/*  -- translated by f2c (version of 23 April 1993  18:34:30).
   You must link the resulting object file with the libraries:
        -lf2c -lm   (in that order)
*/

#include "f2c.h"

integer izmax1_(n, cx, incx)
integer *n;
doublecomplex *cx;
integer *incx;
{
    /* System generated locals */
    integer ret_val, i__1, i__2;
    doublereal d__1;

    /* Local variables */
    static doublereal smax;
    static integer i, ix;


/*  -- LAPACK auxiliary routine (version 2.0) -- */
/*     Univ. of Tennessee, Univ. of California Berkeley, NAG Ltd., */
/*     Courant Institute, Argonne National Lab, and Rice University */
/*     September 30, 1994 */

/*     .. Scalar Arguments .. */
/*     .. */
/*     .. Array Arguments .. */
/*     .. */

/*  Purpose */
/*  ======= */

/*  IZMAX1 finds the index of the element whose real part has maximum */
/*  absolute value. */

/*  Based on IZAMAX from Level 1 BLAS. */
/*  The change is to use the 'genuine' absolute value. */

/*  Contributed by Nick Higham for use with ZLACON. */

/*  Arguments */
/*  ========= */

/*  N       (input) INTEGER */
/*          The number of elements in the vector CX. */

/*  CX      (input) COMPLEX*16 array, dimension (N) */
/*          The vector whose elements will be summed. */

/*  INCX    (input) INTEGER */
/*          The spacing between successive values of CX.  INCX >= 1. */

/* =====================================================================
*/

/*     .. Local Scalars .. */
/*     .. */
/*     .. Intrinsic Functions .. */
/*     .. */
/*     .. Statement Functions .. */
/*     .. */
/*     .. Statement Function definitions .. */

/*     NEXT LINE IS THE ONLY MODIFICATION. */
/*     .. */
/*     .. Executable Statements .. */

    /* Parameter adjustments */
    --cx;

    /* Function Body */
    ret_val = 0;
    if (*n < 1) {
        return ret_val;
    }
    ret_val = 1;
    if (*n == 1) {
        return ret_val;
    }
    if (*incx == 1) {
        goto L30;
    }

/*     CODE FOR INCREMENT NOT EQUAL TO 1 */

    ix = 1;
    smax = (d__1 = cx[1].r, abs(d__1));
    ix += *incx;
    i__1 = *n;
    for (i = 2; i <= i__1; ++i) {
        i__2 = ix;
        if ((d__1 = cx[i__2].r, abs(d__1)) <= smax) {
            goto L10;
        }
        ret_val = i;
        i__2 = ix;
        smax = (d__1 = cx[i__2].r, abs(d__1));
L10:
        ix += *incx;
/* L20: */
    }
    return ret_val;

/*     CODE FOR INCREMENT EQUAL TO 1 */

L30:
    smax = (d__1 = cx[1].r, abs(d__1));
    i__1 = *n;
    for (i = 2; i <= i__1; ++i) {
        i__2 = i;
        if ((d__1 = cx[i__2].r, abs(d__1)) <= smax) {
            goto L40;
        }
        ret_val = i;
        i__2 = i;
        smax = (d__1 = cx[i__2].r, abs(d__1));
L40:
        ;
    }
    return ret_val;

/*     End of IZMAX1 */

} /* izmax1_ */

