
#include <vtol/vtol_vertex_2d.h>
#include <vtol/vtol_zero_chain_2d.h>
#include <vtol/vtol_edge_2d.h>
#include <vtol/vtol_face_2d.h>
#include <vtol/vtol_macros_2d.h>
#include <vnl/vnl_math.h>
//#include <vtol/some_stubs.h>
#include <vtol/vtol_list_functions_2d.h>

#include <vcl/vcl_cassert.h>
#include <vsol/vsol_point_2d.h>

//:
// vtol_vertex_2d() --
// Default Constructor.
vtol_vertex_2d::vtol_vertex_2d(void)
{
  _point=new vsol_point_2d(0,0);
}

//:
// Constructor from an vsol_point_2d.
vtol_vertex_2d::vtol_vertex_2d(const vsol_point_2d_ref pt)
{
#if 0
  if (!pt) {
    cerr << "Error: Zero pointer passed in to vtol_vertex_2d::vtol_vertex_2d(vsol_point_2d*)\n";
    pt = new vsol_point_2d(0.0 ,0.0);
  }
#endif
  _point=pt;
}

//:
// vtol_vertex_2d (const vtol_vertex_2d &vtol_vertex_2d) --
// Constructor from a vtol_vertex_2d.
vtol_vertex_2d::vtol_vertex_2d(const vtol_vertex_2d &other)
{
  _point=new vsol_point_2d(*other._point);
}

//:
// vtol_vertex_2d (const vector_2d &vect) --
// Constructor from a vector_2d.
vtol_vertex_2d::vtol_vertex_2d (const vector_2d &vect) 
{
  _point=new vsol_point_2d(vect[0],vect[1]);
}

//:
// vtol_vertex_2d (vsol_point_2d &pt)  --
// Constructor from an vsol_point_2d.
vtol_vertex_2d::vtol_vertex_2d (const vsol_point_2d &pt)
{
  _point=new vsol_point_2d(pt);
}

//:
// vtol_vertex_2d(double newx, double newy, double newz) --
// Constructor from three doubles.
vtol_vertex_2d::vtol_vertex_2d(double newx,
                               double newy)
{
  _point=new vsol_point_2d(newx,newy);	
}

//:
// shallow copy with no links 

vtol_topology_object_2d *vtol_vertex_2d::shallow_copy_with_no_links(void)
{
  vtol_vertex_2d *result=new vtol_vertex_2d(*_point);
  return result;
}

// ~vtol_vertex_2d() --
// vtol_vertex_2d Destructor.
vtol_vertex_2d::~vtol_vertex_2d()
{
  //  _point->unref();
}

//---------------------------------------------------------------------------
// Name: clone
// Task: Clone `this': creation of a new object and initialization
//       See Prototype pattern
//---------------------------------------------------------------------------
vsol_spatial_object_2d_ref vtol_vertex_2d::clone(void) const
{
  return new vtol_vertex_2d(*this);
}

/*
 ******************************************************
 *
 *    Accessor Functions
 */

//---------------------------------------------------------------------------
// Name: topology_type
// Task: Return the topology type
//---------------------------------------------------------------------------
vtol_vertex_2d::vtol_topology_object_2d_type
vtol_vertex_2d::topology_type(void) const
{
  return VERTEX;
}

vsol_point_2d_ref vtol_vertex_2d::get_point(void) const
{
  return _point;
}

void vtol_vertex_2d::set_point(vsol_point_2d_ref pt)
{
  _point=pt;
}
//:
// CoolListP<vtol_vertex_2d*>* vtol_vertex_2d::Vertices() --
// Returns a list of Vertices which only contains a pointer to itself.
vertex_list_2d *vtol_vertex_2d::vertices(void)
{
  LIST_SELF_2d(vtol_vertex_2d);
}

//:
// zero_chain_list* vtol_vertex_2d::ZeroChains() --
// Returns a list of ZeroChains that contain the vertex. This is the vertex superiors list.
zero_chain_list_2d *vtol_vertex_2d::zero_chains(void)
{
  SEL_SUP_2d(vtol_zero_chain_2d,zero_chains);
}


//:
// edge_list_2d* vtol_vertex_2d::Edges() --
// Returns a list of Edges which contain the vertex.
edge_list_2d *vtol_vertex_2d::edges(void)
{
  SEL_SUP_2d(vtol_edge_2d,edges);
}

//:
// one_chain_list* vtol_vertex_2d::OneChains() --
// Returns a list of OneChains which contain the vertex.
one_chain_list_2d* vtol_vertex_2d::one_chains(void)
{
  SEL_SUP_2d(vtol_one_chain_2d,one_chains);
}

