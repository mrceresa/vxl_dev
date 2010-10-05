/* ray_bundle_library_opt.cl
 * Optimized ray bundle library functions - 
 * 
 * Handles workgroup segmentation (needed to update only one block at a time 
 * during raytrace)
 * 
 * Implements seg_len_obs pass, pre_infinity pass, and bayes_update pass
 * 
 * NOTE: update seg_len, pre_inf and bayes_update to take on more args and 
 * become the step cell functor
 * 
 * NOTE: There may be a bug in Pre Inf and Bayes Update because of the thresholding
 * if statement precludes first calculation, but not second
 */


/* Load Data Mutable Opt
 * This optimized version of the function above serves to replace it with 
 * simpler data structures, and a more parallel algorithm. The current 
 * idea is find the connected components in parallel, where segmentation data
 * is stored in a short2 array (one for each ray in the work group, as follows:
 *    x        y
 *  [next_ptr   | is_leader]
 * 
 *    o  next_ptr points to the LLID of the following ray cast into the same cell 
 *       (or it is -1 if there is no such ray).  
 *    o  is_leader denotes whether other not this particular ray is the owner 
 *       of the segment - if it is, it will be responsible for writing to the 
 *       data cell that it's segment points to.  
 * 
 * It will be the job of the step cell functor (seg len, bayes update, etc) to 
 * use the ray_bundle_array to iterate through each segment, calculating the
 * statistic, and writing it out to global mem.  
 * 
 * EDIT: What if ray doesn't hit anything - that is cell_ptrs is less than zero
 * (presumably the ray would have already been killed - but what happens to 
 * that slot?  Is this what the ACTIVE tag did?) This shouldn't matter if the
 * inactive rays (broken rays) are just never assigned as a leader, nor are 
 * pointed to by any other segment. 
 * 
 */
int load_data_mutable_opt(__local short2  * ray_bundle_array, 
                          __local int     * cell_ptrs)
{
  uchar nbi = (uchar)get_local_size(0);   //num cols (size of X dimension)
  uchar nbj = (uchar)get_local_size(1);   //num rows (size of Y dimension)
  uchar lsize = nbi*nbj;                  //local workgroup size
  uchar col = (uchar)get_local_id(0);     //thread col
  uchar row = (uchar)get_local_id(1);     //thread row
  uchar llid = (col + nbi*row);           //[0-nbi*nbj] index
  
  //initialize segmentation information 
  ray_bundle_array[llid] = (short2) (-1, 1);
  
  //first pass looks to the right neighbor (make sure you don't look for the last one)
  if(cell_ptrs[llid] == cell_ptrs[llid+1] && llid<lsize-1) {
    ray_bundle_array[llid].x   = llid+1;      //NEXT pointer = llid+1
    ray_bundle_array[llid+1].y = 0;           //NEXT node is not a leader
  }
  barrier(CLK_LOCAL_MEM_FENCE);

  //second pass - start at the next row (don't look in the last row)
  if(ray_bundle_array[llid].x == -1 && row<nbi-1) {
    
    //next row index
    int start = (row+1)*nbi;
    int end   = start + col+1;
    for(int i=start; i<end; ++i) {
      if(cell_ptrs[llid] == cell_ptrs[i]) {
        ray_bundle_array[llid].x = i;         //NEXT pointer = i;
        ray_bundle_array[i].y    = 0;         //i is not a leader anymore
      }
    }
  }
  return 1;
}


