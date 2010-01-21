// This is brl/bpro/core/brip_pro/brip_detect_edges_process.h
#ifndef brip_detect_edges_process_h_
#define brip_detect_edges_process_h_
//:
// \file
// \brief A process that takes a grayscale image and returns the corresponding edge map
//
// \author Ibrahim Eden
// \date March 05, 2008
// \verbatim
//  Modifications
//   Isabel Restrepo - Jan 27, 2009 - converted process-class to functions which is the new design for bvxm_processes.
//   Peter Vanroose  - Jul 10, 2009 - split into .h and .cxx
//   Gamze Tunali    - Jan 13, 2010 - moved from bvxm/pro following the move of brip/brip_edge.h
//
// \endverbatim

#include <bprb/bprb_func_process.h>
#include <vcl_string.h>

//: global variables
namespace brip_detect_edges_process_globals
{
  const unsigned n_inputs_ = 1;
  const unsigned n_outputs_=1;

  // parameter strings
  const vcl_string param_noise_multiplier_ =  "noise_multiplier";
  const vcl_string param_smooth_ =  "smooth";
  const vcl_string param_automatic_threshold_ =  "automatic_threshold";
  const vcl_string param_junctionp_ =  "junctionp";
  const vcl_string param_aggressive_junction_closure_ =  "aggressive_junction_closure";
}

//: initialize input and output types
bool brip_detect_edges_process_cons(bprb_func_process& pro);

//: generates the edge map
bool brip_detect_edges_process(bprb_func_process& pro);

#endif // brip_detect_edges_process_h_
