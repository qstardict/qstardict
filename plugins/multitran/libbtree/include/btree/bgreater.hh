/* bgreater.hh
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#ifndef BGREATER_HH__
#define BGREATER_HH__

#include <cstring>

namespace mt
{

    ///////////////////////equal
    
    inline
    bool eq(mem_pos k1,size_t l1,mem_pos k2,size_t l2)
    {
	return (l1 == l2) && !memcmp(k1,k2,l1);
    }

    
    struct beq
    {
	typedef bdata_pair argument_type;
	
	beq(mem_pos key,size_t len):
	    key_(key),
	    len_(len)
	{}
	bool operator()(const bdata_pair& item)
	{
	    return eq(item.first.data_,item.first.len_,key_,len_);
	}
	mem_pos key_;
	size_t len_;
    };
    
    //////////////////////greater

    inline 
    bool gr(mem_pos k1,size_t l1,mem_pos k2,size_t l2)
    {
	const int res = memcmp(k1,k2,std::min(l1,l2));
	return (res)? res > 0: l1 > l2;
    }

    struct bgreater
    {
	typedef bdata_pair argument_type;
    
	bgreater(mem_pos key,size_t len):
	    key_(key),
	    len_(len)
	{}

	//return true if record is greater then key to find
        bool operator()(const bdata_pair& item)
	{
	    return gr(item.first.data_,item.first.len_,key_,len_);
	}

	mem_pos key_;
	size_t	len_;
    };
    
    ////// or - composition of the functors

    template <typename F1,typename F2>
    struct or_impl
    {
	or_impl(F1 f1,F2 f2): f1_(f1), f2_(f2) {}
	
	bool operator()(const bdata_pair& item)
	{
	    return f1_(item) || f2_(item);
	}
	F1 f1_;
	F2 f2_;
    };
    
    template <typename F1,typename F2>
	inline
    or_impl<F1,F2> or_(F1 f1,F2 f2)
    {
	return or_impl<F1,F2>(f1,f2);
    }
    
    ///// and - composition of the functor
    template <typename F1,typename F2>
    struct and_impl
    {
	and_impl(F1 f1,F2 f2): f1_(f1), f2_(f2) {}
	
	bool operator()(const bdata_pair& item)
	{
	    return f1_(item) && f2_(item);
	}
	F1 f1_;
	F2 f2_;
    };
    
    template <typename F1,typename F2>
	inline
    and_impl<F1,F2> and_(F1 f1,F2 f2)
    {
	return and_impl<F1,F2>(f1,f2);
    }

    ///// and - composition of the functor
    template <typename F>
    struct not_impl
    {
	not_impl(F f): f_(f) {}
	
	bool operator()(const bdata_pair& item)
	{
	    return !f_(item);
	}
	F f_;
    };
    
    template <typename F>
	inline
    not_impl<F> not_(F f)
    {
	return not_impl<F>(f);
    }
}

#endif