/*
*  Accumulate ray segment length and the weighted observation
*  sums as each cell is accessed. It can be the case that multiple rays pass
*  through the same cell. In order to avoid conflicts, only one thread
*  is allowed to update the sums for such cells. The active thread is
*  identified with the index of the base pointer of the multiple ray
*  region in the bundle array. The argument image_vect is a linear array of
*  float4 vectors corresponding to the index of the local ray bundle, in
*  raster order. The layout of the vector is:
*  [obs | alpha_integral | vis | pre ]
*
*/
void seg_len_obs_opt(        float    seg_len, 
                     __local float4 * image_vect,
                     __local short2 * ray_bundle_array,
                     __local float4 * cached_aux_data)
{
    /* linear thread id */
    uchar llid = (uchar)(get_local_id(0) + get_local_size(0)*get_local_id(1));
    
    /* limit access to threads that do not own a connected region */
    if (ray_bundle_array[llid].y == 0)
    {
        /* store seg_len in the corresponding aux data slot to be accessed
        * by other threads since these aux_data items are not used, they
        * can be cleared and used to store the seg_len of non-owner rays
        */
        cached_aux_data[llid] = (float4) (seg_len, 0.0f, 0.0f, 0.0f);
    }
    barrier(CLK_LOCAL_MEM_FENCE);
    
    /* now, limit access to the threads that own each connected region */
    if (ray_bundle_array[llid].y == 1)
    {
        /* The region owner (base) is now the only active thread within the region*/
        /* process the base ray */
        cached_aux_data[llid].x += seg_len;                       // seg_len sum
        cached_aux_data[llid].y += (image_vect[llid].x)*seg_len;  // weighted observations */

        /* traverse the linked list and increment sums */
        short next = ray_bundle_array[llid].x;  // linked list pointer 
        while(next >= 0) {
            cached_aux_data[llid].x += cached_aux_data[next].x;
            cached_aux_data[llid].y += (image_vect[next].x)*cached_aux_data[next].x;
            next = ray_bundle_array[next].x;
        }
    }
    barrier(CLK_LOCAL_MEM_FENCE);
}



/*
*  Accumulate the pre and vis image arrays based on the cell data.
*  A vector of images is maintained image_vect that is updated as the rays
*  step through the volume. The image vector is assigned as:
*
*  [obs | alpha_integral | vis | pre ]
*
*/
void pre_infinity_opt(        float    seg_len, 
                      __local float4*  image_vect,
                      __local short2*  ray_bundle_array,
                      __local float16* cached_data,
                      __local float4*  cached_aux_data)
{
    /* linear thread id */
    uchar llid = (uchar)(get_local_id(0) + get_local_size(0)*get_local_id(1));
    cached_data[llid].sd = seg_len;
    barrier(CLK_LOCAL_MEM_FENCE); /*wait for all threads to complete */
    float temp1 = 0.0f, temp2 = 0.0f; /* minimize registers */

    /* if total length of rays is too small, do nothing */
    temp1 = cached_aux_data[llid].x; /* length sum */
    if (temp1>1.0e-10f)
    {
        /* The mean intensity for the cell */
        temp2 = cached_aux_data[llid].y/temp1; /* mean observation */
        temp1 = gauss_3_mixture_prob_density(temp2,
                                             cached_data[llid].s1,
                                             cached_data[llid].s2,
                                             cached_data[llid].s3,
                                             cached_data[llid].s5,
                                             cached_data[llid].s6,
                                             cached_data[llid].s7,
                                             cached_data[llid].s9,
                                             cached_data[llid].sa,
                                             (1.0f-cached_data[llid].s3
                                             -cached_data[llid].s7)
                                            );/* PI */
            
        /* temporary slot to store PI*/
        cached_data[llid].se = temp1;
    }
    barrier(CLK_LOCAL_MEM_FENCE); /*wait for all threads to complete */

    /* Calculate pre and vis infinity */
    /*alpha integral          alpha           *        seg_len      */
    image_vect[llid].y += cached_data[llid].s0*cached_data[llid].sd;

    temp2 = exp(-image_vect[llid].y); /* vis_prob_end */

    /* updated pre                      Omega         *       PI         */
    image_vect[llid].w += (image_vect[llid].z - temp2)*cached_data[llid].se;
    /* updated visibility probability */
    image_vect[llid].z = temp2;
    barrier(CLK_LOCAL_MEM_FENCE);
}


/*
 *  Compute the Bayes update ratio for alpha
 *  A vector of images is maintained image_vect that is updated as the rays
 *  step through the volume. The image vector is assigned as:
 *
 *  [norm | alpha_integral | vis | pre ]
 *
 */
