/* article.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#include <stdexcept>

#include "article.hh"

using namespace mt;

article::article():
    langin_(english),
    langout_(russian),
    lgk_(0)
{}

article::article(const article& other):
		fields_(other.fields_),
		langin_(other.langin_),
		langout_(other.langout_),
		lgk_(other.lgk_),
		str_(other.str_)
{}

void article::swap(article& other)
{
    std::swap(langin_,other.langin_);
    std::swap(langout_,other.langout_);
    std::swap(lgk_,other.lgk_);
    std::swap(fields_,other.fields_);
	std::swap(str_,other.str_);
}

article& article::operator=(const article& other)
{
	article(other).swap(*this);
	return *this;
}

void article::assign(mem_pos pos,size_t len)
{
	if (pos[0] >=32) throw std::runtime_error("wrong field format, no separator at the start");

    size_t prev_pos = 0;
    field_type curr_fnum = static_cast<field_type>(pos[0]);
    size_t i;
    for(i=1;i<len;++i)
    {
	if (pos[i]<32)
	{
	    fields_[curr_fnum] = mem_vector(&pos[prev_pos+1],&pos[prev_pos+1]+i-prev_pos-1);
	    curr_fnum = static_cast<field_type>(pos[i]);
	    prev_pos = i;
	}
    }
    if (len-prev_pos-1 > 0)
	fields_[curr_fnum] = mem_vector(&pos[prev_pos+1],&pos[prev_pos+1]+len-prev_pos-1);
}

//NOTE: Windows has a strange problem:
// due a assert fault when big string is return from method, (this problem only with dll's)
// so I have to use static internal string and return by reference;
const std::string& article::as_string(field_type fnum) const
{
	str_.clear();
	field_map::const_iterator it = fields_.find(fnum);
    if (it != fields_.end())
    {
	const mem_vector item = it->second;
	item_traits<std::string>::assign(str_,&item[0],item.size());
    }
    return str_;
}

const std::string& article::orig() const
{
	return as_string(static_cast<field_type>(langin_));
}

const std::string& article::translated() const
{
    return as_string(static_cast<field_type>(langout_));
}

int article::lgk() const
{
    return lgk_;
}

const std::string& article::subject() const
{
    return as_string(subjectfield);
}


void article::set_langs(lang_code langin,lang_code langout)
{
    langin_ = langin;
    langout_ = langout;
}

void article::set_lgk(int lgk)
{
    lgk_ = lgk;
}
