/* btraits.hh
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#ifndef MT_BTRAITS_HH__
#define MT_BTRAITS_HH__

#include "bdata.hh"

namespace mt
{
    template <typename T> struct item_traits {};

    template<>
    struct item_traits<std::string>
    {
	static mem_pos data(const std::string& str)
	{
	    return reinterpret_cast<mem_pos>(str.c_str());
	}
	static size_t length(const std::string& str)
	{
	    return str.length();
	}
	static std::string def_value() { return ""; }
	static void assign(std::string& to,mem_pos val,size_t len)
	{
	    to.assign(reinterpret_cast<const char*>(val),
		      reinterpret_cast<const char*>(val) + len);
	}
    };

    template<>
    struct item_traits<i16_t>
    {
	static const mem_pos data(const i16_t& val)
	{
	    return reinterpret_cast<mem_pos>(&val);
	}
	static size_t length(const i16_t&)
	{
	    return sizeof(i16_t);
	}
	static i16_t def_value() { return 0; }
	static void assign(i16_t& to,mem_pos val,size_t)
	{
	    to = *reinterpret_cast<const i16_t *>(val);
	}
    };

    template<>
    struct item_traits<mem_vector>
    {
	static mem_pos data(const mem_vector& vec) { return &vec[0]; }
	static size_t length(const mem_vector& vec) { return vec.size(); }
	static mem_vector def_value() { return mem_vector(); }
	static void assign(mem_vector& to,mem_pos val,size_t len) { to.assign(val,val+len); }
    };
}

#endif