void bayes_ratio_opt(float seg_len, __local float4* image_vect,
                     __local short2*   ray_bundle_array,
                     __local float16*  cached_data,
                     __local float4*  cached_aux_data)
{
    /* linear thread id */
    uchar llid = (uchar)(get_local_id(0) + get_local_size(0)*get_local_id(1));
    /* insert seg_len into the local cache. This step
    is carried out by all threads */
    cached_data[llid].sd = seg_len;
    barrier(CLK_LOCAL_MEM_FENCE);

    /* ray must be active after data is loaded */
    float temp1 = 0.0f, temp2 = 0.0f; /* minimize registers */
    
    /* Compute PI for all threads */
    if (cached_aux_data[llid].x > 1.0e-4f) {    /* if  too small, do nothing */
        temp1 = cached_aux_data[llid].x; /* length sum */
        /* The mean intensity for the cell */
        temp2 = cached_aux_data[llid].y/temp1; /* mean observation */
        temp1 = gauss_3_mixture_prob_density(temp2,
                                            cached_data[llid].s1,
                                            cached_data[llid].s2,
                                            cached_data[llid].s3,
                                            cached_data[llid].s5,
                                            cached_data[llid].s6,
                                            cached_data[llid].s7,
                                            cached_data[llid].s9,
                                            cached_data[llid].sa,
                                            (1.0f-cached_data[llid].s3
                                            -cached_data[llid].s7)
                                            );/* PI */
        /* temporary slot to store PI*/
        cached_data[llid].se = temp1;
    }
    barrier(CLK_LOCAL_MEM_FENCE); /*wait for all threads to complete */
    
    /* Below, all active threads participate in updating the alpha integral,
    * pre and vis images */
    /*alpha integral          alpha           *        seg_len      */
    image_vect[llid].y += cached_data[llid].s0*cached_data[llid].sd;

    temp2 = exp(-image_vect[llid].y); /* vis_prob_end */

    /* updated pre                      Omega         *       PI         */
    image_vect[llid].w += (image_vect[llid].z - temp2)*cached_data[llid].se;

    /* updated visibility probability */
    image_vect[llid].z = temp2;

    barrier(CLK_LOCAL_MEM_FENCE); /*wait for all threads to complete */

    /* region owner scans the region and computes Bayes ratio and weighted vis.
    * The aux_data cell elements are assigned as:
    *
    *   [seg_len sum | weighted obs | update_ratio (beta) | weighted vis]
    */
    if (ray_bundle_array[llid].y==1) {
        /* compute data for base ray cell */
        /*   cell.vis         +=        vis(i,j)    *      seg_len */
        cached_aux_data[llid].w += image_vect[llid].z*cached_data[llid].sd;
        /* Bayes ratio */
        /* ( pre + PI*vis)/norm)*seg_len */
        if(image_vect[llid].x>1e-10f)
        {
            cached_aux_data[llid].z +=
                /*      pre(i,j)        +        PI        *       vis(i,j) */
                ((image_vect[llid].w + cached_data[llid].se*image_vect[llid].z)/image_vect[llid].x)*cached_data[llid].sd;
        }
        /*     norm(i,j)        seg_len */

        /* traverse the linked list and increment sums */
        short next = ray_bundle_array[llid].x;  // linked list pointer 
        while(next >= 0) {
            
            /*    cell.vis           +=     vis(i,j)      *      seg_len     */
            cached_aux_data[llid].w += image_vect[next].z*cached_data[next].sd;
            /* Bayes ratio */
            /* ( pre + PI*vis)/norm)*seg_len */
            if( image_vect[next].x>1e-10f)
            {
                cached_aux_data[llid].z +=
                    ((image_vect[next].w + cached_data[llid].se*image_vect[next].z)/
                    image_vect[next].x)*cached_data[next].sd;
            }
            next = ray_bundle_array[next].x;
        }
    }
    barrier(CLK_LOCAL_MEM_FENCE); /*wait for all threads to complete */
}


