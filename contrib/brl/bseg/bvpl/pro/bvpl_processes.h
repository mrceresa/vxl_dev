#ifndef bvpl_processes_h_
#define bvpl_processes_h_

#include <bprb/bprb_func_process.h>
#include <bprb/bprb_macros.h>

DECLARE_FUNC_CONS(bvpl_neighborhood_operator_process);
DECLARE_FUNC_CONS(bvpl_vector_operator_process);
DECLARE_FUNC_CONS(bvpl_create_edge2d_kernel_process);
DECLARE_FUNC_CONS(bvpl_create_edge3d_kernel_process);
DECLARE_FUNC_CONS(bvpl_create_gauss3d_x_kernel_process);
DECLARE_FUNC_CONS(bvpl_create_gauss3d_xx_kernel_process);
DECLARE_FUNC_CONS(bvpl_create_edge2d_kernel_vector_process);
DECLARE_FUNC_CONS(bvpl_create_edge3d_kernel_vector_process);
DECLARE_FUNC_CONS(bvpl_create_corner2d_kernel_vector_process);
DECLARE_FUNC_CONS(bvpl_create_corner2d_kernel_process);
DECLARE_FUNC_CONS(bvpl_create_gauss3d_xx_kernel_vector_process);
DECLARE_FUNC_CONS(bvpl_convert_direction_to_hue_process);
DECLARE_FUNC_CONS(bvpl_convert_id_to_hue_process);
DECLARE_FUNC_CONS(bvpl_compare_surface_and_normal_process);
DECLARE_FUNC_CONS(bvpl_susan_opinion_operator_process);
DECLARE_FUNC_CONS(bvpl_kernel_id_to_axis_process);
DECLARE_FUNC_CONS(bvpl_non_max_suppression_process);
DECLARE_FUNC_CONS(bvpl_visualize_response_process);
DECLARE_FUNC_CONS(bvpl_suppress_and_combine_process);
DECLARE_FUNC_CONS(bvpl_extract_top_response_process);
DECLARE_FUNC_CONS(bvpl_operate_ocp_and_app_process);

//util
DECLARE_FUNC_CONS(bvpl_find_corner_pairs_process);
DECLARE_FUNC_CONS(bvpl_visualize_corner_pairs_process);
DECLARE_FUNC_CONS(bvpl_create_wc_kernel_vector_process);
DECLARE_FUNC_CONS(bvpl_convert_pair_to_hue_process);
DECLARE_FUNC_CONS(bvpl_load_pair_grid_process);
DECLARE_FUNC_CONS(bvpl_find_pairs_process_process);
DECLARE_FUNC_CONS(bvpl_pair_to_float_process);
DECLARE_FUNC_CONS(bvpl_bundler_features_2d_compute_process);
DECLARE_FUNC_CONS(bvpl_bundler_features_2d_write_txt_process);

#endif
