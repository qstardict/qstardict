/* lemm.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#include "bmap.hh"
#include "path.hh"
#include "str.hh"

#include "siktable.hh"
#include "singleton.hh"

#include "lemm.hh"

using namespace mt;

typedef bmap<std::string,mt::i16_t> ends_type;
typedef btree			  stems_type;


typedef singleton_array<ends_type> ends_db;
typedef singleton_array<stems_type> stems_db;
typedef singleton_array<siktable> siks_db;

namespace
{
	struct check_stem
	{ /**< check all available stems */
		check_stem(int num,siktable* sik,lemm* l):
			num_(num),
			lemm_(l),
			siks_(sik)
		{}
    
		bool operator()(const mt::bdata_pair& item)
		{
			//walk throw 7byte blocks
			for(size_t count=0;count<(item.second.len_-1)/7;++count)
			{
				mem_pos start = item.second.data_+1+count*7; //start of the next  7byte block

				const ui16_t	sik = *reinterpret_cast<const ui16_t*>(start+3) & 0x1FFF; //clear leading additional 3 bits (???)
				const ui16_t	lgk = *reinterpret_cast<const ui16_t*>(start+5) ;

				if (siks_->check(sik,num_,lgk))
					lemm_->add(word_info(start,lgk)); //search for the valid sik's
			}
	
			return false;
		}

		int num_;
		lemm *lemm_;
		siktable *siks_;
	};
}

lemm::lemm(const std::string& word,lang_code langnum,const std::string& path):
    word_(word)
{
    const std::string lang = lang_name(langnum);
    const std::string short_lang = lang.substr(0,3);

    ends_type &ends = ends_db::instance(path+path_separator()+lang+
                    		        path_separator()+"end."+ short_lang);

    stems_type &stems = stems_db::instance(path+path_separator()+lang+
                    		    	       path_separator()+"stem."+ short_lang);

    siktable	&siks = siks_db::instance (path+path_separator()+lang+
		                                   path_separator()+"siktable."+ short_lang);

    const int len = word.length();
    for(int i=len; i >= 0 ;--i)
    {
		//check for correct number
		const std::string end = word.substr(i,std::string::npos);
		int end_number;
		if (end.empty() || ('#' == end[0]))
			end_number = 1;
		else
			end_number = ends.get(reverse(end));
	
		if (!end_number) continue;

		//check for correct stem
		const std::string stem = word.substr(0,i);
		if(!stems.find(reinterpret_cast<mt::mem_pos>(stem.c_str()),stem.length()))
			continue;

		//then check for correct sik
		stems.find_dups<check_stem>(check_stem(end_number,&siks,this));
    }
}

void lemm::add(const word_info& wordnum)
{
    words_.push_back(wordnum);
}

bool lemm::empty() const
{
    return words_.empty();
}

size_t lemm::size() const
{
    return words_.size();
}

const std::string& lemm::word() const
{
    return word_;
}
