/* bdata.hh
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#ifndef MT_BDATA_HH__
#define MT_BDATA_HH__

#include <utility>
#include <vector>

namespace mt
{
	typedef const unsigned char* mem_pos; /**< position in the mmaped memory */
	typedef std::vector<unsigned char> mem_vector; /**< vector of void data */
	
	//16bit and 32 bit types: we had to use 16 bit values,'cause multitran has a long history
	typedef	short	i16_t;
	typedef	unsigned short	ui16_t;
	typedef	int	i32_t;
	typedef	unsigned int	ui32_t;

	struct bdata
	{ /**< one set of the data in multitran's database */
		mem_pos data_; /**< pointer to the data */
		size_t len_; /**< length of the data */
	};

	typedef std::pair<bdata, bdata> bdata_pair; /**< pair of the key and value */
}

#endif
