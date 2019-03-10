/* btree.cc
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#include <stdexcept>

#include "btree.hh"

using namespace mt;

btree::btree(const std::string&	fname):
	off_(0,false),
	is_(fname.c_str(),std::ios_base::in|std::ios_base::binary)
{

	if (!is_) throw std::runtime_error("unable to open "+fname);
	//read database header
	is_.read(reinterpret_cast<char*>(&header_),sizeof(db_header));
	if (header_.identifier_ != sysident)
		throw std::runtime_error("invalid database identifier");

	//auto corrections
	if (!header_.level_size_) header_.level_size_ = 1024;
	if (header_.max_records_ < 0) header_.max_records_ = 0;

	records_.data().resize(header_.level_size_);//prepare page buffer;
}

btree::~btree()
{}

bool btree::find(mem_pos key,size_t keysize)
{
	return find_if<bgreater,beq>(bgreater(key, keysize),beq(key,keysize));
}

void btree::load_page(size_t page_num)
{
	is_.seekg(static_cast<std::streamoff>(header_.level_size_*page_num));
	is_ >> records_;
}

const bdata_pair& btree::get()
{
	return *off_;
}
