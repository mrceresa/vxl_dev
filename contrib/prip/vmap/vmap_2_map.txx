// This is prip/vmap/vmap_2_map.txx
#ifndef vmap_2_map_txx_
#define vmap_2_map_txx_

#include "vmap_2_map.h"


template <class D>
vmap_2_map<D>::vmap_2_map(const self_type &right)
{
    operator=(right) ;
}

template <class D>
vmap_2_map<D>::~vmap_2_map()
{
  clear() ;
}


template <class D>
vmap_2_map<D> & vmap_2_map<D>::operator=(const self_type &right)
{
    if (&right!=this)
    {
        dart_sequence::operator=(right) ;
        for (vmap_dart_index i=0; i<nb_darts(); i++)
        {
            set_sigma(i,right.sigma(i)) ;
            set_alpha(i,right.alpha(i)) ;
        }
    }
    return *this ;
}

template <class D>
template <class M>
void vmap_2_map<D>::set_structure(const M &right)
{
    if ((const self_type *)&right!=this)
    {
        initialise_darts(right.nb_darts()) ;
        for (vmap_dart_index i=0; i<nb_darts(); i++)
        {
            set_sigma(i,right.sigma(i)) ;
            set_alpha(i,right.alpha(i)) ;
        }
    }
}

template <class D>
void vmap_2_map<D>::initialise_darts(int arg)
{
    //get_dart_pointer.resize(arg) ;
    dart_sequence::resize(arg) ;
    for (int i=1; i<arg; i+=2)
    {
      get_dart_pointer(i)->set_alpha(get_dart_pointer(i-1)) ;
      //get_dart_pointer[i]->set_alpha(&get_dart_pointer[i-1]) ;
    }
}


template <class D>
bool vmap_2_map<D>::valid() const
{
    for (int i=0;i<nb_darts();++i)
    {
        if (vmap_2_map_alpha(get_dart_pointer(i))==NULL)
          return false ;
        if (vmap_2_map_sigma(get_dart_pointer(i))==NULL)
          return false ;
        if (vmap_2_map_isigma(get_dart_pointer(i))==NULL)
          return false ;
    }
    return true ;
}

template <class D>
void vmap_2_map<D>::read_structure(vcl_istream & stream)
{
    int tmp;
    stream>>tmp;
    initialise_darts(tmp) ;
    for (int i=0; i<nb_darts(); i++)
    {
        stream>>tmp;
        set_sigma(i,tmp) ;
    }
}

template <class D>
void vmap_2_map<D>::write_structure(vcl_ostream & stream) const
{
    stream<<nb_darts()<<vcl_endl ;
    for (int i=0; i<nb_darts(); i++)
    {
        stream<<sigma(i)<<"   " ;
    }
    stream<<vcl_endl ;
}


template <class D>
void vmap_2_map<D>::setDualStructure()
{
    vcl_vector<int> tmp_phi ;
    int nbd= nb_darts() ;
    tmp_phi.resize(nb_darts()) ;
    int i;
    for (i=0;i<nbd; ++i)
    {
        tmp_phi[i]=phi(i);
    }
    clear() ;
    initialise_darts(nbd) ;
    for (i=0; i<nbd; i++)
    {
        set_sigma(i,tmp_phi[i]) ;
    }
}

template <class D>
void vmap_2_map<D>::clear()
{
    dart_sequence::clear() ;
}

template <class D>
void vmap_2_map<D>::set_sigma (vmap_dart_index arg, vmap_dart_index arg_sigma)
{
    dart(arg).set_sigma(get_dart_pointer(arg_sigma)) ;
}

template <class D>
void vmap_2_map<D>::set_alpha (vmap_dart_index arg, vmap_dart_index arg_alpha)
{
    dart(arg).set_alpha(get_dart_pointer(arg_alpha)) ;
}

template <class D>
void vmap_2_map<D>::set_phi (vmap_dart_index arg, vmap_dart_index arg_phi)
{
    dart(arg).set_phi(get_dart_pointer(arg_phi)) ;
}

template <class D>
void vmap_2_map<D>::unchecked_removal(dart_iterator & arg)
{
   dart_type* d=&*arg,
        sd=vmap_2_map_sigma(d),
        a=vmap_2_map_alpha(d),
        sa=vmap_2_map_sigma(aa) ;

  if (sd != d || sa != a) // ! Pendant edges
  {
    if (sd == a) // self direct loop
    {
      vmap_2_map_isigma(d)->set_sigma(sa);
      arg.alpha().sigma() ;
    }
    else
    if (sa == d) // self direct loop
    {
        vmap_2_map_isigma(a)->set_sigma(sd);
        arg.sigma() ;
    }
    else
    {
        // General case
        vmap_2_map_isigma(d)->set_sigma(sd);
        arg.sigma() ;
        vmap_2_map_isigma(a)->set_sigma(sa);
    }
  }
  // remove d and a
  suppress_from_sequence(d) ;
  suppress_from_sequence(a) ;
}

