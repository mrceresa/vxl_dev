#ifndef vsl_easy_canny_h_
#define vsl_easy_canny_h_
#ifdef __GNUC__
#pragma interface "vsl_easy_canny"
#endif
/*
  fsm@robots.ox.ac.uk
*/

#include <vcl/vcl_list.h>
class vsl_edge;
class vil_image;

// 0: oxford
// 1: rothwell1
// 2: rothwell2
void vsl_easy_canny(int which_canny, 
		    vil_image const &image, 
		    vcl_list<vsl_edge*> *edges,
		    double sigma = 0);

#endif