//:
// face_list_2d* vtol_vertex_2d::Faces() --
// Returns a list of Faces which contain the vertex.
face_list_2d* vtol_vertex_2d::faces(void)
{
  SEL_SUP_2d(vtol_face_2d,faces);
}

//:
// CoolListP<TwoChain*>* vtol_vertex_2d::TwoChains() --
// Returns a list of TwoChains which contain the vertex.
two_chain_list_2d* vtol_vertex_2d::two_chains(void)
{
  SEL_SUP_2d(vtol_two_chain_2d,two_chains);
}

//:
// CoolListP<Block*>* vtol_vertex_2d::Blocks() --
// Returns a list of Blocks which contain the vertex.
block_list_2d* vtol_vertex_2d::blocks(void)
{
  SEL_SUP_2d(vtol_block_2d,blocks);
}

double vtol_vertex_2d::x(void) const
{
  return _point->x();
}

double vtol_vertex_2d::y(void) const
{
  return _point->y();
}

void vtol_vertex_2d::set_x(double val)
{
  _point->set_x(val);
  this->touch(); //Timestamp update
}

void vtol_vertex_2d::set_y(double val)
{
  _point->set_y(val);
  this->touch();
}

vtol_vertex_2d *vtol_vertex_2d::cast_to_vertex(void)
{
  return this;
}
vtol_vertex_2d *vtol_vertex_2d::copy(void)
{
  return new vtol_vertex_2d(*this);// non-deep copy of the vertex.
}
vsol_spatial_object_2d *vtol_vertex_2d::spatial_copy(void)
{
  return this->copy();
}


/*
 ******************************************************
 *
 *    Print Functions
 */
//:
// void vtol_vertex_2d::Print(ostream& strm) --
// This method outputs a simple text representation of the vertex including its
// address in memory.
void vtol_vertex_2d::print(ostream &strm)
{
  strm<<"<vtol_vertex_2d "<<x()<<","<<y()<<","<<(void *)this<<"> with id ";
  strm<<get_id()<<endl;
}


//:
// void vtol_vertex_2d::Describe(ostream& strm, int blanking) --
// This method outputs a detailed description of the vertex including the
// inferiors and superiors.
void vtol_vertex_2d::describe(ostream &strm,
                              int blanking)
{
  print(strm);
  describe_inferiors(strm, blanking);
  describe_superiors(strm, blanking);
}


/*
 ******************************************************
 *
 *    Implementor Functions
 */

//:
// This method returns an implicitline edge created from the vertex and the
// other_vtol_vertex_2d.

vtol_edge_2d *vtol_vertex_2d::new_edge(vtol_vertex_2d *other_vert)
{
  // awf: load vrml sped up by factor of 2 using this loop.

  // Scan Zero Chains
  for (topology_list_2d::const_iterator zp=_superiors.begin();zp!=_superiors.end();++zp)
    {
      // Scan superiors of ZChain (i.e. edges)
      topology_list_2d *sups=(*zp)->get_superiors();
      for (topology_list_2d::iterator ep=sups->begin();ep!=sups->end();++ep) {
	vtol_edge_2d* e=(*ep)->cast_to_edge_2d();
	if (e->get_v1()==other_vert||e->get_v2()==other_vert) {
	  return e;
	}
      }
    }
  return new vtol_edge_2d(this,other_vert);
}

//:
// double vtol_vertex_2d::distance_from(const vector_2d &v)  --
// Returns the squared distance from the vertex and the vector location, v.
double vtol_vertex_2d::distance_from(const vector_2d &v)
{
  //  return _point->distance_from(v);
  cerr << "vtol_vertex_2d::distance_from() not implemented yet\n";
  return -1; // TO DO
}

//:
// double vtol_vertex_2d::EuclideanDistance(vtol_vertex_2d& v)  --
// This method returns the distance, not the squared distance,
// from this vertex and another vertex.
double vtol_vertex_2d::euclidean_distance(vtol_vertex_2d& v)
{
  //  return _point->euclidean_distance(*v.get_point());
  cerr << "vtol_vertex_2d::euclidean_distance() not yet implemented\n";
  return -1; // TO DO
}

//:
// int vtol_vertex_2d::order() --
// This method returns the number of superiors of the vertex.
int vtol_vertex_2d::order(void)
{
  return _superiors.size();
}

//:
// bool vtol_vertex_2d::IsConnected(vtol_vertex_2d* v2) --
// This method returns true if a superior of the vertex contains the vertex,
// v2 as an inferior.
bool vtol_vertex_2d::is_connected(vtol_vertex_2d *v2)
{
  edge_list_2d *vertedges=this->edges();
  for(edge_list_2d::iterator i=vertedges->begin();i!= vertedges->end();++i)
    if((*i)->is_endpoint(v2))
      {
        delete vertedges;
        return true;
      }
  delete vertedges;
  return false;
}

