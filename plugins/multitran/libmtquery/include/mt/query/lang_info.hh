/* lang_ingo.hh
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#ifndef LANG_INFO_HH__
#define LANG_INFO_HH__

#include <locale>
#include <functional>
#include <algorithm>

#include "identification.hh"
#include "alphabet.hh"
#include "config.hh"
#include "mtquery_dll.hh"

namespace mt
{
    enum lang_code /**< Language codes */
    {
	none,
	english,
	russian,
	german,
	french,
	max_lang,
	digit = 100 /**< strange abstract language for all digits */
    };

    /**
     * library internally uses numbers for language codes (we must fix it in the future)
     * so we have to made conjugacy between normal locales (and this locale names) and 
     * language codes
     */
    LIBMTQUERY_API const std::locale *get_locale(mt::lang_code num,
				  const std::string& path=datapath); 
    /**< @return appropriate locale for language code */			  
    LIBMTQUERY_API const std::locale *get_locale(const std::string& locname,
				  const std::string& path=datapath);
    /**< @return appropriate locale enhanced with our facets */


    inline
    std::string lang_name(mt::lang_code num)/**< @return language name in English */
    {
	return std::use_facet<mt::identification>(*get_locale(num)).str();
    }

    inline
    std::string short_lang_name(mt::lang_code num) /**< @return 3-letter language name in English */
    {
	return lang_name(num).substr(0,3);
    }

    inline
    char upper_ch(mt::lang_code num,char c) /**< @return upper case for appropriate language*/
    {
	return std::use_facet<std::ctype<char> >(*get_locale(num)).toupper(c);
    }

    inline
    char lower_ch(mt::lang_code num,char c) /**< @return lower case for appropriate language*/
    {
	return std::use_facet<std::ctype<char> >(*get_locale(num)).tolower(c);
    }
    
    std::string upper_str(mt::lang_code num,std::string);
    std::string lower_str(mt::lang_code num,std::string);

    inline
    bool test_ch(mt::lang_code num,char c) /**< test are this symbol is letter from appropriate language*/
    {
	if (num == mt::none || !c)
	    return false;
	if (num == mt::digit)
	    return ( isdigit(c)!= 0 );
	return std::use_facet<mt::alphabet>(*get_locale(num)).isalpha(c);
    }
    
    inline
    bool is_upper(mt::lang_code num,char c)
    {
	return test_ch(num,c) && (upper_ch(num,c) == c);
    }

    inline
    bool is_lower(mt::lang_code num,char c)
    {
	return test_ch(num,c) && (lower_ch(num,c) == c);
    }

    inline
    const std::string& abc(mt::lang_code num)
    {
	return std::use_facet<mt::alphabet>(*get_locale(num)).letters();
    }

    inline
    const std::string& partends(mt::lang_code num)
    {
	return std::use_facet<mt::alphabet>(*get_locale(num)).partends();
    }

    inline
    const std::string& parts(mt::lang_code num)
    {
	return std::use_facet<mt::alphabet>(*get_locale(num)).parts();
    }

    size_t count_letters(mt::lang_code num,const std::string& str);
}

#endif
