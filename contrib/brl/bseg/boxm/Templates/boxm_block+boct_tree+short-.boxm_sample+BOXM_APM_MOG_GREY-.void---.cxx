#include <boxm/boxm_block.txx>
#include <boxm/boxm_apm_traits.h>
#include <boxm/boxm_sample.h>
#include <boct/boct_tree.txx>
#include <vgl/io/vgl_io_point_3d.h>

typedef boct_tree<short, boxm_sample<BOXM_APM_MOG_GREY>,void >  tree_type;
BOXM_BLOCK_INSTANTIATE(tree_type);