#pragma OPENCL EXTENSION cl_khr_global_int32_base_atomics : enable

/////////////////////////////////////////////////////////////////////
// STACK IMPLEMENTATION METHODS:
// the necessary stack size will be about 8*num_levels
/////////////////////////////////////////////////////////////////////
typedef struct {
  int data[999];
  int top;
} ocl_stack;
void init_ocl_stack(ocl_stack *stack) { stack->top = 0; }
bool empty(ocl_stack *stack) { return stack->top <= 0; }
void push(ocl_stack *stack, int a) {
  stack->data[stack->top] = a;
  stack->top++;
}
int pop(ocl_stack *stack) {
  int buff = stack->data[stack->top-1];
  stack->top--;
  return buff;
}
 
///////////////////////////////////////////////////////////////////////
// Swaps 8 blocks of data from index A to index B
// makes sure to switch A and B's parents to point to the new A and B locations
// makes sure to switch A and B's children to point to the new A and B locations
// makes sure to update the location of nodes in the STACK
///////////////////////////////////////////////////////////////////////
void swap_eight(__local int4 *tree, int a, int b, __global float* output)
{
  
  unsigned lid = get_local_id(0);
  unsigned gid = get_group_id(0);

  //update A and B's Parent's child pointer first
  if(lid==0) {
    int parent_ptr = tree[a].x;
    tree[parent_ptr].y = b;
    parent_ptr = tree[b].x;
    tree[parent_ptr].y = a; 
  }

  //copy b to a buffer
  int4 buff;
  buff = tree[b+lid];

  //copy A to B
  tree[b+lid] = tree[a+lid];
  
  //copy Buffer to A
  tree[a+lid] = buff;
  
  //for each child of B and A update the parent pointer
  int childA = tree[a+lid].y;
  int childB = tree[b+lid].y;
  if(childA > 0) {
    for(int j=0; j<8; j++)
      tree[childA+j].x = a+lid;
  } 
  if(childB > 0) {
    for(int j=0; j<8; j++)
      tree[childB+j].x = b+lid;    
  }
}
 
////////////////////////////////////////////
//reformat tree method
//Tree cell organization: (parent ptr, child ptr, data ptr, nada)
///////////////////////////////////////////
void reformat_tree(__local int4 *tree, __global float* output)
{
  unsigned gid = get_group_id(0);
  
  unsigned curr_index = 0;
  ocl_stack open;
  init_ocl_stack(&open);
  push(&open, 0);
 
  while(!empty(&open)){
    //examine node at top of stack
    int currNode = pop(&open);
    //if the current node has no children, nothing to verify
    int child_ptr = tree[currNode].y;
    bool isleaf = (child_ptr < 0);
    if(!isleaf){
      //if child pointer isn't to the right place..
      if(child_ptr != curr_index+1){          
        //-- need to swap 8 nodes at currNode[1] to curr_index+1 --//
        barrier(CLK_GLOBAL_MEM_FENCE);
        swap_eight(tree, child_ptr, curr_index+1, output);
        child_ptr = curr_index+1;  //verify that tree[currNode].y is equal to curr_index+1;
      } 
      //push children on stack (in reverse order)
      for(int i=7; i>=0; i--){
        push(&open, child_ptr+i);
      }
      curr_index += 8;
    }
  }
}



