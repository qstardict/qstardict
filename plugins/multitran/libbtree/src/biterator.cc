/* biterator.cc
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */
#include <iostream>

#include "biterator.hh"

using namespace mt;

biterator::biterator(mem_pos pos, bool fill):
		pos_(pos),
		shift_(0)
{
	if (pos_ && fill) fill_pair(); //if position is not zero fill pair with the some values
}

biterator& biterator::operator++() /**< prefix operator */
{
	shift_ = keylen_ + valuelen_ + 2;
	pos_ += shift_; //go to the next record
	fill_pair(); //save all values in temporary place
	return *this;
}

biterator& biterator::rollback() /**< prefix operator */
{
	pos_ -= shift_ ; //go to the next record
	shift_ = 0;
	fill_pair(); //save all values in temporary place
	return *this;
}

biterator biterator::operator++(int)
{
	biterator tmp(*this);
	operator++();
	return tmp;
}

bool biterator::equal(const biterator& other) const
{
	return (pos_ == other.pos_);
}

const bdata_pair& biterator::operator*() const
{
	return pair_;
}

const bdata_pair* biterator::operator->() const
{
	return &(operator*());
}

void biterator::fill_pair()
{
	keylen_ = pos_[0]; //first byte - key
	valuelen_ = pos_[1]; //second byte - value

	pair_.first.len_ = keylen_;
	pair_.first.data_ = pos_ + 2;

	pair_.second.len_ = valuelen_;
	pair_.second.data_ = pos_ + 2 + keylen_;
}