template <class D>
void vmap_2_map<D>::unchecked_contraction(dart_iterator & arg)
{
  dart_type* d=&*arg,
        sd=vmap_2_map_sigma(d),
        a=vmap_2_map_alpha(d),
        sa=vmap_2_map_sigma(aa) ;

  if (sa != d || sa != a) // ! Loop
  {
    if (sa == a) // pendant edge
    {
      vmap_2_map_isigma(d)->set_sigma(sd);
      arg.sigma() ;
    }
    else
    if (sd == d) // pendant edge
    {
      vmap_2_map_isigma(a)->set_sigma(sa);
      arg.alpha().sigma();
    }
    else
    {
        // General case
        vmap_2_map_isigma(d)->set_sigma(sa);
        vmap_2_map_isigma(a)->set_sigma(sd);
        arg.alpha().sigma();
    }
  }
  // remove d and a
  suppress_from_sequence(d) ;
  suppress_from_sequence(a) ;
}

template <class D>
void vmap_2_map<D>::suppress_from_sequence(dart_type * d)
{
  int i=sequence_index(*d) ;
  dart_sequence::swap(i,nb_darts()-1) ;
  //vcl_swap(get_dart_pointer(i),get_dart_pointer.last()) ;
  //get_dart_pointer(i)->set_sequence_index(i) ;
  //delete_dart(d);
  //get_dart_pointer.pop_back();
    destroyLastDart() ;
}

template <class D>
void vmap_2_map<D>::contraction(const contraction_kernel &arg_kernel)
{
  int ld=get_dart_pointer.size(), id ;

  typename contraction_kernel::const_iterator itk ;
  for (itk=arg_kernel.begin(); itk!=arg_kernel.end(); ++itk)
  {
    //((vmap_2_pyramid_base_dart&)(**itk)).set_last_levmap_2_pyramid_base_dart&)_level.last().
    dart_type * d =get_dart_pointer((*itk)->sequence_index()) ;
    id=d->sequence_index() ;
    --ld ;
    dart_sequence::swap(id,ld) ;

    id=vmap_2_map_alpha(d)->sequence_index() ;
    --ld ;
    dart_sequence::swap(id,ld) ;
  }

  for (id=0; id<ld ; ++id )
  {
    vmap_2_map_dart* d=(vmap_2_map_dart*)get_dart_pointer(id) ;
    vmap_2_map_dart* sd=vmap_2_map_sigma(d) ;
    if (sd->sequence_index()>=ld)
    {
      vmap_2_map_dart* ssd=sd ;
      while (ssd->sequence_index()>=ld)
      {
        ssd=vmap_2_map_phi(ssd) ;
      }
      d->set_sigma(ssd) ;
    }
  }

  ld=get_dart_pointer.size()-ld ;
  for (id=0; id<ld ; ++id )
  {
    //delete_dart(get_dart_pointer.last()) ;
    //get_dart_pointer.pop_back() ;
     destroyLastDart() ;
  }
}

template <class D>
void vmap_2_map<D>::removal(const removal_kernel &arg_kernel)
{
  int ld=get_dart_pointer.size(), id ;

  typename contraction_kernel::const_iterator itk ;
  for (itk=arg_kernel.begin(); itk!=arg_kernel.end(); ++itk)
  {
    //((vmap_2_pyramid_base_dart&)(**itk)).set_last_levmap_2_pyramid_base_dart&)_level.last().
    dart_type * d =get_dart_pointer((*itk)->sequence_index()) ;
    id=d->sequence_index() ;
    --ld ;
    dart_sequence::swap(id,ld) ;

    id=vmap_2_map_alpha(d)->sequence_index() ;
    --ld ;
    dart_sequence::swap(id,ld) ;
  }

  for (id=0; id<ld ; ++id )
  {
    vmap_2_map_dart* d=(vmap_2_map_dart*)get_dart_pointer(id) ;
    vmap_2_map_dart* sd=vmap_2_map_sigma(d) ;
    if (sd->sequence_index()>=ld)
    {
      vmap_2_map_dart* ssd=sd ;
      while (ssd->sequence_index()>=ld)
      {
        ssd=vmap_2_map_sigma(ssd) ;
      }
      d->set_sigma(ssd) ;
    }
  }

  ld=get_dart_pointer.size()-ld ;
  for (id=0; id<ld ; ++id )
  {
    //delete_dart(get_dart_pointer.last()) ;
    //get_dart_pointer.pop_back() ;
        destroyLastDart() ;
  }
}

template <class D>
vmap_2_map_tag vmap_2_map<D>::tag ;

#if 0
template <class V, class E, class F, class D>
void  vmap_2_map< V,E,F,D >::set_edge(vmap_edge_index arg,
              vmap_edge_index arg_edge1, vmap_vertex_index arg_vertex1, vmap_face_index arg_face1,
              vmap_edge_index arg_edge2, vmap_vertex_index arg_vertex2, vmap_face_index arg_face2)
{
    vmap_dart_index tmp1 = edge_first_dart(arg_edge1),
             tmp2 = edge_first_dart(arg_edge2),
             tmp12 = edge_first_dart(arg),
             tmp22 = alpha(tmp12);
    if (dart_associated_vertex(tmp1)!=arg_vertex1)
       tmp1=alpha(tmp1) ;

  if (dart_associated_vertex(tmp2)!=arg_vertex2)
       tmp1=alpha(tmp2) ;

    set_dart(tmp12,tmp1, arg_vertex1, arg_face1) ;
    set_dart(tmp22,tmp2, arg_vertex2, arg_face2) ;
}
#endif // 0

#endif // vmap_2_map_txx_
