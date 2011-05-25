#ifndef VPGL_BUNDLER_SETTINGS
#define VPGL_BUNDLER_SETTINGS

/*-----------------Settings structs for the features stages---------------*/

//Settings for the feature detection.
struct vpgl_bundler_settings_sift{
    float keypoint_curve_ratio; //Used in bapl_keypoint_extractor.
    unsigned sift_octave_size; //Used in bapl_dense_sift
    unsigned sift_num_octaves; //Used in bapl_dense_sift

    vpgl_bundler_settings_sift() : 
        keypoint_curve_ratio(10.0f), sift_octave_size(6), 
        sift_num_octaves(1) {}
};

//Settings for the propose stage. This stage doesn't actually use any
// settings, but we've included it for completeness.
typedef char vpgl_bundler_settings_propose_matches_all;

//Settings for the match stage. The single setting here is the maximum
// allowable distance ratio for a match to be considered valid.
struct vpgl_bundler_settings_match_ann{
    double min_dists_ratio;

    vpgl_bundler_settings_match_ann():
        min_dists_ratio(.6) { } 
};

//Settings for the refine stage. The settings have to do with 
// RANSAC and the f-matrix estimation.
struct vpgl_bundler_settings_refine_epipolar{
    //The desired probability of getting a good f matrix
    double probability_good_f_mat;

    //The maximum distance between the generated point and the matched
    // point for the point to be considered an outlier.
    double outlier_threshold;

    //The maximum expected porportion of the outliers in the set. This, as
    // well as the desired probability, is used to get the number of 
    // iterations to run.
    double max_outlier_frac;

    //The minimum number of inliers in order for two images to be mathched.
    int min_inliers;

    vpgl_bundler_settings_refine_epipolar():
        probability_good_f_mat(.95), outlier_threshold(9.0),
        max_outlier_frac(.5), min_inliers(16) { }
};

/*Same deal as _settings_propose_match_all*/
typedef char vpgl_bundler_settings_chain_matches;

struct vpgl_bundler_settings_tracks{
    vpgl_bundler_settings_sift detect_settings;
    vpgl_bundler_settings_propose_matches_all propose_matches_settings;
    vpgl_bundler_settings_match_ann match_settings;
    vpgl_bundler_settings_refine_epipolar refine_settings;
    vpgl_bundler_settings_chain_matches chain_matches_settings;
};


/*----------------Settings structs for the sfm stages.-------------------*/
struct vpgl_bundler_settings_create_initial_recon{
    
    /*Settings for estimating homographies using RANSAC*/
    int min_number_of_matches_homography;
    int number_ransac_rounds_homography;
    double inlier_threshold_homography; 
    
    /*Pretty much the same settings for estimating essential matrices*/
    int number_ransac_rounds_e_matrix;
    double inlier_threshold_e_matrix;
    
    vpgl_bundler_settings_create_initial_recon() :
        min_number_of_matches_homography(50),
        number_ransac_rounds_homography(128),
        inlier_threshold_homography(6.0),
        number_ransac_rounds_e_matrix(512),
        inlier_threshold_e_matrix(2.25){ }
};

struct vpgl_bundler_settings_select_next_images{
    //The minimum number of points already added that a camera must observe.
    // If no camera meets this description, the reconstruction is
    // considered complete.
    int min_number_observed_points;

    vpgl_bundler_settings_select_next_images():
        min_number_observed_points(20) { }
};

struct vpgl_bundler_settings_add_next_images{
    // The standard two ransac parameters for the 
    // projection matrix estimation.
    int number_ransac_rounds;
    double inlier_thresh;

    vpgl_bundler_settings_add_next_images() :
        number_ransac_rounds(4096),
        inlier_thresh(.4){ }
};

struct vpgl_bundler_settings_add_new_points{
    int dummy;
};

struct vpgl_bundler_settings_bundle_adjust{
    int dummy;
};

struct vpgl_bundler_settings_sfm{
    vpgl_bundler_settings_create_initial_recon initial_recon_settings;
    vpgl_bundler_settings_select_next_images select_next_images_settings;
    vpgl_bundler_settings_add_next_images add_next_images_settings;
    vpgl_bundler_settings_add_new_points add_new_points_settings;
    vpgl_bundler_settings_bundle_adjust bundle_adjust_settings;
};

/*-----------------------------------------------------------------------*/
/*Highest-level settings struct*/
struct vpgl_bundler_settings {
    vpgl_bundler_settings_tracks feature_settings;
    vpgl_bundler_settings_sfm sfm_settings;
};

#endif /*vpgl_bundler_settings*/
