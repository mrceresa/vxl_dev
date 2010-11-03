#include <boxm/ocl/view/boxm_vid_tableau.h>
//:
// \file
#include <vgui/vgui_gl.h>
#include <vgui/vgui_glu.h>
#include <vgui/internals/trackball.h>
#include <vgui/vgui.h>
#include <vgui/vgui_event.h>
#include <vgl/vgl_distance.h>
#include <vgl/vgl_ray_3d.h>


boxm_vid_tableau::boxm_vid_tableau() :
      c_mouse_rotate(vgui_LEFT),
      c_mouse_translate(vgui_LEFT, vgui_CTRL),
      c_mouse_zoom(vgui_MIDDLE)
{
  stare_point_ = vgl_homg_point_3d<double>(0,0,0);
  vgl_point_3d<double> def(2,2,2);
  cam_.set_camera_center(def);
  default_cam_.set_camera_center(def);
}

//: sets up viewport and GL Modes
void boxm_vid_tableau::setup_gl_matrices()
{
  GLint vp[4]; // x,y,w,h
  glGetIntegerv(GL_VIEWPORT, vp);
  glViewport(0, 0, vp[2], vp[3]);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, 1.0, 0.0, 1.0, 0.0, 1.0);
}


//: pans the camera in the direction of x and y (maintaining the look vector)
void boxm_vid_tableau::pan_camera(double dx, double dy) 
{
  
    ////get viewport height, and mouse dx, dy
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp); // ok
    double width = (double)vp[2];
    double height = (double)vp[3];

    //compute on screen coordinates
    double beginx = width/2.0;
    double beginy = height/2.0;
    double endx   = beginx + dx;
    double endy   = beginy + dy;
    double scale = .1;

    //get cam space points
    vgl_point_3d<double> p0(beginx/width, beginy/height, 0.0);
    vgl_point_3d<double> p1(endx/width, endy/height, 0.0);

    //transform points to world space
    vgl_point_3d<double> cam_center = cam_.get_camera_center();
    vgl_point_3d<double> wp0 = cam_.get_rotation().inverse()*p0;
    vgl_point_3d<double> wp1 = cam_.get_rotation().inverse()*p1;
    wp0 += cam_center - vgl_point_3d<double>(0,0,0);
    wp1 += cam_center - vgl_point_3d<double>(0,0,0);
    vgl_vector_3d<double> worldVec = scale * normalized(wp0-wp1);

    //reset camera center 
    cam_.set_camera_center(cam_center + worldVec);
}

void boxm_vid_tableau::zoom_camera(double dz)
{
    cam_.set_camera_center(cam_.get_camera_center() + dz*cam_.principal_axis());
}


//: Handles tableau key and drag events
bool boxm_vid_tableau::handle(vgui_event const &e)
{
  //cam tableau just handles key presses and mouse manipulation
  if (e.type == vgui_KEY_PRESS)
  {
    if (e.key == vgui_key('r')) {
        vcl_cout<<"resetting to initial camera view"<<vcl_endl;
        stare_point_ = vgl_homg_point_3d<double>(0,0,0);
        cam_.set_camera_center(default_cam_.get_camera_center());
        cam_.set_rotation(default_cam_.get_rotation());
    }
    else if (e.key == vgui_key('x')) {
        vcl_cout<<"looking down X axis at the origin"<<vcl_endl;
        stare_point_ = vgl_homg_point_3d<double>(0,0,0);
        cam_.set_camera_center(vgl_point_3d<double>(2,0,0));
        cam_.look_at(vgl_homg_point_3d<double>(0,0,0));
    }
    else if (e.key == vgui_key('y')) {
        vcl_cout<<"looking down Y axis at the origin"<<vcl_endl;
        stare_point_ = vgl_homg_point_3d<double>(0,0,0);
        cam_.set_camera_center(vgl_point_3d<double>(0,2,0));
        cam_.look_at(vgl_homg_point_3d<double>(0,0,0));
    }
    else if (e.key == vgui_key('z')) {
        vcl_cout<<"looking down Z axis at the origin"<<vcl_endl;
        stare_point_ = vgl_homg_point_3d<double>(0,0,0);
        cam_.set_camera_center(vgl_point_3d<double>(0,0,2));
        cam_.look_at(vgl_homg_point_3d<double>(0,0,0));
    }
    else if (e.key == vgui_CURSOR_LEFT || e.key == vgui_key('a')) {
        vcl_cout<<"Panning Left "<<vcl_endl;
        pan_camera(5.0, 0);
    }
    else if (e.key == vgui_CURSOR_RIGHT || e.key == vgui_key('d')) {
        vcl_cout<<"Panning Right "<<vcl_endl;
        pan_camera(-5.0, 0);
    }  
    else if (e.key == vgui_CURSOR_UP || e.key == vgui_key('w')) {
        vcl_cout<<"Moving forward "<<vcl_endl;
        zoom_camera(.1);
    }  
    else if (e.key == vgui_CURSOR_DOWN || e.key == vgui_key('s')) {
        vcl_cout<<"moving backward "<<vcl_endl;
        zoom_camera(-.1);
    } 
    this->post_redraw();
  }
  else if (e.type == vgui_RESHAPE)
  {
      this->setup_gl_matrices();
      this->post_redraw();
  }

  //see if drag should handle it
  event = e;
  if (vgui_drag_mixin::handle(e))
    return true;
  if (vgui_tableau::handle(e))
    return true;

  return false;
}

