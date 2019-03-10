/* biterator.hh
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#ifndef MT_BTREE_ITERATOR_H__
#define MT_BTREE_ITERATOR_H__

#include <iterator>

#include "bdata.hh"
#include "btree_dll.hh"

namespace mt
{
	class biterator: public std::iterator<std::forward_iterator_tag, bdata_pair>
	{ /**< forward walking database page iterator */
	public:
		LIBBTREE_API biterator(mem_pos pos = 0, bool fill = true);  /**< start iterator with appropriate offset in the database file*/
		LIBBTREE_API ~biterator() {}
		LIBBTREE_API biterator& operator++(); /**< prefix operator */
		LIBBTREE_API biterator operator++(int);

		LIBBTREE_API const bdata_pair& operator*() const;
		LIBBTREE_API const bdata_pair* operator->() const;

		LIBBTREE_API bool equal(const biterator& other) const;

		biterator& rollback(); /**< go one step back */


	private:
		void fill_pair(); /**< fill internal storage with current data */

		mem_pos pos_; /**< current position in the file before database record */
		size_t keylen_; /**< length of the current key */
		size_t valuelen_; /**< length of the current value */
		size_t shift_; /**< rollback offset */

		bdata_pair pair_; /**< temporary internal storage of current data pair */
	};

	inline bool operator==(const biterator& lhs, const biterator& rhs)
	{
		return lhs.equal(rhs);
	}

	inline bool operator!=(const biterator& lhs, const biterator& rhs)
	{
		return !(lhs.equal(rhs));
	}
}

#endif
