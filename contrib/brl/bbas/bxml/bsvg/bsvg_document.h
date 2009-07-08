//:
// \file
// \brief classes based on bxml to create SVG documents
//        create an XML document with SVG primitives (text, lines, ellipses, polygons, etc.) and display them using any browser
//
// \author Ozge C. Ozcanli (Brown)
// \date   April 21, 2009
//
// \verbatim
//  Modifications
//   Ozge C. Ozcanli - July 08, 09 - ported to vxl from local repository - minor fixes
// \endverbatim

#if !defined(_bsvg_document_h)
#define _bsvg_document_h

#include <bxml/bxml_document.h>
#include <vcl_string.h>

class bsvg_element;

class bsvg_document : public bxml_document
{
public:
    bsvg_document(float w, float h);  // if viewBox params are not specified, then each pixel corresponds to one unit square while drawing
    bsvg_document(float w, float h, float viewBox_x, float viewBox_y, float viewBox_w, float viewBox_h);
    bool add_description(const vcl_string& d);
    bool add_element(const bxml_data_sptr& element);

public:
    float w_;
    float h_;

};

#endif  //_bsvg_document_h
