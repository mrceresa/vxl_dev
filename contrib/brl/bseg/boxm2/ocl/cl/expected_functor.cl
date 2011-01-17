
void step_cell_render(__global int2   * cell_data, 
                      __global float  * alpha_data, 
                               int      data_ptr, 
                               float    d, 
                               float  * vis,
                               float  * expected_i)
{
  float alpha = alpha_data[data_ptr];
  float diff_omega=exp(-alpha*d);
  float expected_int_cell=0.0f;
  // for rendering only
  if(diff_omega<0.995f)
  {
      uchar8 data = as_uchar8(cell_data[data_ptr]);
      expected_int_cell = ((data.s0) * (data.s2)
                          +(data.s3) * (data.s5)
                          +(data.s6) * (255.0f - data.s2 - data.s5))*1.53787005e-5f; // 1/255.0f/255.0f;
  }
  float omega=(*vis) * (1.0f - diff_omega);
  (*vis) *= diff_omega;
  (*expected_i)+=expected_int_cell*omega;
}

void step_cell_change_detection_uchar8(__global uchar8* cell_data, __global float* alpha_data,int data_ptr,
                                float d, float * vis,float * change_density, float img_intensity)
{
  uchar8 uchar_data = cell_data[data_ptr];
  float alpha=alpha_data[data_ptr];

  if (alpha<0) return;

  float8 data= convert_float8(uchar_data)/255.0f;

  float prob_den=gauss_3_mixture_prob_density(img_intensity,
                                              data.s0,data.s1,data.s2,
                                              data.s3,data.s4,data.s5,
                                              data.s6,data.s7,1-data.s2-data.s5);


  float vis_prob_end = exp(-alpha*d);
  float omega = (*vis)*(1 - vis_prob_end);
  (*change_density) += prob_den*omega;
  (*vis)=(*vis)*vis_prob_end;
}
void step_cell_change_detection_uchar8_w_expected(__global uchar8* cell_data, __global float* alpha_data,int data_ptr,
                                float d, float * vis,float * change_density,float * e_change_density, float img_intensity,float e_img_intensity)
{
  uchar8 uchar_data = cell_data[data_ptr];
  float8 data= convert_float8(uchar_data)/255.0f;

  float prob_den=gauss_3_mixture_prob_density(img_intensity,
                                              data.s0,data.s1,data.s2,
                                              data.s3,data.s4,data.s5,
                                              data.s6,data.s7,1-data.s2-data.s5);
  float alpha=alpha_data[data_ptr];



  float prob = 1-exp(-alpha*d);
  float omega = (*vis)*prob;
  (*vis)=(*vis)*(1-prob);
  (*change_density) += prob_den*omega;

  //float expected_int_cell = data.s0*data.s2+data.s3*data.s5+data.s6*(1-data.s2-data.s5);

  //float var=(e_img_intensity-data.s0)*(e_img_intensity-data.s0)*data.s2+
  //          (e_img_intensity-data.s3)*(e_img_intensity-data.s3)*data.s5+
  //          (e_img_intensity-data.s6)*(e_img_intensity-data.s6)*(1-data.s2-data.s5);

  float e_prob_den=gauss_3_mixture_prob_density(e_img_intensity,
                                              data.s0,data.s1,data.s2,
                                              data.s3,data.s4,data.s5,
                                              data.s6,data.s7,1-data.s2-data.s5);


  (*e_change_density) += e_prob_den*omega;

}

void step_cell_change_detection(__global uchar8* cell_data, __global float* alpha_data,int data_ptr,
                                float d, float4 * data_return, float img_intensity)
{
  uchar8 uchar_data = cell_data[data_ptr];
  float alpha=alpha_data[data_ptr];

  if (alpha<0) return;

  //float expected_int_cell = data.s3;
  float alpha_integral = (*data_return).x;
  float vis            = (*data_return).y;
  float change_density = (*data_return).z;
  float intensity_norm = (*data_return).w;

  float8 data= convert_float8(uchar_data)/255.0f;

  float prob_den=gauss_3_mixture_prob_density(img_intensity,
                                              data.s0,data.s1,data.s2,
                                              data.s3,data.s4,data.s5,
                                              data.s6,data.s7,1-data.s2-data.s5);

  alpha_integral += alpha*d;
  float vis_prob_end = exp(-alpha_integral);
  float omega = vis - vis_prob_end;
  change_density += prob_den*omega;
  (*data_return).x = alpha_integral;
  (*data_return).y = vis_prob_end;
  (*data_return).z = change_density;
  (*data_return).w = intensity_norm + omega;
}
void step_cell_render_depth(__global float* alpha_data,int data_ptr,
                            float d,float depth, float4 * data_return)
{
  float alpha=alpha_data[data_ptr];

  if (alpha<0) return;

  float alpha_integral = (*data_return).x;
  float vis            = (*data_return).y;
  float expected_depth = (*data_return).z;
  float norm           = (*data_return).w;

  alpha_integral += alpha*d;

  float vis_prob_end = exp(-alpha_integral);
  float omega = vis - vis_prob_end;

  expected_depth += depth*omega;
  (*data_return).x = alpha_integral;
  (*data_return).y = vis_prob_end;
  (*data_return).z = expected_depth;
  (*data_return).w = norm + omega;
}

void step_cell_visibility(__global float* cell_data, int data_ptr,
                          float d, float * data_return)
{
  float alpha = cell_data[data_ptr];
  float alpha_integral = (*data_return);
  alpha_integral += alpha*d;
  (*data_return) = alpha_integral;
}


void step_cell_render_depth2(float depth, 
                      __global float  * alpha_data, 
                               int      data_ptr, 
                               float    d, 
                               float  * vis,
                               float  * expected_depth)
{
  float alpha = alpha_data[data_ptr];
  float diff_omega=exp(-alpha*d);
  float omega=(*vis) * (1.0f - diff_omega);
  (*vis) *= diff_omega;
  (*expected_depth)+=depth*omega;
}


