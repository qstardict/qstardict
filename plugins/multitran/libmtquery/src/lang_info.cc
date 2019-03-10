/* lang_info.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */
#include <iostream>


#include <map>

#include "shared_ptr.hh"
#include "lang_info.hh"

namespace
{
    typedef std::map<mt::lang_code,std::string> lname_map_type;
    class locale_map: public lname_map_type
    {
	public:
	locale_map():
	    lname_map_type()
	{
	    //M$ as usual has own incompatible names for locales
	    insert(lname_map_type::value_type(mt::english, "C"));
#ifdef WIN32
	    insert(lname_map_type::value_type(mt::russian, "ru"));//Russian_Russia.1251
	    insert(lname_map_type::value_type(mt::german, "ge"));//German_Germany.1251
	    insert(lname_map_type::value_type(mt::french, "fr"));//French_France.1251
#else
	    insert(lname_map_type::value_type(mt::russian, "ru_RU.CP1251"));
	    insert(lname_map_type::value_type(mt::german, "de_DE"));
	    insert(lname_map_type::value_type(mt::french, "fr_FR"));
#endif
	}
    };
    locale_map names;
    
    typedef std::map<std::string,mt::shared_ptr<std::locale> > loc_map_type;
    loc_map_type locales;
}

const std::locale *mt::get_locale(mt::lang_code num,const std::string& path)
{
    return get_locale(names[num],path);
}

const std::locale *mt::get_locale(const std::string& locname,const std::string& path)
{
    if (!locales[locname].get())
	{
		mt::identification *f1=new mt::identification(std::locale(locname.c_str()).name());
		mt::alphabet *f2=new mt::alphabet(f1->str(),path);
	
		using std::locale;
		locales[locname] = new locale(locale(locale(locname.c_str()),f1),f2);
    }
    return locales[locname].get();
}


std::string mt::upper_str(mt::lang_code num,std::string str)
{
    std::transform(str.begin(),str.end(),str.begin(),
		   std::bind1st(std::ptr_fun(&mt::upper_ch),num));
    return str;
}

std::string mt::lower_str(mt::lang_code num,std::string str)
{
    std::transform(str.begin(),str.end(),str.begin(),
		   std::bind1st(std::ptr_fun(&mt::lower_ch),num));
    return str;
}

size_t mt::count_letters(mt::lang_code num,const std::string& str)
{
  return std::count_if(str.begin(),str.end(),
		       std::bind1st(std::ptr_fun(&mt::test_ch),num));
}

