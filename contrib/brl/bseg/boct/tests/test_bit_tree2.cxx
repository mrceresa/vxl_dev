#include <testlib/testlib_test.h>

#include <boct/boct_bit_tree2.h>
#include <vnl/vnl_random.h>
#include <vcl_iostream.h>

static void test_bit_tree2()
{
    unsigned char bits[73] = {  1,
                                //{1-8}  depth 1
                                1,1,1,1,1,1,1,1,
                                //DEPTH 2
                                //{9-16}               {17-24}
                                1,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0, 
                                //{25-32}              {33-40}
                                0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
                                //{41-48}              {49-56}
                                0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,0,
                                //{57-64}              {65-72}
                                0,0,0,0,0,0,0,0,   0,0,0,0,0,0,0,1};
    unsigned char test_tree[16] = {1,255,1,0,0,0,0,0,0,128,0,0,0,0,0,0};
    boct_bit_tree2 tree(test_tree,4); 
    //---------------------------------------------------------------------------
    //test set and bit_at bits
    //---------------------------------------------------------------------------
    bool good = true;
    for(int i=0; i<73; i++) {

        good = good && (tree.bit_at(i) == bits[i]);
        if(!good)
            vcl_cout<<"i";
    }
    TEST("Set bits and get bits works ", true, good);

  //---------------------------------------------------------------------------
  //test traverse
  //---------------------------------------------------------------------------
  vgl_point_3d<double> o(0.0, 0.0, 0.0);
  int leaf_bit_index = tree.traverse(o);
  TEST("Traverse to origin works ", leaf_bit_index, 73);
  
  vgl_point_3d<double> m(0.25, 0.25, 0.0);
  int mid_bit_index = tree.traverse(m);
  TEST("Traverse to mid point works ", mid_bit_index, 12);
  
  vgl_point_3d<double> h(.99, .99, .99);
  int high_bit_index = tree.traverse(h);
  TEST("Traverse to far edge works ", high_bit_index, 584);
  
  //---------------------------------------------------------------------------
  //test traverse opt
  //---------------------------------------------------------------------------
  int leaf_opt = tree.traverse(o);
  int mid_opt = tree.traverse(m);
  int high_opt = tree.traverse(h);
  vcl_cout<<"origin: "<<leaf_opt<<"  mid: "<<mid_opt<<"  high: "<<high_opt<<vcl_endl;
  good = (leaf_opt == leaf_bit_index) && (mid_opt == mid_bit_index);
  TEST("Traverse opt works ", good, true);

  //---------------------------------------------------------------------------
  //test data_index lookup
  //---------------------------------------------------------------------------
  int origin_data = tree.get_data_index(leaf_bit_index);
  vcl_cout<<origin_data<<vcl_endl;
  TEST("Origin data at 73", origin_data, 73);

  int mid_data = tree.get_data_index(mid_bit_index);
  vcl_cout<<mid_data<<vcl_endl;
  TEST("Mid data @ 12", mid_data, 12);

  int high_data = tree.get_data_index(high_bit_index);
  vcl_cout<<high_data<<vcl_endl;
  TEST("Mid data @ 12", mid_data, 12);
  //---------------------------------------------------------------------------  
  //Test size of tree
  //---------------------------------------------------------------------------
  int size = tree.num_cells();
  TEST("Size of tree = 89", size, 89);

}

TESTMAIN(test_bit_tree2);