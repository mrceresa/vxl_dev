//:
// \file
#include <testlib/testlib_test.h>

#include <bvpl/bvpl_corner2d_kernel_factory.h>
#include <bvpl/bvpl_edge3d_kernel_factory.h>
#include <bvpl/bvpl_edge_algebraic_mean_functor.h>
#include <bvpl/bvpl_edge_geometric_mean_functor.h>
#include <bvpl/bvpl_gauss_convolution_functor.h>
#include <bvpl/bvpl_neighb_operator.h>
#include <bvpl/bvpl_vector_operator.h>
#include <bvpl/bvpl_create_directions.h>
#include <vcl_sstream.h>
#include <vcl_iostream.h>
#include <vcl_iomanip.h>

#include <bvxm/grid/bvxm_opinion.h>
#include <bvxm/grid/bvxm_voxel_grid_basic_ops.h>
#include <bvxm/grid/bvxm_voxel_grid_opinion_basic_ops.h>
#include <bvxm/grid/io/bvxm_io_voxel_grid.h>

#include <vnl/vnl_vector_fixed.h>
#include <vul/vul_file.h>

typedef bsta_num_obs<bsta_gauss_f1> gauss_f1;

void create_window(bvxm_voxel_grid<gauss_f1> *grid)
{
  //fill in window grid
  vgl_vector_3d<unsigned> grid_dim = grid->grid_size();
  grid->initialize_data(bsta_gauss_f1(0.99f, 1.0f));
  unsigned ni=grid_dim.x();
  unsigned nj=grid_dim.y();
  unsigned nk=grid_dim.z();

  unsigned slab_idx = 0;
  bvxm_voxel_grid<gauss_f1>::iterator grid_it = grid->slab_iterator(slab_idx,nk);

  for (unsigned y= 0; y<grid_dim.y(); y++)
    for (unsigned z = grid_dim.z()/3 + 1; z < 2*grid_dim.z()/3 + 1; z++)
      for (unsigned x = grid_dim.x()/3 + 1; x < 2*grid_dim.x()/3 + 1; x++)
        (*grid_it)(x,y,z)=bsta_gauss_f1(0.01f, 1.0f);
}

bool test_result(bvxm_voxel_grid<gauss_f1> *grid, unsigned x, unsigned y, unsigned z)
{
  //iterate throungh the grid and get the max

    bvxm_voxel_grid<gauss_f1>::iterator grid_it = grid->begin();
    float max =  vcl_abs(((*grid_it)(0,0)).mean());
    float min =  vcl_abs(((*grid_it)(0,0)).mean());
    unsigned max_x =0;
    unsigned max_y =0;
    unsigned max_z =0;
    for (unsigned k=0; grid_it != grid->end(); ++grid_it, ++k) {
      for (unsigned i=0; i<(*grid_it).nx(); ++i) {
        for (unsigned j=0; j < (*grid_it).ny(); ++j) {
          if (vcl_abs(((*grid_it)(i,j)).mean())> max){
            max = vcl_abs(((*grid_it)(i,j)).mean());
            max_x = i; max_y = j; max_z = k;
          }
        }
      }
    }
    vcl_cout << "Location of max = " << max_x << max_y << max_z << vcl_endl;
    if ((x!=max_x)||(y!=max_y) || (z!=max_z))
      return false;

    bvxm_voxel_grid<gauss_f1>::iterator grid_it2 = grid->slab_iterator(0,grid->grid_size().z());
    vcl_cout << "Max response= " << vcl_abs(((*grid_it2)(max_x,max_y, max_z)).mean()) << vcl_endl;
    return true;
}

bool test_id_grid(bvxm_voxel_grid<unsigned> *grid,unsigned x, unsigned y, unsigned z, unsigned id)
{
  vgl_vector_3d<unsigned> grid_dim = grid->grid_size();
  unsigned nk=grid_dim.z();
  unsigned slab_idx = 0;
  bvxm_voxel_grid<unsigned>::iterator grid_it = grid->slab_iterator(slab_idx,nk);

  if ((*grid_it)(x,y,z)==id)
    return true;

  return false;
}

