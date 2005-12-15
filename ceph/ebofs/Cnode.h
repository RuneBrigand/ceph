#ifndef __EBOFS_CNODE_H
#define __EBOFS_CNODE_H

#include "Onode.h"

/*
 * collection node
 *
 * holds attribute metadata for collections.
 * colletion membership is stored in b+tree tables, independent of tte cnode.
 */

class Cnode : public LRUObject
{
 private:
  int ref;
  bool dirty;

 public:
  coll_t coll_id;
  Extent cnode_loc;

  map<string,AttrVal> attr;

 public:
  Cnode(coll_t cid) : ref(0), dirty(false), coll_id(cid) {
	cnode_loc.length = 0;
  }
  ~Cnode() {
  }

  block_t get_cnode_id() { return cnode_loc.start; }
  int get_cnode_len() { return cnode_loc.length; }

  void get() {
	if (ref == 0) lru_pin();
	ref++;
  }
  void put() {
	ref--;
	if (ref == 0) lru_unpin();
  }

  void mark_dirty() {
	if (!dirty) {
	  dirty = true;
	  get();
	}
  }
  void mark_clean() {
	if (dirty) {
	  dirty = false;
	  put();
	}
  }
  bool is_dirty() { return dirty; }


  int get_attr_bytes() {
	int s = 0;
	for (map<string, AttrVal >::iterator i = attr.begin();
		 i != attr.end();
		 i++) {
	  s += i->first.length() + 1;
	  s += i->second.len + sizeof(int);
	}
	return s;
  }
  
  //
  //???void clear();

  
};

#endif
