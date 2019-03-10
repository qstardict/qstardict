/* brecset.hh
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#ifndef MT_BRECSET_H__
#define MT_BRECSET_H__

#include <vector>
#include <iostream>

#include "biterator.hh"
#include "btree_dll.hh"

namespace mt
{
	class brecset
	{ /**< current page with set of records, we made linear search on records*/
		public:
		LIBBTREE_API brecset() {}
		LIBBTREE_API ~brecset() {}

		LIBBTREE_API biterator begin(); /**< @return first iterator */
		LIBBTREE_API biterator end(); /**< @return last iterator */

		LIBBTREE_API size_t size(); /**< @return size of the page */
		LIBBTREE_API size_t next_page(); /**< @return next page number */

		LIBBTREE_API mem_vector& data() { return data_; }
		private:
			mem_vector	data_;
	};
	
	inline
	std::istream& operator>>(std::istream& is,brecset& rec)
	{
		is.read(reinterpret_cast<char *>(&rec.data()[0]),
			static_cast<std::streamsize>(rec.data().size()));
		return is;
	}
}

#endif
