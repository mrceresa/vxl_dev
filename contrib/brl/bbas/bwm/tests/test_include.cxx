#include <bwm/algo/bwm_algo.h>
#include <bwm/algo/bwm_image_processor.h>
#include <bwm/algo/bwm_lidar_algo.h>
#include <bwm/algo/bwm_plane_fitting_lsf.h>
#include <bwm/algo/bwm_rat_proj_camera.h>
#include <bwm/algo/bwm_soview2D_cross.h>
#include <bwm/algo/bwm_soview2D_vertex.h>
#include <bwm/algo/bwm_utils.h>
#include <bwm/algo/bwm_utm_lat_lon.h>

#include <bwm/io/bwm_io_config_parser.h>
#include <bwm/io/bwm_io_structs.h>
#include <bwm/io/bwm_site.h>
#include <bwm/io/bwm_site_sptr.h>

#include <bwm/bwm_command.h>
#include <bwm/bwm_command_macros.h>
#include <bwm/bwm_command_sptr.h>
#include <bwm/bwm_corr.h>
#include <bwm/bwm_corr_sptr.h>
#include <bwm/bwm_def.h>
#include <bwm/bwm_input.h>
#include <bwm/bwm_load_commands.h>
#include <bwm/bwm_macros.h>
#include <bwm/bwm_menu.h>
#include <bwm/bwm_observable.h>
#include <bwm/bwm_observable_mesh.h>
#include <bwm/bwm_observable_mesh_sptr.h>
#include <bwm/bwm_observable_point.h>
#include <bwm/bwm_observable_point_sptr.h>
#include <bwm/bwm_observable_polyline.h>
#include <bwm/bwm_observable_polyline_sptr.h>
#include <bwm/bwm_observable_sptr.h>
#include <bwm/bwm_observable_textured_mesh.h>
#include <bwm/bwm_observable_textured_mesh_sptr.h>
#include <bwm/bwm_observer.h>
#include <bwm/bwm_observer_cam.h>
#include <bwm/bwm_observer_cam_proj.h>
#include <bwm/bwm_observer_coin3d.h>
#include <bwm/bwm_observer_img.h>
#include <bwm/bwm_observer_lidar.h>
#include <bwm/bwm_observer_mgr.h>
#include <bwm/bwm_observer_proj2d.h>
#include <bwm/bwm_observer_proj2d_simple.h>
#include <bwm/bwm_observer_proj_cam.h>
#include <bwm/bwm_observer_rat_cam.h>
#include <bwm/bwm_observer_vgui.h>
#include <bwm/bwm_observer_video.h>
#include <bwm/bwm_popup_menu.h>
#include <bwm/bwm_process_commands.h>
#include <bwm/bwm_process_mgr.h>
#include <bwm/bwm_site_mgr.h>
#include <bwm/bwm_tableau.h>
#include <bwm/bwm_tableau_cam.h>
#include <bwm/bwm_tableau_coin3d.h>
#include <bwm/bwm_tableau_factory.h>
#include <bwm/bwm_tableau_img.h>
#include <bwm/bwm_tableau_lidar.h>
#include <bwm/bwm_tableau_mgr.h>
#include <bwm/bwm_tableau_proj2d.h>
#include <bwm/bwm_tableau_proj_cam.h>
#include <bwm/bwm_tableau_rat_cam.h>
#include <bwm/bwm_tableau_sptr.h>
#include <bwm/bwm_tableau_text.h>
#include <bwm/bwm_tableau_video.h>
#include <bwm/bwm_texture_map_generator.h>
#include <bwm/bwm_world.h>

int main() { return 0; }