/////////////////////////////////////////////////////////////////
////SPLIT TREE
////Depth first search iteration of the tree (keeping track of node level)
////1) parent pointer, 2) child pointer 3) data pointer 4) nothing right now
// Kind of a wierd mix of functions - the tree structure is modified locally, 
// so no tree_buffer information is needed, whereas the data is modified 
// on the global level, so buffers, offsets are used
/////////////////////////////////////////////////////////////////
int refine_tree(__local int4    *tree, 
                        int      tree_size, 
               __global float16 *data_cells,
                        int      data_size,
                        float    prob_thresh, 
                        float    boxLen, 
                        int      maxLevel, 
                        int      len_buffer,
               __global float   *output, 
                        int      block_index)
{
  unsigned gid = get_group_id(0);
  unsigned lid = get_local_id(0);

  //max alpha integrated
  float max_alpha_int = (-1)*log(1.0 - prob_thresh);

  //need to keep track of current level and pops for each level
  int popCounts[5];
  for(int i=0; i<5; i++) popCounts[i]=0;
  int currLevel = 0;

  //stack for depth first traversal
  ocl_stack open;
  init_ocl_stack(&open);
  push(&open, 0);
  while(!empty(&open)){
    //figure out the current level
    while(popCounts[currLevel] >= 8) {
      popCounts[currLevel] = 0;
      currLevel--;
    }
   
    //examine node at top of stack (and keep track of popping)
    int currNode = pop(&open);
    popCounts[currLevel]++;
    
    //if the current node has no children, it's a leaf -> check if it needs to be refined
    int child_ptr = tree[currNode].y;
    if(child_ptr < 0){
                
      //////////////////////////////////////////////////
      //INSERT LEAF SPECIFIC CODE HERE
      //////////////////////////////////////////////////
      //find side length for cell of this level (bit shift: two_pow_level = 1<<currLevel;)
      //int two_pow_level = pow(2,currLevel);
      unsigned two_pow_level = 1<<currLevel;
      float side_len = boxLen/two_pow_level;
     
      //get alpha value for this cell;
      int dataIndex = tree[currNode].z;
      float16 datum = data_cells[gid*len_buffer + dataIndex];
      float alpha = datum.s0;
     
      if(lid == 0)
        output[gid] = alpha;

     
      //integrate alpha value
      float alpha_int = alpha * side_len;
      
      //IF alpha value triggers split, tack on 8 children to end of tree array
      //make sure the PARENT cell for each of the new children points to i
      //ALSO make sure currLevel is less than MAX_LEVELS
      if(alpha_int > max_alpha_int && currLevel < maxLevel-1)  {
        
        //new alpha for the child nodes
        float new_alpha = max_alpha_int / side_len;  
        
        //node I points to tSize - the place where it's children will be tacked on
        tree[currNode].y = tree_size;
        
        //each child points to the currNode, has no children, 
        //barrier(CLK_GLOBAL_MEM_FENCE);
/*
        if(lid < 8) {
          int4 tcell = (int4) (currNode, -1, (int)data_size+lid, 0);
          tree[tree_size+lid] = tcell;
        
          //copy data for new children
          //float16 newData = datum;
          float16 newData = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
          newData.s0 = new_alpha;
          int newDataIndex = gid*len_buffer + (data_size+lid);
          data_cells[newDataIndex] = newData;
        }
*/
        for(int i=0; i<8; i++) {
          int4 tcell = (int4) (currNode, -1, (int)data_size+i, 0);
          tree[tree_size+i] = tcell;
          
          float16 newData = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
          newData.s0 = new_alpha;
          int newDataIndex = gid*len_buffer + (data_size+i);
          data_cells[newDataIndex] = newData;
        }

        //update tree and buffer size
        tree_size += 8; 
        data_size += 8;
     
        //reset data for curent node
        float16 zeroDat = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
        data_cells[dataIndex] = zeroDat;    
      }
      

      ////////////////////////////////////////////
      //END LEAF SPECIFIC CODE
      ////////////////////////////////////////////
     
    }
    //for inner nodes
    else {                      
      for(int i=7; i>=0; i--){
        push(&open, child_ptr+i);
      }
      currLevel++;
    }
  }      

  ///////////////////////////////////////////////////////////////////
  ////REFORMAT TREE into cannonical order
  ///////////////////////////////////////////////////////////////////
  //reformat_tree(tree, output); 

  //tree and data size output
  return tree_size;
}

 
///////////////////////////////////////////
//REFINE MAIN
//TODO include CELL LEVEL SOMEHOW to make sure cells don't over split
//TODO include a debug print string at the end to know what the hell is going on.
///////////////////////////////////////////
__kernel
void
refine_main(__global  int4     *block_ptrs,     //3d block array
            __global  int4     *block_nums,     //number of blocks in each dimension
            __private unsigned  num_buffer,     //number of tree buffers
            __private unsigned  len_buffer,     //length of tree buffer (number of int4 cells allocated)
            __global  int4     *tree_cells,     //tree_cells
            __global  float16  *data_cells,     //data_cells
            __global  int2     *mem_ptrs,       //denotes occupied space in each tree buffer
            __private float     prob_thresh,    //refinement threshold
            __private unsigned  max_level,      //maximum number of levels for tree (4 for small tree)
            __private float     bbox_len,       //side length of one block
            __local   int4      *local_tree,    //local copy of the tree
            __global  float     *output)        //TODO delete me later
{

  //global id will be the tree buffer
  unsigned gid = get_group_id(0);
  unsigned lid = get_local_id(0);
     
  //go through the tree array and refine it...
  if(gid < num_buffer) 
  {

    //keep track of the start and end pointer as they change
    //NOTE THAT END POINTER is equal to 2 after the last element
    int startPtr = mem_ptrs[gid].x;
    int endPtr = mem_ptrs[gid].y;
    
    //get the (absolute) index of the start and end pointers
    int preRefineStart = mem_ptrs[gid].x;
    int preRefineEnd = mem_ptrs[gid].y;
    
    //initialize curr root index, next root index 
    int currRootIndex = preRefineStart;
        
    //when currRootIndex is equal to preRefineEnd-1, then you're done
    while(currRootIndex != preRefineEnd-1) {
      
      //1. get current tree information
      int4 currRoot = tree_cells[gid*len_buffer + currRootIndex];
      int currBlkIndex = currRoot.w;
      int currTreeSize = block_ptrs[currBlkIndex].z;
                  
      //2. copy current tree to local mem
      //TODO Make sure your tree doesn't get corrupted because you don't clear out all 585 cells
      //maybe pass in a length to the refine function to make sure yo udon't go out of bounds.  
      for(int j=0; j<currTreeSize; j++) {
        local_tree[j] = tree_cells[gid*len_buffer + (currRootIndex+j)];
      }   

      //3. determine number of data cells used, datasize = occupied space
      int dataSize = (endPtr > startPtr)? (endPtr-1)-startPtr: len_buffer - (startPtr-endPtr)-1;

      //4. refine tree locally
      int newSize = refine_tree(local_tree, 
                                currTreeSize, 
                                data_cells, 
                                dataSize, 
                                prob_thresh, 
                                bbox_len, 
                                max_level,
                                len_buffer, output, currRootIndex);
      
      //5. update start pointer (as data will be moved up to the end)
      startPtr = (startPtr+currTreeSize)%len_buffer;

      //6. if there's enough space, move tree
      int freeSpace = (startPtr >= endPtr)? startPtr-endPtr : len_buffer - (endPtr-startPtr);
      if(newSize <= freeSpace) {    
        for(int j=0; j<newSize; j++) {
          int cellIndex = gid*len_buffer + (endPtr-1+j+len_buffer)%len_buffer;
          tree_cells[cellIndex] = local_tree[j];
        }
                  
        //6c. update endPtr
        endPtr = (endPtr+newSize)%len_buffer;
        
        //6d. update block_ptrs 
        block_ptrs[currBlkIndex].y = (endPtr-1+len_buffer)%len_buffer;
        block_ptrs[currBlkIndex].z = newSize;
      } 
      //7. not enough space, throw whole block into emergency space
      else {
        //(*output) = 666;
        //block_ptrs[currBlkIndex].y = -666;  //you need to really do something about this one.
        //block_ptrs[currBlkIndex].z = newSize;
      }

      //update current root index
      currRootIndex = (currRootIndex+currTreeSize)%len_buffer;
      
    }
    
    mem_ptrs[gid].x = startPtr;
    mem_ptrs[gid].y = endPtr; 
  }
  


}

 
 
 
