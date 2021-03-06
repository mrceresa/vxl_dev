#include "boxm2_cpp_register.h"

#include <bprb/bprb_macros.h>
#include <bprb/bprb_batch_process_manager.h>
#include <bprb/bprb_func_process.h>

#include "boxm2_cpp_processes.h"

void boxm2_cpp_register::register_datatype()
{
}

void boxm2_cpp_register::register_process()
{
  // utilities
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_render_expected_image_process, "boxm2CppRenderExpectedImageProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_render_cone_expected_image_process, "boxm2CppRenderConeExpectedImageProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_update_image_process, "boxm2CppUpdateImageProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_cone_update_image_process, "boxm2CppConeUpdateImageProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_refine_process2, "boxm2CppRefineProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_change_detection_process2, "boxm2CppChangeDetectionProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_query_cell_data_process, "boxm2CppQueryCellDataProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_vis_of_point_process, "boxm2CppVisOfPointProcess");

//  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_render_expected_depth_process, "boxm2CppRenderExpectedDepthProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_cast_intensities_process, "boxm2CppCastIntensitiesProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_mean_intensities_batch_process, "boxm2CppMeanIntensitiesBatchProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_mean_intensities_print_process, "boxm2CppMeanIntensitiesPrintProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_create_norm_intensities_process, "boxm2CppCreateNormIntensitiesProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_data_print_process, "boxm2CppDataPrintProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_filter_process, "boxm2CppFilterProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_compute_derivative_process, "boxm2CppComputeDerivativeProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_filter_response_process, "boxm2CppFilterResponseProcess");

  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_probe_intensities_process, "boxm2CppBatchProbeIntensitiesProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_ray_probe_process, "boxm2CppRayProbeProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_ray_app_density_process, "boxm2CppRayAppDensityProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_create_aux_data_process, "boxm2CppCreateAuxDataProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_update_process, "boxm2CppBatchUpdateProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_create_aux_data_opt2_process, "boxm2CppCreateAuxDataOPT2Process");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_update_opt2_process, "boxm2CppBatchUpdateOPT2Process");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_image_density_process, "boxm2CppImageDensityProcess");

  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_compute_shadow_model_process, "boxm2CppBatchComputeShadowModelProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_update_nonsurface_model_process, "boxm2CppBatchUpdateNonsurfaceModelProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_compute_phong_model_process, "boxm2CppBatchComputePhongModelProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_compute_synoptic_function_1d_process, "boxm2CppBatchComputeSynopticFunction1dProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_compute_normal_albedo_process, "boxm2CppBatchComputeNormalAlbedoProcess");

  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_query_cell_brdf_process, "boxm2CppQueryCellBrdfProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_pre_infinity_opt2_phongs_process, "boxm2CppPreInfinityOpt2PhongsProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_update_opt2_phongs_process, "boxm2CppBatchUpdateOpt2PhongsProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_update_nonray_process, "boxm2CppBatchUpdateNonrayProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_render_z_images_process, "boxm2CppRenderZImagesProcess");

  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_merge_process, "boxm2CppMergeProcess");

  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_update_app_process, "boxm2CppBatchUpdateAppProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_batch_update_alpha_process, "boxm2CppBatchUpdateAlphaProcess");

  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_update_with_shadow_process, "boxm2CppUpdateWithShadowProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_image_density_masked_process, "boxm2CppImageDensityMaskedProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_update_using_quality_process, "boxm2CppUpdateUsingQualityProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_points_to_volume_process, "boxm2CppPointsToVolumeProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_export_stack_images_process, "boxm2ExportStackImagesProcess");

  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_create_mog_image_process, "boxm2CppCreateMogImageProcess");
  REG_PROCESS_FUNC_CONS(bprb_func_process, bprb_batch_process_manager, boxm2_cpp_los_visibility_process, "boxm2CppLosVisibilityProcess");

}
