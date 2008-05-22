#include "bvxm_lidar_processor.h"
//:
// \file
#include "bvxm_voxel_slab.h"
#include "bvxm_voxel_slab_iterator.h"
#ifdef OPTION2 // currently FALSE
#include <vcl_cassert.h>
#endif

#include <vgl/vgl_point_2d.h>
#include <vgl/vgl_box_2d.h>
#include <vil/vil_image_view.h>

//: Return probability density of observing pixel values
bvxm_voxel_slab<float>
bvxm_lidar_processor::prob_density(float z_dim,  bvxm_voxel_slab<float> const& obs, float voxel_width )
{
  //the output
  bvxm_voxel_slab<float> probabilities(obs.nx(), obs.ny(), obs.nz());

  // Option 1: No LIDAR registration error.
  bvxm_voxel_slab<float>::const_iterator obs_it;
  bvxm_voxel_slab<float>::iterator prob_it = probabilities.begin();
  for (obs_it = obs.begin(); obs_it!= obs.end(); ++obs_it, ++prob_it)
  {
    float d;

    d = *obs_it - z_dim;

    // Calculate threshold such that the given number of pixels corresponds to
    // 2.5 standard deviations away in a standard normal gaussian
    float thresh = 2.5f / float(vox_thresh_);
    d *= thresh;

    *prob_it = gauss_.prob_density(d);
  }

#ifdef OPTION2 // currently FALSE
  // Option 2: As in thesis.
  assert( obs.nz() == 1 ); // Code assumes this.
  int search_rad = 4;
  for ( unsigned i = 0; i < obs.nx(); i++ ){
    for ( unsigned j = 0; j < obs.ny(); j++ ){
      int min_i = i-search_rad; if ( min_i < 0 ) min_i = 0;
      int min_j = j-search_rad; if ( min_j < 0 ) min_j = 0;
      int max_i = i+search_rad; if ( max_i >= (int)obs.nx() ) max_i = obs.nx()-1;
      int max_j = j+search_rad; if ( max_j >= (int)obs.ny() ) max_j = obs.ny()-1;
      float min_d;
      for ( int ni = min_i; ni <= max_i; ni++ ){
        for ( int nj = min_j; nj <= max_j; nj++ ){
          float d = obs(ni,nj,0)-z_dim;
          d = vcl_sqrt( d*d + (ni-min_i)*(ni-min_i)*voxel_width*voxel_width + (nj-min_j)*(nj-min_j)*voxel_width*voxel_width );
          if ( ni == min_i || nj == min_j ) min_d = d;
          if ( d < min_d ) min_d = d;
        }
      }

      float thresh = 2.5f / float(vox_thresh_);
      min_d *= thresh;
      probabilities(i,j,0) = gauss_.prob_density(min_d);
    }
  }
#endif // OPTION2
  return probabilities;
}

float bvxm_lidar_processor::prob_density(vil_image_view_base_sptr lidar,
                                        float z_dim,//vgl_point_3d<float>& mean,
                                        vnl_vector_fixed<float,3>& covar,  // sigma vals
                                        vgl_box_2d<double> lidar_roi,
                                        float voxel_width) 
{
  //vcl_cout << "FORMAT=" << lidar->pixel_format();
  vgl_point_2d<double> roi_min(lidar_roi.min_x(), lidar_roi.min_y());
  vgl_point_2d<double> roi_max(lidar_roi.max_x(), lidar_roi.max_y());

  vnl_vector_fixed<float,3> min(lidar_roi.min_x(), lidar_roi.min_y(), z_dim-0.5*voxel_width);//multiply 1/2 with voxel size!!!1
  vnl_vector_fixed<float,3> max(lidar_roi.max_x(), lidar_roi.max_y(), z_dim+0.5*voxel_width);
  //vcl_cout << "  min-->" << min << "  max->" << max << vcl_endl;
  
  int min_i = (int) vcl_floor(lidar_roi.min_x()); if( min_i < 0 ) min_i = 0;
  int min_j = (int) vcl_floor(lidar_roi.min_y()); if( min_j < 0 ) min_j = 0;
  int max_i = (int) vcl_ceil(lidar_roi.max_x()); if( max_i >= (int)lidar->ni() ) max_i = lidar->ni()-1;
  int max_j = (int) vcl_ceil(lidar_roi.max_y()); if( max_j >= (int)lidar->nj() ) max_j = lidar->nj()-1;

  float p = 1.0, d;
  for( int ni = min_i; ni < max_i; ni++ ){
    for( int nj = min_j; nj < max_j; nj++ ){
      if (lidar->pixel_format() == VIL_PIXEL_FORMAT_BYTE) {
        if (vil_image_view<unsigned char> *img_view = dynamic_cast<vil_image_view<unsigned char>*>(lidar.ptr())) 
          d = (*img_view)(ni, nj);
      } else if (lidar->pixel_format() == VIL_PIXEL_FORMAT_FLOAT) {
        if (vil_image_view<float> *img_view = dynamic_cast<vil_image_view<float>*>(lidar.ptr())) 
          d = (*img_view)(ni, nj);
      }
      //vcl_cout << "       lidar pt -->(" << ni << " " << nj << " " << d << ")" << vcl_endl;
      vnl_vector_fixed<float,3> m(ni+0.5, nj+0.5, d-2);
      //vcl_cout << "      mean=" << m << vcl_endl;
      bsta_gauss_if3 gauss(m, covar);
      float p1 = gauss.probability(min,max);
      //vcl_cout << "         p=" << p1 << vcl_endl;
      p *= 1 - p1;
   }
  }
  return (1 - p);
}
