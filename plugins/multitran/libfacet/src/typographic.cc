/* typographic.cc
 * This file is part of libfacet library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libfacet as the file COPYING.
 */

#include <map>

#include <iostream>

#include "typographic.hh"

namespace
{
	inline
	std::string join(const std::string& s1,const std::string& s2)
	{
		return (s1+'\000'+s2);
	}
    
	class simple_map
	{
		typedef std::map<std::string,std::string> map_type;
	public:
		simple_map()
		{
			//russian simplifications: CP1251
			map_.insert(map_type::value_type(join("russian","¸"),"å"));
			map_.insert(map_type::value_type(join("russian","¨"),"Å"));

			//german simplfications: ISO-8859-5
			map_.insert(map_type::value_type(join("german","ß"), "ss"));
			map_.insert(map_type::value_type(join("german","ï"), "i")); // zairer
			map_.insert(map_type::value_type(join("german","Ï"), "I")); // zairer
			map_.insert(map_type::value_type(join("german","ä"), "ae"));
			map_.insert(map_type::value_type(join("german","Ä"), "Ae"));
			map_.insert(map_type::value_type(join("german","ü"), "ue"));
			map_.insert(map_type::value_type(join("german","Ü"), "Ue"));
			map_.insert(map_type::value_type(join("german","ö"), "oe"));
			map_.insert(map_type::value_type(join("german","Ö"), "Oe"));
		}
	  
		std::string get(const std::string& lang,char c) const
		{
			map_type::const_iterator it = map_.find(join(lang,std::string(1,c)));
			return (it == map_.end())?"":it->second;
		}
	private:
		map_type map_;
	};

	static const simple_map simplifications;
}


std::locale::id mt::typographic::id;

using namespace mt;

/**
 * I've temporary made maps for each language, it will be replaced with external datafiles
 * in the future
 */
typographic::typographic(const std::string& langname,size_t i):
    std::locale::facet(i),
    lang_(langname)
{}


std::string typographic::to_simpler(char c) const
{
	const std::string res=simplifications.get(lang_,c);
	return res.empty()?std::string(1,c):res;
}
