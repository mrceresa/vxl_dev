#include <vcl_iostream.h>

#include <vsol/vsol_point_2d.h>

#include <vtol/vtol_vertex_2d.h>
#include <vtol/vtol_edge_2d.h>

#include <vdgl/vdgl_edgel_chain.h>
#include <vdgl/vdgl_interpolator_linear.h>
#include <vdgl/vdgl_digital_curve_ref.h>
//#include <vdgl/vdgl_digital_region.h>

int main()
{
  // a set of edgels
  double data[]= {
    -0.49769585253456,  -0.36257309941520,
    -0.61290322580645,  -0.13450292397661,
    -0.64516129032258,   0.11111111111111,
    -0.60368663594470,   0.36842105263158,
    -0.42857142857143,   0.48538011695906,
    -0.19815668202765,   0.52046783625731,
    -0.00460829493088,   0.60818713450292,
    0.11520737327189,   0.74853801169591,
    0.32718894009217,   0.74269005847953,
    0.42857142857143,   0.53801169590643,
    0.43317972350230,   0.18713450292398,
    0.32258064516129,  -0.00584795321637,
    0.30414746543779,  -0.21052631578947,
    0.35023041474654,  -0.39766081871345,
    0.23963133640553,  -0.68421052631579,
    0.05069124423963,  -0.73099415204678,
    -0.12903225806452,  -0.63157894736842,
    -0.12442396313364,  -0.40935672514620,
    -0.15668202764977,  -0.22222222222222,
    -0.31336405529954,  -0.25146198830409,
    -999, -999 };

  // create a list of edgels from data
  //   note: grad = 0 and theta = 1 for all edgels
  vcl_vector<vdgl_edgel> edgels;

  for( int i=0; data[i]!= -999; i+= 2)
    edgels.push_back( vdgl_edgel( data[i], data[i+1], 0, 1));

  // Create an edgel chain...  
  vdgl_edgel_chain_ref edgel_chain= new vdgl_edgel_chain( edgels);

  // Define an interpolator (linear interpolation, here)
  vdgl_interpolator_linear interpolator_linear( edgel_chain);
  vdgl_interpolator_ref    interpolator( &interpolator_linear);

  // And use this to create a digital curve
  vdgl_digital_curve_ref digital_curve= new vdgl_digital_curve( interpolator);

  // Now we can create an edge (although first we need to define
  //  the starting and edging vertices of this edge---in this case
  //  they are the same)
  vsol_point_2d_ref pointstart( digital_curve->p0().ptr());
  vsol_point_2d_ref pointend( digital_curve->p1().ptr());

  //  vtol_vertex_2d vertexstart( pointstart);
  //  vtol_vertex_2d vertexend( pointend);
  vtol_vertex_2d_ref vertexstart= new vtol_vertex_2d( 1, 2);
  vtol_vertex_2d_ref vertexend  = new vtol_vertex_2d( 3, 4);


  //  vtol_edge_2d edge( vertexstart, vertexend, vsol_curve_2d_ref( digital_curve.ptr()));
  vtol_edge_2d edge( *(vertexstart.ptr()), *(vertexend.ptr()), digital_curve.ptr());

  // Sample 50 points along the curve and print them out
  for( double i=0; i< 1; i+=0.02)
    {
      double x= digital_curve->get_x(i);
      double y= digital_curve->get_y(i);

      vcl_cerr << x << "\t" << y << vcl_endl;
    }

  // And now play with the digital region
//   vdgl_digital_region digitalregion;
//   digitalregion.add_point( vgl_point_2d<double>(1,2));
//   digitalregion.add_point( vgl_point_2d<double>(4,3));
//   digitalregion.add_point( vgl_point_2d<double>(5,6));

//   vcl_cerr << digitalregion << vcl_endl;

  return 0;
}
