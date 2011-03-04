//:
// \file
// \brief Finds connected regions in a boolean image.
// \author Ian Scott

#include "vil_blob.h"
#include <vcl_algorithm.h>



// Produce a label image that enumerates all disjoint blobs in a binary image
void vil_blob_labels(const vil_image_view<bool>& src_binary,
                     vil_blob_connectivity conn,
                     vil_image_view<unsigned>& dest_label)
{
  unsigned ni=src_binary.ni();
  unsigned nj=src_binary.nj();
  dest_label.set_size(ni, nj);
  dest_label.fill(0);

  vcl_vector<unsigned> renumbering(1u, 0u); // renumber 0->0
  vcl_vector<unsigned> neighbouring_labels;

  unsigned n_prev_neighbours;
  switch (conn)
  {
   case vil_blob_4_conn:
    n_prev_neighbours=2;
    break;
   case vil_blob_8_conn:
    n_prev_neighbours=4;
    break;
   default:
    n_prev_neighbours=0; assert(!"unknown connectivity");
  }

  // The 2-prev-(of 6)-neighbourhood are the first two entries.
  // The 4-prev-(of 8)-neighbourhood are those two plus the rest.
  int neighbourhood_ii[] = { -1,  0, -1, +1};
  int neighbourhood_jj[] = {  0, -1, -1, -1};

  typedef vcl_vector<unsigned>::iterator ITER;

  for (unsigned j=0; j<nj; ++j)
    for (unsigned i=0; i<ni; ++i)
    {
      if (!src_binary(i,j)) continue;
      neighbouring_labels.clear();
      for (unsigned l=0; l<n_prev_neighbours; ++l)
      {
        unsigned ii = i + neighbourhood_ii[l];
        if (ii >= ni) continue; // rely on wraparound to find -ne overruns.
        unsigned jj = j + neighbourhood_jj[l];
        if (jj >= nj) continue;
        unsigned d = dest_label(ii,jj);
        if (d!=0) neighbouring_labels.push_back(d);
      }
      if (neighbouring_labels.empty())
      {
        unsigned new_label = renumbering.size();
        dest_label(i,j) = new_label;
        renumbering.push_back(new_label);
      }
      else
      {
        // See how many unique labels neighbouring labels we have
        vcl_sort(neighbouring_labels.begin(), neighbouring_labels.end());
        ITER end = vcl_unique(neighbouring_labels.begin(), neighbouring_labels.end());
        // don't bother erasing unique's suffix, just keeping the end iterator
        // will be enough.
        ITER it=neighbouring_labels.begin();
        unsigned label = *it++;
        dest_label(i,j) = label;

        // If we have neighbours with multiple labels.
        //   then record merges of the previously disjointly labelled blocks.
        // If there was only a single unique label, the following for loop
        //   will not execute.
        for (; it!=end; ++it)
          renumbering[*it] = vcl_min(renumbering[*it], label);
      }
    }

  // Propagate the renumbering so that when 3 points to 2 and 2 points to 1,
  //   3 should then point to 1.
  for (ITER it=renumbering.begin(), end=renumbering.end(); it!=end; ++it)
    *it = renumbering[*it];

  // Now due to the renumbering, the set of labels may not compactly occupy
  // the number line. So renumber the renumbering array.
  vcl_vector<unsigned> labels(renumbering.begin(), renumbering.end());
  vcl_sort(labels.begin(), labels.end());
  labels.erase(vcl_unique(labels.begin(), labels.end()), labels.end());
  const unsigned dodgy = static_cast<unsigned>(-1);
  vcl_vector<unsigned> renum_renum(renumbering.size(), dodgy);
  renum_renum[0]=0;
  for (unsigned l=0, n=labels.size(); l<n; ++l)
    renum_renum[labels[l]] = l;

  for (ITER it=renumbering.begin(), end=renumbering.end(); it!=end; ++it)
    *it=renum_renum[*it];

  // Check than no DODGY values got into the renumbering.
  assert(vcl_find(renumbering.begin(), renumbering.end(), dodgy)
    == renumbering.end() );

  // Renumber the labels, to merge connected blobs, with a compact set of labels.

  for (unsigned j=0; j<nj; ++j)
    for (unsigned i=0; i<ni; ++i)
      dest_label(i,j) = renumbering[dest_label(i,j)];
}


//: Produce a list of regions label image that enumerates all disjoint blobs in a binary image.
// A blob label value of n will be returned in dest_regions[n-1].
void vil_blob_labels_to_regions(const vil_image_view<unsigned>& src_label,
                                vcl_vector<vil_blob_region>& dest_regions)
{
  dest_regions.clear();
  unsigned ni=src_label.ni();
  unsigned nj=src_label.nj();

  for (unsigned j=0; j<nj; ++j)
    for (unsigned i=0; i<ni;) // don't auto increment i, since the loop body does it.
    {
      unsigned label = src_label(i,j);
      if (!label)
      { // if not a label - go to next pixel.
        ++i;
        continue;
      }
      // Make sure there is a region for this label.
      if (label > dest_regions.size()) dest_regions.resize(label);
      unsigned i_start=i;
      // Find end of chord.
      while (++i < ni && src_label(i,j)==label);
      dest_regions[label-1].push_back(vil_chord(i_start, i-1, j));
    }
}