//: called on all mouse down events on tableau
bool boxm_vid_tableau::mouse_down(int x, int y, vgui_button /*button*/, vgui_modifier /*modifier*/)
{
  if (c_mouse_rotate(event) || c_mouse_translate(event) || c_mouse_zoom(event)) {
    beginx = x;
    beginy = y;
    last = event;
    return true;
  }
  return false;
}

//: called on mouse movement while mousedown is true
bool boxm_vid_tableau::mouse_drag(int x, int y, vgui_button button, vgui_modifier modifier)
{
  //vcl_cout<<"Cam @ "<<cam_.get_camera_center()<<vcl_endl;
  // SPINNING
  if (c_mouse_rotate(button, modifier)) {

    //get viewport height and width
    GLdouble vp[4];
    glGetDoublev(GL_VIEWPORT, vp); // ok
    float width  = (float)vp[2];
    float height = (float)vp[3];

    // get camera plane look point
    double scale = .1;
    double dx = -(beginx - x); // / width;
    double dy = (beginy - y); // / height;
    double camx = width/2.0 + scale*dx;
    double camy = height/2.0 + scale*dy;
    
    //back project and get a single point along the ray 
    vgl_ray_3d<double> ray        = cam_.backproject(camx, camy);
    vgl_homg_point_3d<double> newPoint = vgl_homg_point_3d<double>(ray.origin() + ray.direction() * 10.0);
    cam_.look_at(newPoint);
    this->post_redraw();
    return true;
  }

  // ZOOMING
  if (c_mouse_zoom(button, modifier)) {

    //get viewport size
    GLdouble vp[4];
    glGetDoublev(GL_VIEWPORT, vp); // ok
    double width = vp[2];
    double height = vp[3];

    //dy determines distance change in principle axis
    double dy = (beginy - y) / height;

    double scale = dy/2.0f;
    cam_.set_camera_center(cam_.get_camera_center() + scale*cam_.principal_axis());

    this->post_redraw();
    return true;
  }

  // TRANSLATION
  if (c_mouse_translate(button, modifier)) {

    //get viewport height, and mouse dx, dy
    GLint vp[4];
    glGetIntegerv(GL_VIEWPORT, vp); // ok
    double width = (double)vp[2];
    double height = (double)vp[3];
    double dx = (beginx - x) / width;
    double dy = (beginy - y) / height;
    double scale = .5 * vcl_sqrt(vcl_pow(dx,2.0) + vcl_pow(dy,2.0));

    //get cam space points
    vgl_point_3d<double> p0(beginx/width, beginy/height, 0.0);
    vgl_point_3d<double> p1(x/width, y/height, 0.0);

    //transform points to world space
    vgl_point_3d<double> cam_center = cam_.get_camera_center();
    vgl_point_3d<double> wp0 = cam_.get_rotation().inverse()*p0;
    vgl_point_3d<double> wp1 = cam_.get_rotation().inverse()*p1;
    wp0 += cam_center - vgl_point_3d<double>(0,0,0);
    wp1 += cam_center - vgl_point_3d<double>(0,0,0);
    vgl_vector_3d<double> worldVec = scale * normalized(wp0-wp1);

    //vcl_cout<<"  new cam center: "<<cam_center + worldVec<<vcl_endl;
    stare_point_ += worldVec;
    cam_.set_camera_center(cam_center + worldVec);

    this->post_redraw();
    return true;
  }
  return false;
}

//Necessary for drag_mixin
bool boxm_vid_tableau::mouse_up(int x, int y, vgui_button button, vgui_modifier modifier)
{
  return false;
}
