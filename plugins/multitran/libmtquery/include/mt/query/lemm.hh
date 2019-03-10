/* lemm.hh
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#ifndef MT_LEMM_HH
#define MT_LEMM_HH

#include <vector>

#include "lang_info.hh"
#include "config.hh"
#include "bdata.hh"

#include "mtquery_dll.hh"

namespace mt
{
    struct word_info
    { /**< information about word after lemmatization process */
	word_info():
	    lgk_(0),
	    num_(3)
	{}
	word_info(mem_pos pos,ui16_t lgk = 0):
	    lgk_(lgk),
	    num_(pos,pos+3)
	{}
	ui16_t lgk_;/**< word's lgk */
	mem_vector num_; /**< word number: long compressed to 3 bytes*/
    };

    inline bool operator==(const word_info& lhs,const word_info& rhs)
    { /**< only word number are interesting for us to compare */
	return lhs.num_ == rhs.num_;
    }

    typedef std::vector<word_info> word_info_list;

    class lemm
    { /**< made morphological analisys of the word and story all it's wordnumbers */
	public:
	    LIBMTQUERY_API lemm(const std::string& str,lang_code lang,const std::string& path=datapath);
	    LIBMTQUERY_API ~lemm() {}

	    void add(const word_info& num);
	    
	    bool empty() const;
	    LIBMTQUERY_API size_t size() const;
	    const std::string& word() const;

	    word_info_list words_;/**< possible unique word numbers */

	private:
	    std::string		word_; /**< input string*/
    };
}    

#endif
