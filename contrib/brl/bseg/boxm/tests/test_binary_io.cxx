
#include <testlib/testlib_test.h>

#include <boxm/boxm_scene.h>
#include <boct/boct_tree.h>
#include <vpl/vpl.h>
#include <vul/vul_file.h>

MAIN( test_binary_io )
{
  START ("CREATE SCENE");
  short nlevels=5;
  boct_tree<short,vgl_point_3d<double>,void > * block = new boct_tree<short,vgl_point_3d<double>,void >(nlevels);
  TEST("No of Max levels of tree",nlevels, block->num_levels());
  
  block->split();
  //block->print();

  bgeo_lvcs lvcs(33.33,44.44,10.0, bgeo_lvcs::wgs84, bgeo_lvcs::DEG, bgeo_lvcs::METERS);
  vgl_point_3d<double> origin(10,10,20);
  vgl_vector_3d<double> block_dim(10,10,10);
  vgl_vector_3d<double> world_dim(30,30,30);
  boxm_scene<boct_tree<short,vgl_point_3d<double>,void > > scene(lvcs, origin, block_dim, world_dim);
  scene.set_paths("./boxm_scene", "block");
  x_write(vcl_cout, scene, "scene");

  vsl_b_ofstream os("scene.bin");
  scene.b_write(os);
  os.close();

  boxm_scene<boct_tree<short,vgl_point_3d<double>,void > > scene_out;
  vsl_b_ifstream is("scene.bin", vcl_ios_binary);
  scene_out.b_read(is);

  vpl_unlink("scene.bin");

  //: writing the binary file for a scene with grey MOG
  vgl_point_3d<double>  mog_origin(0,0,0);
  vgl_vector_3d<double> mog_block_dim(1,1,1);
  vgl_vector_3d<double> mog_world_dim(3,3,1);

  boxm_scene<boct_tree<short,boxm_sample<BOXM_APM_MOG_GREY>,void > > mog_scene( mog_origin, mog_block_dim, mog_world_dim);
  mog_scene.set_paths("./boxm_scene", "block");
  vul_file::make_directory("./boxm_scene");
  boxm_block_iterator<boct_tree<short,boxm_sample<BOXM_APM_MOG_GREY>,void > > iter(&mog_scene);
  
  while(!iter.end())
  {
    mog_scene.load_block(iter.index().x(),iter.index().y(),iter.index().z());
    boxm_block<boct_tree<short,boxm_sample<BOXM_APM_MOG_GREY>,void > > * block=mog_scene.get_active_block();
    boct_tree<short,boxm_sample<BOXM_APM_MOG_GREY>,void > * tree=new boct_tree<short,boxm_sample<BOXM_APM_MOG_GREY>,void >(10,6);
    block->init_tree(tree);
    mog_scene.write_active_block();
    iter++;
  }

  vsl_b_ofstream mog_os("./boxm_scene/scene.bin");
  mog_scene.b_write(mog_os);
  mog_os.close();
  //: writing the binary file for a scene with grey MOG
  boxm_scene<boct_tree<short,boxm_sample<BOXM_APM_MOG_GREY>,void > > read_mog_scene;
  read_mog_scene.load_scene("./boxm_scene/scene.bin");

  boxm_block_iterator<boct_tree<short,boxm_sample<BOXM_APM_MOG_GREY>,void > > read_iter(&read_mog_scene);
  while(!read_iter.end())
  {
    read_mog_scene.load_block(read_iter.index().x(),read_iter.index().y(),read_iter.index().z());
    boxm_block<boct_tree<short,boxm_sample<BOXM_APM_MOG_GREY>,void > > * block=read_mog_scene.get_active_block();
    boct_tree<short,boxm_sample<BOXM_APM_MOG_GREY>,void > * tree=block->get_tree();
    read_iter++;
  }

  SUMMARY();  
}
