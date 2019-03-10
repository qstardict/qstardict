/* shared_ptr.hh
 * This file is part of libmtsupport library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtsupport as the file COPYING.
 */

#ifndef MT_SHARED_PTR_HH__
#define MT_SHARED_PTR_HH__


namespace mt
{
	struct counter
	{ /**< object usage counter for shared objects */
		counter() : usage_(new int(1)) {}
		counter(const counter& other): usage_(other.usage_) { ++*usage_; }
		~counter() { if (0 == --*usage_) delete usage_; }
		void swap(counter& other) { std::swap(usage_, other.usage_); }
		int *usage_;
	};

    template <typename T>
	struct shared_ptr
	{ /**< yet another implementation of the shared pointer */
		shared_ptr(T* ptr = 0):
				counter_(),
				ptr_(ptr)
		{}
		~shared_ptr()
		{
			if (1 == *counter_.usage_) delete ptr_;
		}
		shared_ptr(const shared_ptr<T>& other):
				counter_(other.counter_),
				ptr_(other.ptr_)
		{}
		shared_ptr& operator=(const shared_ptr<T>& other)
		{
			shared_ptr(other).swap(*this);
			return *this;
		}
		T *operator->() const
		{
			return ptr_;
		}
		T *get() const
		{
			return ptr_;
		}
		void swap(shared_ptr<T>& other)
		{
			std::swap(ptr_, other.ptr_);
			counter_.swap(other.counter_);
		}
		counter counter_;
		T *ptr_;
	};
}

#endif
