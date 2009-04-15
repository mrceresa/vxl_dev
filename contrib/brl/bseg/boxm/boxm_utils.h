#ifndef boxm_utils_h_
#define boxm_utils_h_

//:
// \file
// \brief  The utility methods for the boxm project
//
// \author Gamze Tunali
// \date 04/07/2009
// \verbatim
//  Modifications
//  
// \endverbatim

#include <vgl/vgl_box_3d.h>
#include <vgl/vgl_point_3d.h>
#include <vpgl/vpgl_camera.h>
#include <vsol/vsol_polygon_3d_sptr.h>
#include <boct/boct_tree_cell.h>
#include <vgl/vgl_polygon_scan_iterator.h>
#include <vil/vil_image_view.h>

class boxm_utils {

public:

    //: returns true if a given box is visible from the camera
    // and the image plane
  static bool is_visible(vgl_box_3d<double> const& bbox, 
                    vpgl_camera_double_sptr const& camera, 
                    unsigned int img_ni, unsigned int img_nj, 
                    bool do_front_test = true);


  static vcl_vector<vgl_point_3d<double> > corners_of_box_3d(vgl_box_3d<double> box);

  static bool is_face_visible(vcl_vector<vgl_point_3d<double> > &face,
                              vpgl_camera_double_sptr const& camera);

    //: returns the visible faces of a box given a camera. It puts
    // a bit 1 for each face visible based on the boct_cell_face values.
  static boct_face_idx visible_faces(vgl_box_3d<double> &bbox, 
                                vpgl_camera_double_sptr camera);

  static void faces_of_box_3d(vgl_box_3d<double> &bbox, 
                            vcl_map<boct_face_idx, vcl_vector<vgl_point_3d<double> > > & faces);

  static void project_cube(vgl_box_3d<double> &bbox, 
                           vpgl_camera_double_sptr camera,
                           vgl_polygon<double> &poly);

  
  static void quad_interpolate(vgl_polygon_scan_iterator<double> &poly_it, 
						 double* xvals, double* yvals, double* vals, 
						 vil_image_view<float> &min_img, vil_image_view<float> &max_img,
						 unsigned int v0=0, unsigned int v1=1, 
						 unsigned int v2=2,unsigned int v3=3);
static bool cube_camera_distance(vgl_box_3d<double> & cube,
						  vpgl_camera_double_sptr const& cam, 
						  vil_image_view<float> &dist);


};

#endif