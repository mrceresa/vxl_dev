#include <vcl/vcl_fstream.h>#include <vtol/vtol_topology_io.h>#include <vtol/vtol_vertex_2d.h>#include <vtol/vtol_list_functions.h>#include <vtol/vtol_zero_chain.h>#include <vtol/vtol_edge_2d.h>#include <vtol/vtol_one_chain.h>#include <vtol/vtol_face_2d.h>#include <vtol/vtol_two_chain.h>#include <vtol/vtol_block.h>// default constructorvtol_topology_io::vtol_topology_io(){}// default destructorvtol_topology_io::~vtol_topology_io(){}// write out a list of topology objectsvoid vtol_topology_io::write(vcl_list<vtol_topology_object_sptr> &topo_objects,                             vcl_ostream &strm){  // write out the dtd table  strm << "<?xml version=\"1.0\" standalone=\"yes\"?>" << vcl_endl;  write_dtd(strm);  strm << "<vxl>" << vcl_endl;  // ****** get lists of all the topology types *******  // start off with getting a list of all the vertices  vcl_list<vtol_topology_object_sptr> vertices;  vcl_list<vtol_topology_object_sptr> zero_chains;  vcl_list<vtol_topology_object_sptr> edges;  vcl_list<vtol_topology_object_sptr> one_chains;  vcl_list<vtol_topology_object_sptr> faces;  vcl_list<vtol_topology_object_sptr> two_chains;  vcl_list<vtol_topology_object_sptr> blocks;  vcl_list<vtol_topology_object_sptr>::iterator ti;  for(ti=topo_objects.begin();ti!= topo_objects.end();ti++){    // *** get the vertices    vertex_list *vl = (*ti)->vertices();    // copy vl into vertices    vertex_list::iterator vi;    for(vi=vl->begin();vi!=vl->end();vi++){      vertices.push_back((*vi).ptr());    }    delete vl;    // *** get the zero chains    zero_chain_list *zcl = (*ti)->zero_chains();    // copy zcl into the zero_chains    zero_chain_list::iterator zci;    for(zci=zcl->begin();zci!=zcl->end();zci++){      zero_chains.push_back((*zci).ptr());    }    delete zcl;    // *** get the edges    edge_list *el = (*ti)->edges();    // copy zcl into the zero_chains    edge_list::iterator ei;    for(ei=el->begin();ei!=el->end();ei++){      edges.push_back((*ei).ptr());    }    delete el;    // *** get the one chains    one_chain_list *ocl = (*ti)->one_chains();    // copy ocl into the one_chains    one_chain_list::iterator oci;    for(oci=ocl->begin();oci!=ocl->end();oci++){      one_chains.push_back((*oci).ptr());    }    delete ocl;    // *** get the faces    face_list *fl = (*ti)->faces();    // copy fl into the faces    face_list::iterator fi;    for(fi=fl->begin();fi!=fl->end();fi++){      faces.push_back((*fi).ptr());    }    delete fl;    // *** get the two chains    two_chain_list *tcl = (*ti)->two_chains();    // copy tcl into the two_chains    two_chain_list::iterator tci;    for(tci=tcl->begin();tci!=tcl->end();tci++){      two_chains.push_back((*tci).ptr());    }    delete tcl;    // *** get the blocks    block_list *bl = (*ti)->blocks();    // copy bl into the blocks    block_list::iterator bi;    for(bi=bl->begin();bi!=bl->end();bi++){      blocks.push_back((*bi).ptr());    }    delete bl;  }  // ******* get rid of duplicates *********  tagged_union(&vertices);  tagged_union(&zero_chains);  tagged_union(&edges);  tagged_union(&one_chains);  tagged_union(&faces);  tagged_union(&two_chains);  tagged_union(&blocks);  // ******** write the topology objects  //  int id =1;  for(ti=vertices.begin();ti!= vertices.end();ti++){    vtol_vertex *v= (*ti)->cast_to_vertex();    if(v){      vtol_vertex_2d *v2d = v->cast_to_vertex_2d();      if(v2d){        // set the id of the vertex        v2d->set_id(id);        id++;        // write the vertex        write_vertex_2d(v2d,strm);      }    }  }  id =1;  for(ti=zero_chains.begin();ti!= zero_chains.end();ti++){    vtol_zero_chain *zc= (*ti)->cast_to_zero_chain();    if(zc){      zc->set_id(id);      id++;      // write the zero chain      write_zero_chain(zc,strm);    }  }  id =1;  for(ti=edges.begin();ti!= edges.end();ti++){    vtol_edge *e= (*ti)->cast_to_edge();    if(e){      vtol_edge_2d *e2d = e->cast_to_edge_2d();      if(e2d){        // set the id of the edge        e2d->set_id(id);        id++;        // write the edge        write_edge_2d(e2d,strm);      }    }  }  id =1;  for(ti=one_chains.begin();ti!= one_chains.end();ti++){    vtol_one_chain *oc= (*ti)->cast_to_one_chain();    if(oc){      oc->set_id(id);      id++;      // write the one_chain      write_one_chain(oc,strm);    }  }  id=1;  for(ti=faces.begin();ti!= faces.end();ti++){    vtol_face *f= (*ti)->cast_to_face();    if(f){      vtol_face_2d *f2d = f->cast_to_face_2d();      if(f2d){        // set the id of the face        f2d->set_id(id);        id++;        // write the face        write_face_2d(f2d,strm);      }    }  }  id =1;  for(ti=two_chains.begin();ti!= two_chains.end();ti++){    vtol_two_chain *tc= (*ti)->cast_to_two_chain();    if(tc){      tc->set_id(id);      id++;      // write the two_chain      write_two_chain(tc,strm);    }  }  id =1;  for(ti=blocks.begin();ti!= blocks.end();ti++){    vtol_block *b= (*ti)->cast_to_block();    if(b){      b->set_id(id);      id++;      // write the block      write_block(b,strm);    }  }  strm << "</vxl>" << vcl_endl;}// write out a 2d vertexvoid vtol_topology_io::write_vertex_2d(vtol_vertex_2d *v,                                       vcl_ostream &strm){  // ok lets write this vertex out  // it looks something like this  //    <vxl_vertex_2d id="v1">  //       <vxl_point_2d id="p1" x="0" y="0"/>  //    </vxl_vertex_2d>  strm << "<vxl_vertex_2d id=\"v2d" << v->get_id() << "\">" << vcl_endl;  strm << " <vxl_point_2d x=\""<< v->x() << "\"" << " y=\"" << v->y() << "\"/>" << vcl_endl;  strm << "</vxl_vertex_2d>" << vcl_endl;}void vtol_topology_io::write_zero_chain(vtol_zero_chain *zc,                                        vcl_ostream &strm){  strm << "<vxl_zero_chain_2d id=\"zc_" << zc->get_id() << "\">" << vcl_endl;  // write the inferiors  const vcl_vector<vtol_topology_object_sptr> *inferiors = zc->inferiors();  vcl_vector<vtol_topology_object_sptr>::const_iterator i;  for(i=inferiors->begin();i!=inferiors->end();++i) {    strm << " <vxl_vertex_2d_sptr id=\"v2d" << (*i)->get_id() << "\"/>" << vcl_endl;  }  strm << "</vxl_zero_chain_2d>" << vcl_endl;}void vtol_topology_io::write_edge_2d(vtol_edge_2d *e, vcl_ostream &strm){  strm << "<vxl_edge_2d id=\"e2d" << e->get_id() << "\">" << vcl_endl;  // write the inferiors  const vcl_vector<vtol_topology_object_sptr> *inferiors = e->inferiors();  vcl_vector<vtol_topology_object_sptr>::const_iterator i;  for(i=inferiors->begin();i!=inferiors->end();++i) {    strm << " <vxl_zero_chain_2d_sptr id=\"zc_" << (*i)->get_id() << "\"/>" << vcl_endl;  }  strm << "</vxl_edge_2d>" << vcl_endl;}void vtol_topology_io::write_one_chain(vtol_one_chain *oc, vcl_ostream &strm){  strm << "<vxl_one_chain_2d id=\"oc_" << oc->get_id() << "\">" << vcl_endl;  // write the inferiors  const vcl_vector<vtol_topology_object_sptr> *inferiors = oc->inferiors();  vcl_vector<vtol_topology_object_sptr>::const_iterator i;  for(i=inferiors->begin();i!=inferiors->end();++i) {    vtol_edge *e = (*i)->cast_to_edge();    if(e){      if(e->cast_to_edge_2d()){        // deterimine the direction of the edge        if(oc->direction(*e) ==1){          strm << " <vxl_edge_2d_sptr id=\"e2d" << (*i)->get_id() << "\"/>" << vcl_endl;        }        else{          // we have to signal a flip          strm << " <vxl_edge_2d_sptr id=\"e2d" << (*i)->get_id() << "\"/><vxl_flip/>"  << vcl_endl;        }      }    }  }  strm << "</vxl_one_chain_2d>" << vcl_endl;}void vtol_topology_io::write_face_2d(vtol_face_2d *f, vcl_ostream &strm){  strm << "<vxl_face_2d id=\"f2d" << f->get_id() << "\">" << vcl_endl;  // write the inferiors  const vcl_vector<vtol_topology_object_sptr> *inferiors = f->inferiors();  vcl_vector<vtol_topology_object_sptr>::const_iterator i;  for(i=inferiors->begin();i!=inferiors->end();++i) {    strm << " <vxl_one_chain_2d_sptr id=\"oc_" << (*i)->get_id() << "\"/>" << vcl_endl;  }  strm << "</vxl_face_2d>" << vcl_endl;}void vtol_topology_io::write_two_chain(vtol_two_chain *tc, vcl_ostream &strm){  strm << "<vxl_two_chain_2d id=\"tc_" << tc->get_id() << "\">" << vcl_endl;  // write the inferiors  const vcl_vector<vtol_topology_object_sptr> *inferiors = tc->inferiors();  vcl_vector<vtol_topology_object_sptr>::const_iterator i;  for(i=inferiors->begin();i!=inferiors->end();++i) {    vtol_face *f = (*i)->cast_to_face();    if(f){      if(f->cast_to_face_2d()){        // deterimine the direction of the edge        if(tc->direction(*f) ==1){          strm << " <vxl_face_2d_sptr id=\"f2d" << (*i)->get_id() << "\"/>" << vcl_endl;        }        else{          // we have to signal a flip          strm << " <vxl_face_2d_sptr id=\"f2d" << (*i)->get_id() << "\"/><vxl_flip/>"  << vcl_endl;        }      }    }  }  strm << "</vxl_two_chain_2d>" << vcl_endl;}void vtol_topology_io::write_block(vtol_block *b, vcl_ostream &strm){  strm << "<vxl_block_2d id=\"b__" << b->get_id() << "\">" << vcl_endl;  // write the inferiors  const vcl_vector<vtol_topology_object_sptr> *inferiors = b->inferiors();  vcl_vector<vtol_topology_object_sptr>::const_iterator i;  for(i=inferiors->begin();i!=inferiors->end();++i) {    strm << " <vxl_two_chain_2d_sptr id=\"tc_" << (*i)->get_id() << "\"/>" << vcl_endl;  }  strm << "</vxl_block_2d>" << vcl_endl;}void vtol_topology_io::write_dtd(vcl_ostream &strm){  #include <vtol/vtol_dtd.cxx>  strm << vtol_dtd << vcl_endl;}