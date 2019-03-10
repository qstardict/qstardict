/* btree.hh
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#ifndef MT_BTREE_HH__
#define MT_BTREE_HH__

#include <string>
#include <fstream>
#include <algorithm>

//debug 
#include <iostream>

#include "brecset.hh"
#include "bgreater.hh"
#include "btree_dll.hh"

namespace mt
{
	const i16_t sysident='M';
	const char upper_page_id='U';
	const char lower_page_id='L';

	class btree
	{ /**< multitran's database */

		struct db_header
		{ /**< multitran database header */
			i16_t identifier_; /**< magic number */
			i16_t encode_page_; /**< are we need to made additional page encoding/decoding */
			long anchor_offset_; /**< offset of the first available level */
			i16_t void4_;
			i16_t language_; /**< language code */
			long max_num_;
			unsigned long check_sum_; /**< control sum of the header: unused */
			unsigned long void5_;
			i16_t type_;
			i16_t void6_;
			i16_t level_size_;	/**< one page size */
			i16_t code_table_;             //Windows or DOS
			i16_t version_;  /**< database format version */
			long max_records_;  /**< number of the records in database */
			long	max_info_blocks_;
		};

	public:
		LIBBTREE_API btree(const std::string&);
#ifdef WIN32
		LIBBTREE_API ~btree(); //under WIN32 "virtual" usage gives us a segfault
#else
		virtual ~btree();
#endif

		/**
		 * @todo optimization: first try to find in current page, if it is low level page
		 */
		template <typename GreaterT,typename EqualT>
		bool find_if(GreaterT gr,EqualT eq)
		{ /**< search for the key */
			//TODO: Try fast search here
			page_num_ = 1;
			//load root page
			is_.seekg(header_.anchor_offset_);
			is_ >> records_;
		    
			//walk throw the upper level pages
			while(records_.data()[0] == upper_page_id)
			{
			    //walk throw the records on this page
			    off_ = std::find_if(records_.begin(),
					       records_.end(),
					       gr);
			    
			    //calculate next page number
			    if (off_  == records_.end())
				page_num_ = records_.next_page(); //default reference on each page
			    else
				page_num_ = *(reinterpret_cast<const i16_t*>(off_->second.data_));//founded key's value is a page number

			    load_page(page_num_);
    		}

			off_ = std::find_if(records_.begin(),
					    records_.end(),
					    or_(gr,eq));

			return (off_ != records_.end()) && eq(*off_);
		}
		
		LIBBTREE_API bool find(mem_pos key,size_t keysize);
		LIBBTREE_API const bdata_pair& get();

		template <typename FunctorT>
		bool find_dups(FunctorT f)
		{
		    typedef not_impl<beq> not_eq_t;
		    typedef or_impl< not_eq_t ,FunctorT> new_f_t;

		    beq equal(off_->first.data_,off_->first.len_);

		    not_eq_t  ne(equal);
		    new_f_t nf(ne, f);
		    walk_if<new_f_t>(nf);
		    return (off_ != records_.end()) && equal(*off_);
		}
		
		template <typename FunctorT>
		void walk_if(FunctorT f)
		{
		    size_t prev_page;
		    do{
			prev_page = page_num_;
			off_ = std::find_if(off_,records_.end(),f);

			if (off_ != records_.end()) break;
			
			//try to continue searching at the next page
			load_page(page_num_ = records_.next_page());
			off_ = records_.begin();
		    }while( prev_page != page_num_ );
		}

	protected:
		//current memory window
		size_t 	page_num_;
		brecset records_; /**< current set of records */
		biterator off_; /**< current offset in window */
	private:
		//this is uncopiable class
		btree();
		btree(const btree&);
		btree& operator=(const btree&);

		LIBBTREE_API void load_page(size_t page_num);
		std::ifstream is_;
		db_header	header_;
		unsigned long cksum_;
	};
}

#endif
