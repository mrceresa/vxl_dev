/*
  crossge@crd.ge.com
*/
#ifdef __GNUC__
#pragma implementation
#endif
#include "gmvl_connection.h"



gmvl_connection::gmvl_connection( const gmvl_node_ref node1, const gmvl_node_ref node2)
  : n1_( node1), n2_( node2)
{
}

gmvl_connection::~gmvl_connection()
{
}

// input / output

ostream &operator<<( ostream &os, const gmvl_connection &c)
{
  return os << "<Connection n1=" << c.n1_.ptr() << " n2=" << c.n2_.ptr() << ">";
}
