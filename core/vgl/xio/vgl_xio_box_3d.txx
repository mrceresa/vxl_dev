// This is core/vgl/xio/vgl_xio_box_3d.txx
#ifndef vgl_xio_box_3d_txx_
#define vgl_xio_box_3d_txx_
//:
// \file

#include "vgl_xio_box_3d.h"
#include <vgl/vgl_box_3d.h>
#include <vsl/vsl_basic_xml_element.h>

//============================================================================
//: Binary save self to stream.
template<class T>
void x_write(vcl_ostream &os, const vgl_box_3d<T> & p, vcl_string element_name)
{  
    vsl_basic_xml_element xml_element(element_name);
    xml_element.add_attribute("min_x", p.min_x());
    xml_element.add_attribute("min_y", p.min_y());
    xml_element.add_attribute("min_z", p.min_z());
    xml_element.add_attribute("max_x", p.max_x());
    xml_element.add_attribute("max_y", p.max_y());
    xml_element.add_attribute("max_z", p.max_z());
    xml_element.x_write(os);
}

#define VGL_XIO_BOX_3D_INSTANTIATE(T) \
template void x_write(vcl_ostream &, const vgl_box_3d<T > &, vcl_string)

#endif // vgl_xio_box_3d_txx_
