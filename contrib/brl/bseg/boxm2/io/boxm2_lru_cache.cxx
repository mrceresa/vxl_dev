#include "boxm2_lru_cache.h"
//:
// \file
#include <boxm2/boxm2_block_metadata.h>
#include <vcl_sstream.h>

//: constructor, set the directory path
boxm2_lru_cache::boxm2_lru_cache(boxm2_scene* scene) : boxm2_cache(scene)
{
  scene_dir_ = scene->data_path();
}

//: destructor flushes the memory for currently ongoing asynchronous requests
boxm2_lru_cache::~boxm2_lru_cache()
{
  //DELETE items in both maps
}

//: realization of abstract "get_block(block_id)"
boxm2_block* boxm2_lru_cache::get_block(boxm2_block_id id)
{
  //then look for the block you're requesting
  if ( cached_blocks_.find(id) != cached_blocks_.end() )
  {
#ifdef DEBUG
    vcl_cout<<"CACHE HIT!"<<vcl_endl;
#endif
    return cached_blocks_[id];
  }

#ifdef DEBUG
  vcl_cout<<"Cache miss :("<<vcl_endl;
#endif
  //otherwise load it from disk with blocking and update cache
  boxm2_block* loaded = boxm2_sio_mgr::load_block(scene_dir_, id);

  //if the block is null then initialize an empty one
  if (!loaded && scene_->block_exists(id)) {
    vcl_cout<<"boxm2_lru_cache::initializing empty block "<<id<<vcl_endl;
    boxm2_block_metadata data = scene_->get_block_metadata(id);
    loaded = new boxm2_block(data);
  }

  //update cache before returning the block
  cached_blocks_[id] = loaded;
  return loaded;
}


//: get data by type and id
boxm2_data_base* boxm2_lru_cache::get_data_base(boxm2_block_id id, vcl_string type, vcl_size_t num_bytes)
{
  //grab a reference to the map of cached_data_
  vcl_map<boxm2_block_id, boxm2_data_base*>& data_map =
    this->cached_data_map(type);

  //then look for the block you're requesting
  if ( data_map.find(id) != data_map.end() )
  {
    //congrats you've found the data block in cache, update cache and return block
#ifdef DEBUG
    vcl_cout<<"DATA CACHE HIT! for "<<type<<vcl_endl;
#endif
    return data_map[id];
  }

  //otherwise it's a miss, load sync from disk, update cache
#ifdef DEBUG
  vcl_cout<<"Cache miss :( for "<<type<<vcl_endl;
#endif
  boxm2_data_base* loaded = boxm2_sio_mgr::load_block_data_generic(scene_dir_, id, type);
  if (!loaded && scene_->block_exists(id)) {
    vcl_cout<<"boxm2_lru_cache::initializing empty data "<<id<<" type: "<<type<<vcl_endl;
    boxm2_block_metadata data = scene_->get_block_metadata(id);
    if (num_bytes > 0 && !data.random_)
      loaded = new boxm2_data_base(new char[num_bytes], num_bytes, id);
    else
      loaded = new boxm2_data_base(data, type);
  }

  //update data map
  data_map[id] = loaded;
  return loaded;
}

//: removes data from this cache (may or may not write to disk first)
void boxm2_lru_cache::remove_data_base(boxm2_block_id id, vcl_string type)
{
  //grab a reference to the map of cached_data_
  vcl_map<boxm2_block_id, boxm2_data_base*>& data_map =
    this->cached_data_map(type);

  //then look for the block you're requesting
  if ( data_map.find(id) != data_map.end() )
  {
    //found the block, now delete it
    delete data_map[id];
    data_map.erase(id);
  }
}

//: helper method returns a reference to correct data map (ensures one exists)
vcl_map<boxm2_block_id, boxm2_data_base*>& boxm2_lru_cache::cached_data_map(vcl_string prefix)
{
  // if map for this particular data type doesn't exist, initialize it
  if ( cached_data_.find(prefix) == cached_data_.end() )
  {
    vcl_map<boxm2_block_id, boxm2_data_base*> dmap;
    cached_data_[prefix] = dmap;
  }

  //grab a reference to the map of cached_data_ and return it
  vcl_map<boxm2_block_id, boxm2_data_base*>& data_map = cached_data_[prefix];
  return data_map;
}

//: helper method says whether or not block id is valid
bool boxm2_lru_cache::is_valid_id(boxm2_block_id id)
{
  //use scene here to determine if this id is valid
  return scene_->block_exists(id);
}


//: Summarizes this cache's data
vcl_string boxm2_lru_cache::to_string()
{
  vcl_stringstream stream;
  stream << "boxm2_lru_cache:: scene dir="<<scene_dir_<<'\n'
         << "  blocks: ";
  vcl_map<boxm2_block_id, boxm2_block*>::iterator blk_iter;
  for (blk_iter = cached_blocks_.begin(); blk_iter != cached_blocks_.end(); ++blk_iter) {
    boxm2_block_id id = blk_iter->first;
    stream << '(' << id /* << ',' << blk_iter->second */ << ")  ";
  }

  vcl_map<vcl_string, vcl_map<boxm2_block_id, boxm2_data_base*> >::iterator dat_iter;
  for (dat_iter = cached_data_.begin(); dat_iter != cached_data_.end(); ++dat_iter)
  {
    vcl_string data_type = dat_iter->first;
    stream<< "\n  data: "<<data_type<<' ';
    vcl_map<boxm2_block_id, boxm2_data_base*> dmap = dat_iter->second;
    vcl_map<boxm2_block_id, boxm2_data_base*>::iterator it;
    for (it = dmap.begin(); it != dmap.end(); ++it)
    {
      boxm2_block_id id = it->first;
      stream<< '(' << id /*<< ',' <<it->second */<< ")  ";
    }
  }
  return stream.str();
}

//: shows elements in cache
vcl_ostream& operator<<(vcl_ostream &s, boxm2_lru_cache& scene)
{
  s << scene.to_string();
  return s;
}

