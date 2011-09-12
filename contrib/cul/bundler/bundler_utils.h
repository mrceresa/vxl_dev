#ifndef BUNDLER_UTILS_H
#define BUNDLER_UTILS_H
//:
// \file

#include <vcl_vector.h>

#include <vgl/vgl_point_2d.h>
#include <vnl/vnl_double_3x3.h>

#include <mrc/vpgl/vpgl_perspective_camera.h>
#include <bundler/bundler_inters.h>

// Generally useful function used for RANSAC.
// Randomly chooses n distinct indices into the set
void bundler_utils_get_distinct_indices(
    int n, int *idxs, int number_entries);

// Takes in a list of points and
// cameras, and finds the least-squared solution to the intersection
// of the rays generated by the points.
// TODO: TEST THIS. I think it's broken.
double bundler_utils_triangulate_points(
    const bundler_inters_track_sptr &track);

// Estimates a homography and returns the percentage of inliers
double bundler_utils_get_homography_inlier_percentage(
    const bundler_inters_match_set &match,
    double threshold_squared, int num_rounds);


int bundler_utils_fill_persp_camera_ransac(
    const bundler_inters_image_sptr &image,
    int ransac_rounds,
    double inlier_threshold);


#endif // BUNDLER_UTILS_H
