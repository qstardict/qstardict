/* article.hh
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#ifndef MT_ARTICLE_HH__
#define MT_ARTICLE_HH__

#include <map>
#include <string>

#include "lang_info.hh"
#include "bmap.hh"

#include "mtquery_dll.hh"

namespace mt
{
    struct article
    {/**< one article in dictionary */
	enum field_type
	{
	    note = 6,
	    example,
	    user,
	    source,
	    void1, //10 - unused
	    time,
	    picture,
	    void2, //13 - unused
	    e_mail,
	    subjectfield,
	    void3, //16 - unused
	    dump,
	    color,
	    bullet,
	    bullet1
	};
	
	LIBMTQUERY_API article();
	LIBMTQUERY_API article(const article&);
	LIBMTQUERY_API article& operator=(const article& other);
	LIBMTQUERY_API ~article() {}

	void set_lgk(int lgk);
	void set_langs(lang_code langin,lang_code langout);
	LIBMTQUERY_API void assign(mem_pos pos,size_t len);/**< assign to fields */

	LIBMTQUERY_API const std::string& orig() const;/**< original phrase */
	LIBMTQUERY_API const std::string& translated() const;/**< translated phrase */
	LIBMTQUERY_API int lgk() const;/**< phrase's lgk if we have it */
	LIBMTQUERY_API const std::string& subject() const; /**< return subject number */
    
	protected:
	    void swap(article &other);
    
	private:
	    const std::string& as_string(field_type fnum) const;
	
	    typedef std::map<field_type,mem_vector> field_map;

	    field_map fields_;
	    lang_code langin_; /**< original language code */
	    lang_code langout_; /**< target language code */
	    int lgk_; /**< original's lgk*/
		mutable std::string str_;
    };

    //traits for bmap
    template<>
    struct item_traits<article>
    {
	static mem_pos data(const article&) { return 0; } //not implemented
	static size_t length(const article&) { return 0; } //not implemented
	static article def_value() { return article(); }
	static void assign(article& to,mem_pos val,size_t len) { to.assign(val,len); }
    };

}

#endif
