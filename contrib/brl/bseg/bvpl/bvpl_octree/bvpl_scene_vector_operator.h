// This is brl/bseg/bvpl/bvpl_octree/bvpl_scene_vector_operator.h
#ifndef bvpl_scene_vector_operator_h
#define bvpl_scene_vector_operator_h
//:
// \file
// \brief  A class that applies a vector of bvpl_kernels on a boxm_scene
// \author Isabel Restrepo mir@lems.brown.edu
// \date  November 30, 2009
//
// \verbatim
//  Modifications
//   <none yet>
// \endverbatim

#include "bvpl_octree_sample.h"
#include <bvpl/bvpl_kernel.h>
#include <bvpl/bvpl_local_max_functor.h>
#include <boxm/boxm_scene.h>
#include <vnl/vnl_vector_fixed.h>
#include <vcl_iostream.h>

class bvpl_scene_vector_operator
{
 public:

  //: "Convolves" a vector of kernel with an input octree, storing the output in an output octree.
  //  The shape of the output is deterrmined by a templated function
  //  This function only iterate through leaf_cells at level 0;
  template<class T_data_in, class T_data_out, class F >
  void operate(boxm_scene<boct_tree<short, T_data_in> > &scene_in,
               F functor,
               bvpl_kernel_vector_sptr kernel_vector,
               boxm_scene<boct_tree<short, T_data_out > > &scene_out);

#if 0 //Deprecated
  //: Main interface function. Applies a vector of bvpl_kernels on a boxm_scene. Combines the responses on a single scene
  template<class T_data, class F >
  bool operate(boxm_scene<boct_tree<short, T_data> > &scene_in,
               bvpl_octree_vector_operator<T_data> * vec_opertor,
               F functor,
               bvpl_kernel_vector_sptr kernel_vector,
               boxm_scene<boct_tree<short, bvpl_octree_sample<T_data> > > &scene_out,
               short level=0)
  {
    // iterate through the scene
    boxm_block_iterator<boct_tree<short, T_data> > iter_in = scene_in.iterator();
    boxm_block_iterator<boct_tree<short, bvpl_octree_sample<T_data> > > iter_out = scene_out.iterator();
    iter_in.begin();
    iter_out.begin();

    bvpl_local_max_functor<T_data> func_max;

    for (; !iter_in.end(); iter_in++, iter_out++) {
      scene_in.load_block(iter_in.index());
      scene_out.load_block(iter_out.index());
      boct_tree<short, T_data>  *tree_in= (*iter_in)->get_tree();
      boct_tree<short, bvpl_octree_sample<T_data> >  *tree_out = tree_in ->template clone_to_type<bvpl_octree_sample<T_data> >();
      tree_out->init_cells(bvpl_octree_sample<T_data>());
      double cell_length = 1.0/(double)(1<<(tree_in->root_level() -level));
      //at each tree, run the vector of kernels
      vec_opertor->operate(tree_in, functor, kernel_vector, tree_out, level, cell_length);
      (*iter_out)->init_tree(tree_out);
      scene_out.write_active_block();
    }
    return true;
  }
#endif

 protected:

  //:Function incharged of combining results
  template<class T_data>
  void combine_kernel_responses(const vcl_vector<T_data>& responses, vnl_vector_fixed<T_data, 3> &response_out);

  //:Function incharged of combining results
  template<class T_data>
  void combine_kernel_responses(const vcl_vector<T_data>& responses, bvpl_octree_sample<T_data> &response_out);
};