/*
// Vertex* vtol_vertex_2d::CalculateAverageNormal(vector_2d& vertnorm) --
// This method calculates the vertex normal from averaging normals of connected faces.
void vtol_vertex_2d::calculate_average_normal(IUE_vector<double> & vertnorm)
{
  face_list_2d faces;
  vtol_topology_object_2d::faces(faces);

  vector_2d fnormsum((double)0.0, (double)0.0);

for (face_list_2d::iterator i = faces.begin(); 
	       i != faces.end(); ++i)
    {
      vector_2d *cvf= (*i)->get_normal();
      fnormsum += *cvf;
    }

  vertnorm = fnormsum / faces.size();
}
*/
//:
// vtol_vertex_2d* vtol_vertex_2d::Vertex_diff(Vertex& diffvert) --
// This method returns a new Vertex whose location is the vector difference of the vertex and diffvert.
vtol_vertex_2d *vtol_vertex_2d::vertex_diff(vtol_vertex_2d& diffvert)
{
  // return new vtol_vertex_2d((*_point) - (*(diffvert._point)));
  cerr << "vtol_vertex_2d::vertex_diff() not implemented yet\n";
  return 0; // TO DO
}

//:
// This method returns true if the Edge, edg, is on the superior list of the vertex.
bool vtol_vertex_2d::is_endpointp(const vtol_edge_2d &edg)
{
  vcl_vector<vtol_edge_2d*> *edge_list = this->edges();

  if(vcl_find(edge_list->begin(),edge_list->end(),(vtol_edge_2d*)(&edg)) != edge_list->end()){
    delete edge_list;
    return true;
  }
  
  delete edge_list;
  return false;
}


//:
// -- Assign X,Y,Z values only
vtol_vertex_2d &vtol_vertex_2d::operator=(const vtol_vertex_2d &rhs)
{
  
  _point->set_x(rhs.get_point()->x());
  _point->set_y(rhs.get_point()->y());
  
  this->touch();
  return *this;
}


//:
// bool vtol_vertex_2d::eq(vtol_vertex_2d& oVertex)  --
// This method returns true if the vertex is equivalent to the
bool vtol_vertex_2d::eq(vtol_vertex_2d &overtex)
{
  return *this==overtex;
}

bool vtol_vertex_2d::operator==(const vsol_spatial_object_2d &obj) const
{
 
  if ((obj.spatial_type() == vsol_spatial_object_2d::TOPOLOGYOBJECT) &&
      (((vtol_topology_object_2d&)obj).topology_type() == vtol_topology_object_2d::VERTEX))
    return (vtol_vertex_2d &)*this == (vtol_vertex_2d&) (vtol_topology_object_2d&) obj;
  else return false;
 
 
}

//:
// bool vtol_vertex_2d::operator== (vtol_vertex_2d & v2) --
// This method returns true if the vertex is equivalent to v2.
bool vtol_vertex_2d::operator== (const vtol_vertex_2d &v2) const
{
  
  /*
  if (near_eq(x(), v2.x()) &&
      near_eq(y(), v2.y()))
    return true;
  else return false;
  */
  
  if((x() == v2.x()) && (y() == v2.y())){
    return true;
  }
  
  return false;

  
}

//:
// void subst_vertex_on_edge(Vertex* v1, Vertex* v2, Edge* edge)  --
// This function shouldn't be in here....pav January 93.
// It substitutes vertex, v1 for vertex, v2 on Edge, edge.

void subst_vertex_on_edge(vtol_vertex_2d *v1,
                          vtol_vertex_2d *v2,
                          vtol_edge_2d *edge)
{
  vtol_vertex_2d *V1=edge->get_v1();
  vtol_vertex_2d *V2=edge->get_v2();
  if(V1==v2)
    {
      edge->set_v1(v1);
      return;
    }
  if(V2==v2)
    {
      edge->set_v2(v1);
      return;
    }
  cout<<"In vtol_vertex_2d::merge_references(.) Shouldn't happen"<<endl;
}

// vtol_vertex_2d::merge_references(vtol_vertex_2d* v2)  --
// Should not be a method on Vertex...not tested.
void vtol_vertex_2d::merge_references(vtol_vertex_2d *v2)
{
  edge_list_2d *edgelist=v2->edges();
  for (int i=0;i<edgelist->size();i++)
    subst_vertex_on_edge(this,v2,((*edgelist)[i])->cast_to_edge_2d());
}

/*
 ******************************************************
 *
 *    Functions
 */

