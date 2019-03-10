/* translation.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#include <stdexcept>

#include "btree.hh"
#include "singleton.hh"
#include "path.hh"
#include "dict_path.hh"

#include "translation.hh"

using namespace mt;

typedef btree	index_type;
typedef bmap<mem_vector,article> transdb_type;

typedef singleton_array<index_type> index_db;
typedef singleton_array<transdb_type> trans_db;

struct candidate
{
    candidate(const bdata_pair& item):
    	key_(item.first.data_, item.first.data_+item.first.len_),
	value_(item.second.data_, item.second.data_+item.second.len_)
    {}
    mem_vector key_;
    mem_vector value_;
};

typedef std::vector<candidate>	candidate_list; //list of candidate strings

namespace
{
    struct append_lemm
    {
	append_lemm(lemm_phrase* list,lang_code lang,const std::string& path):
	    list_(list),
	    lang_(lang),
	    path_(path)
	{}
	void operator()(const std::string& word)
	{
		list_->push_back(lemm(word,lang_,path_));
	}
	lemm_phrase	*list_;
	lang_code	lang_;
	std::string	path_;
    };

    struct append_phrase
    {
	append_phrase(mt::mem_pos first,candidate_list* list,
		      size_t max_size):
	    first_(first),
	    list_(list),
	    max_size_(max_size)
	{}
    
	bool operator()(const mt::bdata_pair& item)
	{
	    if (memcmp(item.first.data_,first_,3))
		return true;

	    if ((item.first.len_/3) > max_size_)
		return false;
	    
	    list_->push_back(candidate(item));
	    
	    return false;
	}
	
	mt::mem_pos	first_; /**< first word in the phrase */
	candidate_list	*list_;  /**< list where we append candidates */
	size_t		max_size_; /**< little optimization:
				   it's not interesting for us to see phrases longer then our */
    };
    
    struct append_good
    {
	append_good(candidate_list *good,lemm_phrase *list,size_t offset):
	    good_(good),
	    list_(list),
	    offset_(offset)
	{}
	
	void operator()(const candidate& c)
	{
	    const mem_vector& item = c.key_;//process key only
	    size_t k;
	    for(k=1;k<(item.size()/3);++k)
	    {
		word_info_list &lst = list_->at(offset_+k).words_;
		//compare k-th word with all candidates in original string on k-th position
		word_info w(&item[3*k]);
		word_info_list::iterator it = std::find(lst.begin(),lst.end(),w);

		//in this position full non-match -> wrong phrase
		if (it == lst.end()) break;
	    }
	    if (k == (item.size()/3)) good_->push_back(c); // if <= then was mismatch on some word
	}
		
	candidate_list		*good_;
	lemm_phrase			*list_;
	size_t				offset_;
    };
    
    bool compare_sizes(candidate& k1,candidate& k2)
    {
	return k1.key_.size() < k2.key_.size();
    }
    
    struct fill_artset
    {
	typedef bmap<mem_vector,article> transdb_type;
	
	fill_artset(article_set* as,
	            size_t max_size,transdb_type* trans, word_info_list* words):
	    as_(as),
	    max_size_(max_size),
	    words_(words),
	    trans_(trans)
	{}
	
	void operator()(const candidate& c)
	{
		const mem_vector& list = c.key_;
	    const mem_vector& numbers = c.value_;
	    
	    if (list.size() == max_size_)
	    {
		int lgk = 0;
		if (list.size() == 3)
		{
		    word_info w(&list[0]);
		    word_info_list::iterator it = std::find(words_->begin(),words_->end(),w);
		    if (it != words_->end()) lgk = it->lgk_;
		}
		for (size_t i=0;i<(numbers.size()-2)/3;++i)
		{
		    mem_pos start = &numbers[0]+i*3+2;

		    mem_vector number(start,start+3);
		    article a = trans_->get(number);
		    
		    a.set_lgk(lgk);
		    a.set_langs(as_->langin_,as_->langout_);
		    as_->articles_.push_back(a);
		}
	    }
	}
	article_set* as_;
	size_t max_size_;
	
	word_info_list* words_;
	transdb_type* trans_;
    };
}

translation::translation(const phrase& input,lang_code langin,lang_code langout,const std::string& path)
{
	//first step: phrase lemmatization
    lemm_phrase list;
    std::for_each(input.begin(),input.end(),append_lemm(&list,langin,path));

    index_type &index = index_db::instance( path+path_separator()+dict_path(langin,langout)+
                                            lang_name(langin)[0]+lang_name(langout)[0]+"d" );


    transdb_type& trans = trans_db::instance(path+path_separator()+dict_path(langin,langout)+
					     std::min(lang_name(langin)[0],lang_name(langout)[0])+
					     std::max(lang_name(langin)[0],lang_name(langout)[0])+"t");

    for(size_t offset = 0; offset < list.size();)
    {
		candidate_list candidates;

		//try to find all strings for the current first word in phrase: fill candidates list
		for(size_t j=0;j<list[offset].words_.size();++j)
		{
			mem_vector& vec = list[offset].words_[j].num_;
			if (index.find(&vec[0],vec.size()))
			index.walk_if<append_phrase>(append_phrase(&vec[0],
	                                             &candidates,
						     list.size()-offset));
		}
	
		//process all strings that begining from current word :FIXME replace with foreach
		candidate_list good_list;
		std::for_each(candidates.begin(),
					  candidates.end(),
					  append_good(&good_list,&list,offset));

		article_set as(langin,langout);

		if (!good_list.empty())
		{
			//calculate maximum size of the good phrase
			candidate_list::iterator it = std::max_element(good_list.begin(),
                                                    	   good_list.end(),
						    							   compare_sizes);
	

			//dump all phrases (it's terms) with maximum possible length to term list
			std::for_each(good_list.begin(),
	            		  good_list.end(),
						  fill_artset(&as,it->key_.size(),&trans,&(list[offset].words_)));
			  
			offset += it->key_.size()/3;
		}
		else
		{
			as.untranslated_ = input[offset];
			++offset;
		}
	
		art_sets_.push_back(as);
    }
}