bool test_non_max_grid(bvxm_voxel_grid<gauss_f1> *grid)
{
  bvxm_voxel_grid<gauss_f1>::iterator grid_it = grid->begin();
  float max =  vcl_abs(((*grid_it)(0,0)).mean());
  float min =  vcl_abs(((*grid_it)(0,0)).mean());
  unsigned max_x =0;
  unsigned max_y =0;
  unsigned max_z =0;
  unsigned count = 0;
  for (unsigned k=0; grid_it != grid->end(); ++grid_it, ++k) {
    for (unsigned i=0; i<(*grid_it).nx(); ++i) {
      for (unsigned j=0; j < (*grid_it).ny(); ++j) {
        if (vcl_abs(((*grid_it)(i,j)).mean())> 1.0e-15){
          vcl_cout << "Response at " << i << ',' << j << ',' << k << " is " <<vcl_abs(((*grid_it)(i,j)).mean())
                   << vcl_endl;
          count++;
        }
      }
    }
  }
  if (count!=4)
    return false;
  return true;
}


MAIN(test_detect_corner)
{
  //Create vector of kernels
  unsigned length = 1;
  unsigned width = 1;
  unsigned thickness = 2;

  bvpl_kernel_vector_sptr kernel_vector= new bvpl_kernel_vector();
  bvpl_corner2d_kernel_factory factory(length, width, thickness);
  factory.set_rotation_axis( vnl_float_3(0, 1, 0));
  factory.set_angle(0.0f);
  bvpl_kernel_sptr kernel = new bvpl_kernel(factory.create());
  kernel_vector->kernels_.push_back(kernel);

  //Create a synthetic world, with a window
  bvxm_voxel_grid<gauss_f1> *grid = new bvxm_voxel_grid<gauss_f1> (vgl_vector_3d<unsigned>(9,5,9));
  create_window(grid);

  bvxm_voxel_grid<gauss_f1> *grid_out = new bvxm_voxel_grid<gauss_f1> (vgl_vector_3d<unsigned>(9,5,9));
  grid_out->initialize_data(bsta_gauss_f1(0.0f,1.0f));

  //Run apply kernel to world
  bvpl_gauss_convolution_functor func(kernel->iterator());
  bvpl_neighb_operator<gauss_f1, bvpl_gauss_convolution_functor> oper(func);
  oper.operate(grid, kernel, grid_out);

  //run several kernels and determine which window is being found
  TEST("Position Corner1", true, test_result(grid_out,6,2,6));

  factory.set_angle(float(vnl_math::pi_over_2));
  kernel = new bvpl_kernel(factory.create());
  kernel_vector->kernels_.push_back(kernel);
  oper.operate(grid, kernel, grid_out);
  TEST("Position Corner2", true, test_result(grid_out,4,2,6));

  factory.set_angle(float(vnl_math::pi));
  kernel = new bvpl_kernel(factory.create());
  kernel_vector->kernels_.push_back(kernel);
  oper.operate(grid, kernel, grid_out);
  TEST("Position Corner3", true, test_result(grid_out,4,2,4));

  factory.set_angle(float(3.0*vnl_math::pi_over_2));
  kernel = new bvpl_kernel(factory.create());
  kernel_vector->kernels_.push_back(kernel);
  oper.operate(grid, kernel, grid_out);
  TEST("Position Corner 4", true, test_result(grid_out,6,2,4));

  //create a functor
  bvpl_gauss_convolution_functor gauss_func(kernel->iterator());
  bvxm_voxel_grid<unsigned > *id_grid=new bvxm_voxel_grid<unsigned >(grid->grid_size());
  bvpl_vector_operator vector_oper;
  vector_oper.apply_and_suppress(grid,kernel_vector,&oper,grid_out,id_grid);
  TEST("Id at Corner 1", true, test_id_grid(id_grid,6,2,6,0));
  TEST("Id at Corner 2", true, test_id_grid(id_grid,4,2,6,1));
  TEST("Id at Corner 3", true, test_id_grid(id_grid,4,2,4,2));
  TEST("Id at Corner 4", true, test_id_grid(id_grid,6,2,4,3));

  //test non-maxima suppression
  //bvxm_voxel_grid<gauss_f1> *non_max_grid= new bvxm_voxel_grid<gauss_f1>(grid->grid_size());
  //bvxm_voxel_grid_copy<gauss_f1> (grid_out, non_max_grid);
  vector_oper.non_maxima_suppression(grid_out,id_grid,kernel_vector);
  TEST("Number of corners after non_max", true, test_non_max_grid(grid_out));
  return 0;
}
