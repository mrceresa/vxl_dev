// This is brl/bbas/bhdfs/pro/processes/bhdfs_boxm2_load_scene_process.cxx
//:
// \file
// \brief  A process for loading the scene from HDFS file system. 
//
// \author Gamze Tunali
// \date   Sep 16, 2012

#include <bprb/bprb_func_process.h>

#include <vcl_fstream.h>
#include <boxm2/boxm2_scene.h>

namespace bhdfs_boxm2_load_scene_process_globals
{
  const unsigned n_inputs_ = 1;
  const unsigned n_outputs_ = 1;
}
bool bhdfs_boxm2_load_scene_process_cons(bprb_func_process& pro)
{
  using namespace bhdfs_boxm2_load_scene_process_globals;

  //process takes 1 input
  vcl_vector<vcl_string> input_types_(n_inputs_);
  input_types_[0] = "vcl_string";

  // process has 1 output:
  // output[0]: scene sptr
  vcl_vector<vcl_string>  output_types_(n_outputs_);
  output_types_[0] = "boxm2_scene_sptr";

  return pro.set_input_types(input_types_) && pro.set_output_types(output_types_);
}

bool bhdfs_boxm2_load_scene_process(bprb_func_process& pro)
{
  using namespace bhdfs_boxm2_load_scene_process_globals;

  if ( pro.n_inputs() < n_inputs_ ){
    vcl_cout << pro.name() << ": The input number should be " << n_inputs_<< vcl_endl;
    return false;
  }
  //get the inputs
  unsigned i = 0;
  vcl_string filename = pro.get_input<vcl_string>(i++);

  // load the XML from HDFS
  if (!bhdfs_manager::exists())
    bhdfs_manager::create(vcl_string("default"), 0);

  bhdfs_manager_sptr mgr = bhdfs_manager::instance();
  bhdfs_fstream_sptr fs = new bhdfs_fstream(filename,"r");
  unsigned long fsize = fs->file_size()-1;
  char* buf = new char[fsize];
  if (fs->read(buf, fsize) == -1)
    vcl_cerr << "HDFS -- error reading the file!" << vcl_endl;
  vcl_string str(buf);

  // clean up the garbage chars after the last bracket '>'
  int pos = str.find_last_of('>');
  str = str.erase(pos+1);
  delete buf;
  const char* newbuf = str.c_str();
  vcl_cout << fsize << "[" << newbuf << "]" << vcl_endl;
  if (!parser.parseString(newbuf)) {
    vcl_cerr << "HDFS--" << XML_ErrorString(parser.XML_GetErrorCode()) << " at line "
             << parser.XML_GetCurrentLineNumber() << '\n';
     return;
  }

  boxm2_scene_sptr scene= new boxm2_scene(newbuf);
   
  i=0;
  // store scene smaprt pointer
  pro.set_output_val<boxm2_scene_sptr>(i++, scene);
  return true;
}
