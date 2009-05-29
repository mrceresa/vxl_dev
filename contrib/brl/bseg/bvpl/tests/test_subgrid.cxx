//:
// \file
#include <testlib/testlib_test.h>
#include <bvpl/bvpl_subgrid_iterator.h>
#include <bvpl/bvpl_voxel_subgrid.h>

#include <vul/vul_file.h>

//: Test changes
static void test_subgrid()
{
  // create the grid
// we need temporary disk storage for this test.
  vcl_string storage_fname("bvxm_voxel_grid_test_temp.vox");
  vcl_string storage_cached_fname("bvxm_voxel_grid_cached_test_temp.vox");
  // remove file if exists from previous test.
  if (vul_file::exists(storage_fname.c_str())) {
    vul_file::delete_file_glob(storage_fname.c_str());
  }
  if (vul_file::exists(storage_cached_fname.c_str())) {
    vul_file::delete_file_glob(storage_cached_fname.c_str());
  }

  vgl_vector_3d<unsigned> grid_size(50,50,20);
  unsigned max_cache_size = grid_size.x()*grid_size.y()*18;

  // try test with all types of underlying storage.
  bvxm_voxel_grid<float>* grid = new bvxm_voxel_grid<float>(storage_fname,grid_size); // disk storage;
  vcl_string test_name;

  
    // check num_observations
    unsigned nobs = grid->num_observations();

    // fill with test data
    float init_val = 0.5f;
    grid->initialize_data(init_val);
    bool init_check = true;
    bool write_read_check = true;


    grid->increment_observations();
  
    // read in each slice, check that init_val was set, and fill with new value
    unsigned count = 0;
    vcl_cout << "read/write: ";
    bvxm_voxel_grid<float>::iterator slab_it;
    for (slab_it = grid->begin(); slab_it != grid->end(); ++slab_it) {
      vcl_cout << '.';
      bvxm_voxel_slab<float>::iterator vit;
      for (vit = slab_it->begin(); vit != slab_it->end(); vit++, count++) {
        if (*vit != init_val) {
          vcl_cerr << "error: read in value does not match init value! count = " << count << vcl_endl;
          init_check = false;
        }
        // write new value
        *vit = static_cast<float>(count);
      }
    }
    vcl_cout << "done." << vcl_endl;


  // get the subgrid iterator
  int kernel_x=11, kernel_y=11, kernel_z=11;
  bvpl_subgrid_iterator<float> iter(*grid, vgl_vector_3d<int> (kernel_x,kernel_y,kernel_z));
  while (!iter.done()) {
    bvpl_voxel_subgrid<float> subgrid = *iter;
    ++iter;
  }
  // go through the subgrid
}

TESTMAIN( test_subgrid );