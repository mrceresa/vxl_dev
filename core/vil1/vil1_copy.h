#ifndef vil_copy_h_
#define vil_copy_h_

// Author: awf@robots.ox.ac.uk
// Created: 16 Feb 00

#include <vil/vil_fwd.h>

//: Copy SRC to DST.
// Images must be exactly the same dimensions, but datatype isn't matched
void vil_copy(vil_generic_image const* SRC, vil_generic_image* DST);

#endif   // DO NOT ADD CODE AFTER THIS LINE! END OF DEFINITION FOR CLASS vil_copy.
