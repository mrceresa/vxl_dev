
#include <boxm/ocl/view/boxm_cam_tableau.h>
#include <vgui/vgui_gl.h>
#include <vgui/vgui_glu.h>
#include <vgui/internals/trackball.h>
#include <vgui/vgui.h>
#include <vgui/vgui_event.h>
#include <vgl/vgl_distance.h>


boxm_cam_tableau::boxm_cam_tableau() :
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
void boxm_cam_tableau::setup_gl_matrices()
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

//: Handles tableau key and drag events  
bool boxm_cam_tableau::handle(vgui_event const &e)
{
  //cam tableau just handles key presses and mouse manipulation
  if (e.type == vgui_KEY_PRESS) 
  {
    if(e.key == vgui_key('r')) {
        vcl_cout<<"resetting to initial camera view"<<vcl_endl;
        stare_point_ = vgl_homg_point_3d<double>(0,0,0);
        cam_.set_camera_center(default_cam_.get_camera_center());
        cam_.set_rotation(default_cam_.get_rotation());
    }
    else if(e.key == vgui_key('x')) {
        vcl_cout<<"looking down X axis at the origin"<<vcl_endl;
        stare_point_ = vgl_homg_point_3d<double>(0,0,0);
        cam_.set_camera_center(vgl_point_3d<double>(2,0,0));
        cam_.look_at(vgl_homg_point_3d<double>(0,0,0));
    }
    else if(e.key == vgui_key('y')) {
        vcl_cout<<"looking down Y axis at the origin"<<vcl_endl;
        stare_point_ = vgl_homg_point_3d<double>(0,0,0);
        cam_.set_camera_center(vgl_point_3d<double>(0,2,0));
        cam_.look_at(vgl_homg_point_3d<double>(0,0,0));
    }
    else if(e.key == vgui_key('z')) { 
        vcl_cout<<"looking down Z axis at the origin"<<vcl_endl;
        stare_point_ = vgl_homg_point_3d<double>(0,0,0);
        cam_.set_camera_center(vgl_point_3d<double>(0,0,2));
        cam_.look_at(vgl_homg_point_3d<double>(0,0,0));
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
bool boxm_cam_tableau::mouse_down(int x, int y, vgui_button /*button*/, vgui_modifier /*modifier*/)
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
bool boxm_cam_tableau::mouse_drag(int x, int y, vgui_button button, vgui_modifier modifier)
{    
  vcl_cout<<"Cam @ "<<cam_.get_camera_center()<<vcl_endl;
  // SPINNING
  if (c_mouse_rotate(button, modifier)) {
    
    //get viewport height and width
    GLdouble vp[4];
    glGetDoublev(GL_VIEWPORT, vp); // ok
    float width  = (float)vp[2];
    float height = (float)vp[3];

    // get mouse deltas 
    double dx = (beginx - x) / width;
    double dy = (beginy - y) / height;
      
    //cam location in spherical coordinates (this should be relative to the stare point
    vgl_point_3d<double> cam_center = cam_.get_camera_center();
    vgl_point_3d<double> origin(0.0f,0.0f,0.0f);
    double rad = vgl_distance<double>(cam_center, origin);
    double theta = vcl_acos(cam_center.z()/rad);
    double phi = vcl_atan2(cam_center.y(), cam_center.x());
    
    //update theta by a function of dy 
    double angleScale = .1;
    double newTheta = theta - dy * angleScale;
    double newPhi = phi + dx * angleScale;
    vgl_point_3d<double> newCenter(rad * vcl_sin(newTheta) * vcl_cos(newPhi), 
                                   rad * vcl_sin(newTheta) * vcl_sin(newPhi),
                                   rad * vcl_cos(newTheta));
    cam_.set_camera_center(newCenter);
    cam_.look_at(stare_point_);      

    vcl_cout<<cam_;
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
    vcl_cout<<"Translating: begin ("<<beginx<<","<<beginy<<") -> ("<<x<<","<<y<<")"<<vcl_endl;
    
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
    
    vcl_cout<<"  new cam center: "<<cam_center + worldVec<<vcl_endl;
    stare_point_ += worldVec;
    cam_.set_camera_center(cam_center + worldVec);

    this->post_redraw();
    return true;
  }
  return false;
}

//TODO i don't think this really does anything. 
bool boxm_cam_tableau::mouse_up(int x, int y, vgui_button button, vgui_modifier modifier)
{
  // SPINNING
  if (c_mouse_rotate(button, modifier))
  {
    GLdouble vp[4];
    glGetDoublev(GL_VIEWPORT, vp); // ok
    double width = vp[2];
    double height = vp[3];

    double wscale = 2.0 / width;
    double hscale = 2.0 / height;
    float delta_r[4];
    trackball(delta_r,
              static_cast<float>(wscale*beginx - 1), static_cast<float>(hscale*beginy - 1),
              static_cast<float>(wscale*x - 1), static_cast<float>(hscale*y - 1));
  }
  return false;
}
