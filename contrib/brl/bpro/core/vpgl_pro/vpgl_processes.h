#ifndef vpgl_processes_h_
#define vpgl_processes_h_

#include <bprb/bprb_macros.h>
#include <bprb/bprb_func_process.h>

// execute and const functions
DECLARE_FUNC_CONS(vpgl_load_proj_camera_process);
DECLARE_FUNC_CONS(vpgl_load_local_rational_camera_process);
DECLARE_FUNC_CONS(vpgl_load_rational_camera_process);
DECLARE_FUNC_CONS(vpgl_load_rational_camera_nitf_process);
DECLARE_FUNC_CONS(vpgl_load_perspective_camera_process);
DECLARE_FUNC_CONS(vpgl_save_perspective_camera_process);
DECLARE_FUNC_CONS(vpgl_scale_perspective_camera_process);
DECLARE_FUNC_CONS(vpgl_save_rational_camera_process);
DECLARE_FUNC_CONS(vpgl_nitf_camera_coverage_process);
DECLARE_FUNC_CONS(vpgl_create_local_rational_camera_process);
DECLARE_FUNC_CONS(vpgl_create_local_rational_camera_nitf_process);
DECLARE_FUNC_CONS(vpgl_projective_to_rational_camera_process);
DECLARE_FUNC_CONS(vpgl_correct_rational_camera_process);
DECLARE_FUNC_CONS(vpgl_create_perspective_camera_process);
DECLARE_FUNC_CONS(vpgl_save_perspective_camera_vrml_process);
DECLARE_FUNC_CONS(vpgl_correct_rational_cameras_process);
DECLARE_FUNC_CONS(vpgl_correct_rational_cameras_mult_corr_process);
DECLARE_FUNC_CONS(vpgl_correct_rational_cameras_mult_corr_refine_process);
DECLARE_FUNC_CONS(vpgl_project_process);
DECLARE_FUNC_CONS(vpgl_convert_to_generic_camera_process);
DECLARE_FUNC_CONS(vpgl_resample_perspective_camera_process);

#endif
