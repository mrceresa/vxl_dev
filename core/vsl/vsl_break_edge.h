#ifndef vsl_break_edge_h_
#define vsl_break_edge_h_
#ifdef __GNUC__
#pragma interface
#endif
// .NAME vsl_break_edge
// .INCLUDE vsl/vsl_break_edge.h
// .FILE vsl_break_edge.cxx
// \author fsm@robots.ox.ac.uk

class vsl_edge;
#include <vcl_list.h>
#include <vcl_vector.h>

//: Break the given edge at the positions indicated.
//
// The first element of 'where' must be 0 and the last 
// must be in->size()-1.
//
// The output edges will be in the order of the edgelchain
// and the first (resp. last) output edge will share its
// start (resp. end) vertex with the start (resp. end)
// vertex of the given edge.
//
// The input edge is not modified in any way, though its
// endpoints will obviously have the reference counts
// increased.
void vsl_break_edge(vsl_edge const *in,
		    vcl_vector<unsigned> const &where,
		    vcl_list<vsl_edge*> *broken);

//: Break the given edge at points of "high curvature".
//
// This means edgels where the rms line fitting error over 
// a neighbourhood of the given size exceeds the given threshold.
//
// For a neighbourhood size of k,the number of edgels used in 
// each line fit is 2*k+1 (so the default is 5 edgels). The new 
// edges are pushed onto the end of the given vcl_list in the 
// obvious order.
void vsl_break_edge(vsl_edge const *in, 
		    vcl_list<vsl_edge*> *broken,
		    double threshold = 0.2,
		    unsigned nbhd_size = 2);

#endif // vsl_break_edge_h_
