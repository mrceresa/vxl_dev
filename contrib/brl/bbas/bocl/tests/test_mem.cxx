#include <testlib/testlib_test.h>
#include <testlib/testlib_root_dir.h>
#include <bocl/bocl_mem.h>
#include <bocl/bocl_manager.h>

bool test_fill_kernel()
{
  //initialize a buffer
  int length = 128*128; 
  int* a = new int[length]; 
  int* b = new int[length];
  for(int i=0; i<length; ++i) {
   a[i] = (int) i;
   b[i] = (int) i;
  }
  
  //make bocl manager (handles a lot of OpenCL stuff)
  bocl_manager_child_sptr mgr = bocl_manager_child::instance();
  if(mgr->gpus_.size() < 1) {
    vcl_cout<<"NO GPUS!!!!"<<vcl_endl;  
    return false;
  }
  
  //create command queue
  cl_command_queue queue = clCreateCommandQueue(mgr->context(), mgr->devices()[0], CL_QUEUE_PROFILING_ENABLE, NULL);
  
  //cr  //make sure a is set to zero
  bocl_mem a_mem(mgr->context(), a, length * sizeof(int), "test int buffer");
  a_mem.create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR); 
  a_mem.zero_gpu_buffer(queue);
  a_mem.read_to_buffer(queue); 
  for(int i=0; i<length; ++i) {
    if(a[i] != 0) {
      TEST("bocl_mem zero gpu buffer returned non zero value", true, false);
      return false;
    }
  }
  TEST("bocl_mem zero gpu buffer returned all zero values", true, true);

  //test float val
  float* c = new float[length]; 
  for(int i=0; i<length; ++i)
    c[i] = (float) i / 2.0f;
  bocl_mem c_mem(mgr->context(), c, length * sizeof(float), "test float buffer");
  c_mem.create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR); 
  c_mem.fill(queue, 1.337f, "float");
  c_mem.read_to_buffer(queue);
  for(int i=0; i<length; ++i) {
    if(c[i] != 1.337f) {
      TEST("bocl_mem fill gpu buffer float returned bad value", true, false);
      return false;
    }
  }
  TEST("bocl_mem float fill gpu buffer returned all correct values", true, true);
  
  //make sure b is set 
  bocl_mem b_mem(mgr->context(), b, length * sizeof(int), "test int buffer");
  b_mem.create_buffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR); 
  b_mem.fill(queue, (uint) 1337, "uint");
  b_mem.read_to_buffer(queue);
  for(int i=0; i<length; ++i) {
    if(b[i] != 1337) {
      TEST("bocl_mem fill gpu buffer returned bad value", true, false);
      return false;
    }
  }
  TEST("bocl_mem fill gpu buffer returned all correct values", true, true);

  //clean up buffer
  delete[] a; 
  delete[] b;
  delete[] c;
  return true;
}

static void test_mem()
{
  test_fill_kernel(); 
}

TESTMAIN(test_mem);
