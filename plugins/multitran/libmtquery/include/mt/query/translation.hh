/* translation.hh
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#ifndef MT_TRANSLATE_HH__
#define MT_TRANSLATE_HH__

#include <vector>
#include <string>

#include "lemm.hh"
#include "config.hh"
#include "article.hh"

#include "mtquery_dll.hh"

namespace mt
{
    typedef std::vector<std::string> phrase;//input phrase for translation
    typedef std::vector<lemm> lemm_phrase;//phrase after lemmatization
    typedef std::vector<article> article_list;//phrase after lemmatization

    struct article_set 
    { /**< set of articles for the word */
	article_set(lang_code langin,lang_code langout):
	    langin_(langin),
	    langout_(langout)
	{}
	~article_set() {}
	std::string untranslated_; /**< untranslated word */
	article_list articles_; /**< articles with translations for this word */
	lang_code langin_;
	lang_code langout_;
    };

    typedef std::vector<article_set> artset_vector;

    class translation
    {/**< storage with articles for given phrase */
	public:
	    LIBMTQUERY_API translation(const phrase&,lang_code langin,lang_code langout,const std::string& path=datapath);
		LIBMTQUERY_API ~translation() {}
		LIBMTQUERY_API artset_vector& asets() { return art_sets_; }
	private:
		artset_vector art_sets_; /**< all terms avaliable for this phrase */	
    };
}

#endif
