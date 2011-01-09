//This is brl/bseg/boct/boct_bit_tree2.h
#ifndef boct_bit_tree2_h_
#define boct_bit_tree2_h_
//:
// \file
// \brief  Test octree implementation using an implicit bit-array to denote tree structure.
//         Data structure is currently float 16.
//
// \author Andrew Miller
// \date   August 11, 2010
// \verbatim
//  Modifications
//   <none yet>
// \endverbatim

#include <vnl/vnl_vector_fixed.h>
#include <vgl/vgl_point_3d.h>
#include <vgl/vgl_box_3d.h>
#include <vcl_bitset.h>
#include <vcl_iosfwd.h>
#include <vcl_vector.h>

unsigned char bit_lookup[] =
                         { 0,   1,   1,   2,   1,   2,   2,   3,   1,   2,   2,   3,   2,   3,   3,   4,
                           1,   2,   2,   3,   2,   3,   3,   4,   2,   3,   3,   4,   3,   4,   4,   5,
                           1,   2,   2,   3,   2,   3,   3,   4,   2,   3,   3,   4,   3,   4,   4,   5,
                           2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6,
                           1,   2,   2,   3,   2,   3,   3,   4,   2,   3,   3,   4,   3,   4,   4,   5,
                           2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6,
                           2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6,
                           3,   4,   4,   5,   4,   5,   5,   6,   4,   5,   5,   6,   5,   6,   6,   7,
                           1,   2,   2,   3,   2,   3,   3,   4,   2,   3,   3,   4,   3,   4,   4,   5,
                           2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6,
                           2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6,
                           3,   4,   4,   5,   4,   5,   5,   6,   4,   5,   5,   6,   5,   6,   6,   7,
                           2,   3,   3,   4,   3,   4,   4,   5,   3,   4,   4,   5,   4,   5,   5,   6,
                           3,   4,   4,   5,   4,   5,   5,   6,   4,   5,   5,   6,   5,   6,   6,   7,
                           3,   4,   4,   5,   4,   5,   5,   6,   4,   5,   5,   6,   5,   6,   6,   7,
                           4,   5,   5,   6,   5,   6,   6,   7,   5,   6,   6,   7,   6,   7,   7,   8
                         };

class boct_bit_tree2
{
 public:

  //: Default constructor
  boct_bit_tree2();
  boct_bit_tree2(unsigned char* bits, int num_levels=4);

  //: Destructor
  ~boct_bit_tree2() {
    if (bits_) {
      delete[] bits_; bits_ = NULL;
    }
  }

  //: Returns index of data for given bit
  int get_data_index(int bit_index);

  //: traverse tree to get leaf index that contains point
  int traverse(const vgl_point_3d<double> p);

  //: Return the maximum number of levels, which is root_level+1
  short number_levels() const { return num_levels_; }

  //: Return number of cells in this tree
  int num_cells() const;

  //: returns depth (0,1,2,3) at given index
  //  Note that cumulative nodes = (1/7) * (8^(n+1) -1)
  int depth_at(int index) const;

  //: returns value (0 or 1) of bit at given index (0,73);
  unsigned char bit_at(int index);
  //: sets bit at given index with bool value
  void set_bit_at(int index, bool val);

  //get bits and data
  unsigned char* get_bits() {return bits_; }

 private:

  //: Tree structure stored as "bits" = really a char array
  unsigned char* bits_;
  //: Maximum number of levels in the octree
  short num_levels_;
};

vcl_ostream& operator <<(vcl_ostream &s, boct_bit_tree2 &t);

#endif // boct_bit_tree2_h_