//:
// Vertex* min_z_vertex(vertex_list_2d* Verts) --
// not implemented.
# if 0 // use in 3d 
// TO DO
vtol_vertex_2d* min_z_vertex(vertex_list_2d* Verts)
{
  vtol_vertex_2d* min_vertex;
  if (Verts->size() > 0) min_vertex = ((*Verts)[0])->cast_to_vertex_2d(); // Get first element.
  else min_vertex = 0;
  vtol_vertex_2d* tVert;
  double min_z = fabs(min_vertex->z());
  double tz;
  for(int i = 1; i < Verts->size();i++) 
    {
      tVert = Verts[i];
      tz = fabs(tVert->z());
      if (tz < min_z) 
	{
	  min_vertex = tVert;
	  min_z = tz;
	}
    }
  return min_vertex;
}

#endif

// vertex_list_2d* merge_vertices_listwise(vertex_list_2d* Verts) --
// not tested....should not be a method.
vertex_list_2d *merge_vertices_listwise(vertex_list_2d *Verts)
{
  vertex_list_2d *newl=new vertex_list_2d();
  vtol_vertex_2d *tVertex;
  vtol_vertex_2d *tVertex2;
  for(int a=0;a<Verts->size();a++) 
    {
      ((*Verts)[a])->unset_tagged_union_flag();
    }
  for(int i=0;i<Verts->size();i++)
    {
      tVertex=(*Verts)[i];
      if(!tVertex->get_tagged_union_flag())
	{
	  for(int k=0;k<Verts->size();k++) 
	    {
	      if(k!=i) 
		{
		  tVertex2=(*Verts)[k];
		  if(tVertex==tVertex2) 
		    {
		      tVertex->merge_references(tVertex2);
		      tVertex2->set_tagged_union_flag();
		    }
		}
	    }
	  newl->push_back(tVertex);
	}
    }
  return newl;
}

//:
// vertex_list_2d* merge_vertices_all(vertex_list_2d* Verts) --
// Should not be a method....not tested.
vertex_list_2d *merge_vertices_all(vertex_list_2d *Verts)
{
  return merge_vertices_listwise(Verts);
}

//:
//-----------------------------------------------------------------------------
//
// -- Determine which other vertices share edges with this. Add any of these which
// are not in the list to it, and recursively call ExploreVertex on them. The
// method is intended to recover all of the vertices in a single topological
// structure which is composed of connected edges.
//
void vtol_vertex_2d::explore_vertex(vertex_list_2d &verts)
{
  edge_list_2d *edges_=edges();

  for (edge_list_2d::iterator i=edges_->begin();i!=edges_->end();++i)
    {
      vtol_vertex_2d *vv;
      vtol_edge_2d *e=*i;
      if(e->get_v1()==this)
        vv=e->get_v1();
      else if(e->get_v2()==this)
        vv=e->get_v2();
      else
        {
          cerr << "Explorevtol_vertex_2d: shouldn't get this\n";
          continue;
        }
      
      if(vcl_find(verts.begin(),verts.end(),vv)!=verts.end())
        {
          verts.push_back(vv);
          vv->explore_vertex(verts);
        }
    }
  
  delete edges_;
}

//:
// -- This method removes the object from the topological structure
//    by unlinking it.  The removal of the object may recursively cause
//    the removal of some of the object's superiors if these superiors
//    are no longer valid.  In addition, inferiors of the object are
//    recursively checked to see if they should be removed.  They
//    are removed if all their superiors have been removed.
//
//    A record of the changes to the topological structure is returned 
//    through the parameters changes and deleted.  For details on what is
//    stored in these parameters, see Edge::Disconnect( changes, deleted ).
//    (RYF 7-16-98)
//
bool vtol_vertex_2d::disconnect(topology_list_2d  &changes,
                                topology_list_2d &deleted)
{

  // Remove this object from its superiors.  Make copy of superiors
  // list first because body of for loop modifies it.
  topology_list_2d *tmp=get_superiors();
  zero_chain_list_2d sup;
      
  for(topology_list_2d::iterator i=tmp->begin();i!=tmp->end();++i)
    sup.push_back((vtol_zero_chain_2d *)(*i));

  for(zero_chain_list_2d::iterator i=sup.begin();i!=sup.end();i++)
      (*i)->remove(this,changes,deleted);

  // Completely unlink this object from the topology structure
  unlink_all_superiors_twoway(this);
  
  return true;
}

//:
// -- For each inferior, this method unlinks the inferior
//    from this object.  If the inferior now has zero superiors,
//    the method is invoked recursively by it.  Finally, this
//    object is pushed onto the list removed.
//    Note: A vertex has no inferiors so this function is
//    much shorter than the corresponding method of the
//    other topological objects.  (RYF 7-16-98)
//
void vtol_vertex_2d::deep_remove(topology_list_2d &removed)
{
  removed.push_back(this);
  return;
}




