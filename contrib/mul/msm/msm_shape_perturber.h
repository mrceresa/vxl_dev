#ifndef __msm_shape_perturber_h__
#define __msm_shape_perturber_h__

#include <vnl/vnl_random.h>
#include <vnl/vnl_vector.h>

#include <msm/msm_shape_model.h>
#include <msm/msm_shape_instance.h>
#include <msm/msm_aligner.h>

//:
// \file
// \brief Class for perturbing shapes from ground truth
// \author Patrick Sauer

class msm_shape_perturber
{
  private:

    double rel_gauss_;

    msm_shape_model    sm_;
    msm_shape_instance sm_inst_;

    msm_shape_instance gt_inst_;

    vnl_random rand_;  
  
    vnl_vector<double> max_dpose_;
    vnl_vector<double> max_dparams_;

    vnl_vector<double> dpose_,    inv_dpose_;
    vnl_vector<double> dparams_,  inv_dparams_;
    vnl_vector<double> all_, dall_, inv_dall_;

    size_t n_pose_;
    size_t n_params_;

    double trunc_normal_sample( vnl_random& rand1, double sd, double max_d );
    double random_value( vnl_random& rand, double max_v, double rel_gauss_sd );    
  
  public:
    
    msm_shape_perturber();
  
    void perturb( const msm_points& );

    void set_model( const msm_shape_model& );
    void set_max_dp( const vnl_vector<double>&, const vnl_vector<double>& = vnl_vector<double>() );
    void set_rel_gauss( double );
    void set_seed( size_t );

    size_t seed() const;
    double rel_gauss() const;

    const vnl_vector<double>& max_d_pose() const;
    const vnl_vector<double>& max_d_params() const;    

    const vnl_vector<double>& gt_params() const;
    const vnl_vector<double>& gt_pose() const;

    //: return model parameters of perturbed points
    const vnl_vector<double>& params() const;

    //: return pose of perturbed points
    const vnl_vector<double>& pose() const;

    //: return concatenated [pose params] vector of perturbed points
    const vnl_vector<double>& all() const;

    //: return params displacement needed to go from perturbed points back to ground truth
    const vnl_vector<double>& inv_d_params() const;

    //: return pose displacement needed to go grom perturbed points back to ground truth
    const vnl_vector<double>& inv_d_pose() const;
   
    //: return pose+params displacement needed to go grom perturbed points back to ground truth
    const vnl_vector<double>& inv_d_all() const;

    //: return perturbed points
    const msm_points& points() const;
};

#endif

