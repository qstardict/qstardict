/* brecset.cc
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#include "brecset.hh"

using namespace mt;

biterator brecset::begin()
{
	return biterator(&data_[0] + 3);
}

biterator brecset::end()
{
	return biterator(&data_[0] + size(),false);//are we need to use size + 1?
}


size_t brecset::size()
{
	return (*reinterpret_cast<const i16_t*>(&data_[0] + 1));
}

size_t brecset::next_page()
{
	return (*reinterpret_cast<const i16_t*>(&data_[0] + size()));
}