///*
// *
// * Map the 2-d group id so that the mapped ids step by 2x in both column
// * and row. The odd column indices are addressed after 1/2 of the
// * groups have been processed. The two skipped odd row indices
// * are processed after two even rows are processed.
// *
// */
//
void map_work_space_2d(int* mapped_id0,
                       int* mapped_id1)
{
    unsigned lid0 = get_local_id(0);
    unsigned lid1 = get_local_id(1);
    int group_id0 = get_group_id(0), group_id1 = get_group_id(1);

    /* map the group id to permuted 2d coordinates */
    int ls0 = get_local_size(0), ls1 = get_local_size(1);
    int ngi = (get_global_size(0))/ls0, ngj = (get_global_size(1))/ls1;
    /* 1-dimensional group index */
    int g_1d = group_id0+ngi*group_id1;

    /* offset is 1 if the 1-d group index is past the halfway point */
    int offset0 = 0;

    int offset1 = 0;

    if (g_1d>=(ngi*ngj)/4)
    {
        offset0=1;
        if (g_1d>=(ngi*ngj)/2)
        {
            offset0=0;
            offset1=1;
            if (g_1d>=3*(ngi*ngj)/4)
            {
                offset0=1;
            }
        }
    }

    /* step by 2 in group column index - add offset for odd indices after 1/2
    the groups have been processed*/
    int mi = (group_id0 <ngi/2)? 2*(group_id0):2*(group_id0-ngi/2);
    int mj = 4*(group_id1%(ngj/4))+2*(group_id0/(ngi/2));

    mi=mi+offset0;
    mj=mj+offset1;
    /* map the individual bundle coordinates */
    (*mapped_id0) = mi*ls0 + lid0;
    (*mapped_id1) = mj*ls1 + lid1;
}

void map_work_space_2d_offset(int* mapped_id0,
                              int* mapped_id1,
                              int offset0,
                              int offset1)
{
    unsigned lid0 = get_local_id(0);
    unsigned lid1 = get_local_id(1);
    int group_id0 = get_group_id(0), group_id1 = get_group_id(1);

    /* map the group id to permuted 2d coordinates */
    int ls0 = get_local_size(0), ls1 = get_local_size(1);

    /* map the individual bundle coordinates */
    (*mapped_id0) = (2*group_id0+offset0)*ls0 + lid0;
    (*mapped_id1) = (2*group_id1+offset1)*ls1 + lid1;
}



void update_cell(float16 * data, float4 aux_data,float t_match, float init_sigma, float min_sigma)
{
    if (aux_data.x>1e-5)
    {
        float mu0 = (*data).s1, sigma0 = (*data).s2, w0 = (*data).s3;
        float mu1 = (*data).s5, sigma1 = (*data).s6, w1 = (*data).s7;
        float mu2 = (*data).s9, sigma2 = (*data).sa;
        float w2=0.0f;


        if(w0>0.0f && w1>0.0f)
            w2=1-(*data).s3-(*data).s7;  

        short Nobs0 = (short)(*data).s4, Nobs1 = (short)(*data).s8, Nobs2 = (short)(*data).sb; 
        float Nobs_mix = (*data).sc;

        update_gauss_3_mixture(aux_data.y/aux_data.x,
                               aux_data.w/aux_data.x,
                               t_match,
                               init_sigma,min_sigma,
                               &mu0,&sigma0,&w0,&Nobs0,
                               &mu1,&sigma1,&w1,&Nobs1,
                               &mu2,&sigma2,&w2,&Nobs2,
                               &Nobs_mix);
        (*data).s0*=aux_data.z/aux_data.x;
        (*data).s1=mu0; (*data).s2=sigma0, (*data).s3=w0;(*data).s4=(float)Nobs0;
        (*data).s5=mu1; (*data).s6=sigma1, (*data).s7=w1;(*data).s8=(float)Nobs1;
        (*data).s9=mu2; (*data).sa=sigma2, (*data).sb=(float)Nobs2;
        (*data).sc=(float)Nobs_mix;
    }
}



__kernel void proc_norm_image(__global float4* image, __global float4* p_inf,__global uint4   * imgdims)
{
    /* linear global id of the normalization image */
    int lgid = get_global_id(0) + get_global_size(0)*get_global_id(1);

    int i=0;
    int j=0;
    map_work_space_2d(&i,&j);
    
    if (i>=(*imgdims).z && j>=(*imgdims).w)
        return;

    float4 vect = image[j*get_global_size(0)+i];
    float mult = (p_inf[0].x>0.0f) ? 1.0f :
        gauss_prob_density(vect.x, p_inf[0].y, p_inf[0].z);
    /* compute the norm image */
    vect.x = vect.w + mult * vect.z;
    /* the following  quantities have to be re-initialized before
    *the bayes_ratio kernel is executed
    */
    vect.y = 0.0f;/* clear alpha integral */
    vect.z = 1.0f; /* initial vis = 1.0 */
    vect.w = 0.0f; /* initial pre = 0.0 */
    /* write it back */
    image[j*get_global_size(0)+i] = vect;
}


