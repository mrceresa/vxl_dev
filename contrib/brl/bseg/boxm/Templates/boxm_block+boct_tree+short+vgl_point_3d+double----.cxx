#include <boxm/boxm_block.txx>
#include <boct/boct_tree.txx>
#include <vgl/io/vgl_io_point_3d.h>

typedef boct_tree<short, vgl_point_3d<double> >  tree_type;
BOXM_BLOCK_INSTANTIATE(tree_type);