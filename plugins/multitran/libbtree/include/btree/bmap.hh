/* bmap.hh
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#ifndef MT_BMAP_HH__
#define MT_BMAP_HH__

#include "btree.hh"
#include "btraits.hh"

namespace mt
{
    template <typename KeyT,typename ValueT,
              typename KTraitsT = item_traits<KeyT>,
	      typename VTraitsT = item_traits<ValueT> >
    class bmap: public btree
    {
	typedef KTraitsT key_traits;
	typedef VTraitsT value_traits;
	public:
	bmap(const std::string& fname): btree(fname)
	{}
	~bmap()
	{}
	
	ValueT get(const KeyT& key)
	{
	    ValueT value = value_traits::def_value();
	    if (find(key_traits::data(key),key_traits::length(key)))
		value_traits::assign(value,off_->second.data_,off_->second.len_);
	    return value;
	}
    };

}

#endif
