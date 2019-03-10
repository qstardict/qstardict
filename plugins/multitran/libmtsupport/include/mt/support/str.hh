/* str.hh
 * This file is part of libmtsupport library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtsupport as the file COPYING.
 */

#ifndef STR_MT_HH__
#define STR_MT_HH__

#include <string>
#include <sstream>

#include "mtsupport_dll.hh"

namespace mt
{
    /**
     * special version for multitran's data files
     * with ability to strip M$'s \r symbol
     */
    template<typename CharT, typename Traits, typename Alloc>
    inline
    std::basic_istream<CharT,Traits>&
    getline(std::basic_istream<CharT,Traits>& is,
	  std::basic_string<CharT,Traits,Alloc>& str)
    {
	std::getline(is,str);
	if (*str.rbegin() == 13)
	    str = str.substr(0, str.length() - 1);
	return is;
    }

    LIBMTSUPPORT_API std::string trim_left(const std::string& str); /**< trim left space symbols */
    LIBMTSUPPORT_API std::string trim_right(const std::string& str); /**< trim right space symbols */
    LIBMTSUPPORT_API std::string trim(const std::string& str); /**< trim left and right space symbols */

    /**
     * reverse string content, pos - final position of the reverse,
     * needed by some strange algorithms in libmt
     */
    LIBMTSUPPORT_API std::string reverse(std::string str,std::string::size_type pos = std::string::npos);

    /**
     * replace one substring with other in whole string 
     */    
    LIBMTSUPPORT_API void find_replace(std::string& str,const std::string& from, const std::string& to);

    /**
     * @todo rewrite like boost::lexical_cast in the future (?)
     */
    template <typename T>
    inline
    std::string to_string(T t) /**< template to convert any type to string */
    {
	std::ostringstream ost;
	ost<<t;
	return ost.str();
    }
    
    template <typename T>
    inline
    T string_to(const std::string& str) /**< template to convert string to any type */
    {
	T obj;
	std::istringstream ist(str);
	ist>>obj;
	return obj;
    }

}

#endif
