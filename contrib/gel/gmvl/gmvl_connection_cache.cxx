/*
  crossge@crd.ge.com
*/
#ifdef __GNUC__
#pragma implementation "gmvl_connection_cache"
#endif
#include "gmvl_connection_cache.h"

#include <vcl_cassert.h>
#include <vnl/vnl_math.h>

// constructors / destructors

gmvl_connection_cache::gmvl_connection_cache()
{
}

gmvl_connection_cache::~gmvl_connection_cache()
{
}

// simple accessors

void gmvl_connection_cache::add( const gmvl_node_ref node1, const gmvl_node_ref node2)
{
  if( node1.ptr()!= node2.ptr()) 
    { 
      gmvl_connection_ref c= new gmvl_connection( node1, node2);

      connections_.push_back( c);
      
      // and add to the cache
      assert( node1->ref_>= 0);
      assert( node2->ref_>= 0);

      if( node1->ref_>= cache_.size()) cache_.resize( node1->ref_+1);
      if( node2->ref_>= cache_.size()) cache_.resize( node2->ref_+1);

      int biggest= vnl_math_max( node1->ref_, node2->ref_);

      if( biggest>= cachebool_.rows())
	{
	  cachebool_.enlarge( (biggest+1)*2, (biggest+1)*2);
	}

      cache_[node1->ref_].push_back( node2->ref_);
      cache_[node2->ref_].push_back( node1->ref_);

      cachebool_.put( node1->ref_, node2->ref_, true);
      cachebool_.put( node2->ref_, node1->ref_, true);
    }
}


// clever accessors

vcl_vector<int> gmvl_connection_cache::get_connected_nodes( const gmvl_node_ref node1,
							    const gmvl_node_ref node2) const
{
  vcl_vector<int> c= get_connected_nodes( node1);
  vcl_vector<int> d;

  for( int i=0; i< c.size(); i++)
    if( cachebool_(node2->ref_,c[i]))
      d.push_back(c[i]);

  return d;
}

vcl_vector<int> gmvl_connection_cache::get_connected_nodes( const gmvl_node_ref node1,
							    const gmvl_node_ref node2,
							    const gmvl_node_ref node3) const
{
  vcl_vector<int> c= get_connected_nodes( node1);
  vcl_vector<int> d;

  for( int i=0; i< c.size(); i++)
    if( cachebool_(node2->ref_,c[i]) &&
	cachebool_(node3->ref_,c[i]))
      d.push_back(c[i]);

  return d;
}

vcl_vector<int> gmvl_connection_cache::get_connected_nodes( const vcl_vector<gmvl_node_ref> nodes) const
{
  vcl_vector<int> c= get_connected_nodes( nodes[0]);
  vcl_vector<int> d;

  for( int i=0; i< c.size(); i++)
    {
      bool ok= true;

      for( int j=1; (j< nodes.size()) && ok; j++)
	{
	  if( !cachebool_(nodes[j]->ref_,c[i]))
	    ok= false;
	}
      
      if( ok)
	d.push_back(c[i]);
    }

  return d;
}

// house-keeping

void gmvl_connection_cache::rebuild()
{
  cache_.clear();
  assert(false);

  for( int i=0; i< connections_.size(); i++)
    {
      gmvl_node_ref node1= connections_[i]->get_node1();
      gmvl_node_ref node2= connections_[i]->get_node2();

      assert( node1->ref_>= 0);
      assert( node2->ref_>= 0);

      if( node1->ref_>= cache_.size()) cache_.resize( node1->ref_+1);
      if( node2->ref_>= cache_.size()) cache_.resize( node2->ref_+1);

      int biggest= vnl_math_max( node1->ref_, node2->ref_);

      if( biggest>= cachebool_.rows())
	{
	  gbl_bit_array_2d temp( biggest+1, biggest+1, false);

	  for( int ci=0; ci< cachebool_.rows(); ci++) 
	    for( int cj=0; cj< cachebool_.cols(); cj++)
	      temp.put(ci,cj, cachebool_(ci,cj));

	  cachebool_= temp;
	}

      cache_[node1->ref_].push_back( node2->ref_);
      cache_[node2->ref_].push_back( node1->ref_);

      cachebool_.put( node1->ref_, node2->ref_, true);
      cachebool_.put( node2->ref_, node1->ref_, true);
    }
}

// input / output

ostream &operator<<( ostream &os, const gmvl_connection_cache &c)
{
  //  for( int i=0; i< c.connections_.size(); i++)
  //    os << *c.connections_[i];

  for( int i=0; i< c.cache_.size(); i++)
    {
      os << "<" << i << " - ";

      for( int j=0; j< c.cache_[i].size(); j++)
	{
	  os << c.cache_[i][j];
	  if( j!= (c.cache_[i].size()-1))
	    os << ", ";
	}
      
      os << ">";
    }

  return os;
}
