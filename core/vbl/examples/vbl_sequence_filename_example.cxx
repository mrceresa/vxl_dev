#include <vcl_fstream.h>
#include <vbl/vbl_sequence_filename_map.h>

int main (int argc, char** argv)
{
  if (argc > 1) {
    // Initialize a vbl_sequence_filename_map with the pattern
    vbl_sequence_filename_map map(argv[1]);
    
    // Print out all the files that are specified by that pattern.
    for(int i = 0; i < map.get_nviews(); ++i) {
      vcl_cout << map.name(i) << vcl_endl;
    }
    
  } else {
    // Capes' examples
    {
      vbl_sequence_filename_map map("images/img.%03d.pgm,0:2:100");
    }
    {
      vbl_sequence_filename_map map("plop/plip/img_####.tiff",5,10,200);
    }
    {
      vbl_sequence_filename_map map("pop.pop/poo_poo/img.###.ppm;1:2:10");
    }
    {
      vbl_sequence_filename_map map("im.###,:5:");
      vcl_cerr << map.name(10) << vcl_endl;
      vcl_cerr << map.pair_name(10,11) << vcl_endl;
      vcl_cerr << map.triplet_name(10,11,12) << vcl_endl;
      vcl_cerr << map.image_name(10) << vcl_endl;
    }
  }
}