//: Operates a vector of kernels on a scene by : (1)Traversing input scene and for every leaf cell, (2) request a region around it, and (3) apply the functor
template<class T_data_in, class T_data_out,class F >
void bvpl_scene_vector_operator::operate(boxm_scene<boct_tree<short, T_data_in> > &scene_in,
                                         F functor,
                                         bvpl_kernel_vector_sptr kernel_vector,
                                         boxm_scene<boct_tree<short, T_data_out > > &scene_out)
{
  short finest_level = scene_in.finest_level();
#ifdef DEBUG
  vcl_cout << "bvpl_scene_kernel_operator: Operating on cells at level: " << finest_level << " and length: " << cell_length << vcl_endl;
#endif

  scene_in.template clone_blocks_to_type<boct_tree<short, T_data_out > >(scene_out, T_data_out());

  //(1) Traverse the scene - is there an easy way to modify the cell iterator so to only use leaf cells at level 0;
  boxm_cell_iterator<boct_tree<short, T_data_in > > iterator = scene_in.cell_iterator(&boxm_scene<boct_tree<short, T_data_in > >::load_block_and_neighbors);
  iterator.begin();

  boxm_cell_iterator<boct_tree<short, T_data_out > > out_iter = scene_out.cell_iterator(&boxm_scene<boct_tree<short, T_data_out > >::load_block);
  out_iter.begin();

  while ( !(iterator.end() || out_iter.end()) )
  {
    boct_tree_cell<short,T_data_in> *center_cell = *iterator;
    boct_tree_cell<short,T_data_out> *out_center_cell = *out_iter;
    boct_loc_code<short> out_code = out_center_cell->get_code();
    boct_loc_code<short> in_code = center_cell->get_code();

    //if level and location code of cells isn't the same then continue
    if ((center_cell->level() != out_center_cell->level()) || !(in_code.isequal(&out_code))){
      vcl_cerr << " Input and output cells don't have the same structure\n";
      ++iterator;
      ++out_iter;
      continue;
    }

    //we are only interested in finest resolution
    if ((!(center_cell->level() == finest_level)) || !center_cell->is_leaf()){
      ++iterator;
      ++out_iter;
      continue;
    }

    vgl_point_3d<double> center_cell_origin = iterator.global_origin();

    //(2) Run vector of kernels

    vcl_vector<T_data_in> responses;
    for (unsigned k= 0; k< kernel_vector->kernels_.size(); k++)
    {
      bvpl_kernel_sptr kernel = kernel_vector->kernels_[k];
      double cell_length = kernel->voxel_length();
      bvpl_kernel_iterator kernel_iter = kernel->iterator();
      kernel_iter.begin(); // reset the kernel iterator
      while (!kernel_iter.isDone())
      {
        vgl_point_3d<int> kernel_idx = kernel_iter.index();

        vgl_point_3d<double> kernel_cell_origin(center_cell_origin.x() + (double)kernel_idx.x()*cell_length + 1.0e-7,
                                                center_cell_origin.y() + (double)kernel_idx.y()*cell_length + 1.0e-7,
                                                center_cell_origin.z() + (double)kernel_idx.z()*cell_length + 1.0e-7);

        boct_tree_cell<short,T_data_in> *this_cell = scene_in.locate_point_in_memory(kernel_cell_origin);

        if (this_cell) {
          bvpl_kernel_dispatch d = *kernel_iter;
          T_data_in val = this_cell->data();
          functor.apply(val, d);
        }
        else {
          break;
        }

        ++kernel_iter;
      }

      responses.push_back(functor.result());
        //do something to either choose max or send all responses to vector
    }

    T_data_out data_out;
    combine_kernel_responses(responses, data_out);
    out_center_cell->set_data(data_out);
    ++iterator;
    ++out_iter;
  }
  return;
}

template <class T_data>
void bvpl_scene_vector_operator::combine_kernel_responses(const vcl_vector<T_data>& responses, vnl_vector_fixed<T_data, 3> &response_out)
{
  if (responses.size() != 3){
    vcl_cerr << "Wrong number of responses\n";
    return;
  }

  for (unsigned i = 0 ; i < 3; i++)
  {
    response_out[i] = responses[i];
  }
  return;
}

template <class T_data>
void bvpl_scene_vector_operator::combine_kernel_responses(const vcl_vector<T_data>& responses, bvpl_octree_sample<T_data> &response_out)
{
  if (responses.size() != 3){
    vcl_cerr << "Wrong number of responses\n";
    return;
  }

  bvpl_local_max_functor<T_data> func_max;

  T_data max= responses[0];
  int max_id =0;
  for (unsigned i = 1 ; i < responses.size(); i++)
  {
    if (func_max.greater_than(max, responses[i])){
      max = responses[i];
      max_id = i;
    }
  }
  response_out.response_=max;
  response_out.id_ = max_id;
  return;
}

#endif // bvpl_scene_vector_operator_h
