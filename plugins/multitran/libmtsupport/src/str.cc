/* str.cc
 * This file is part of libmtsupport library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtsupport as the file COPYING.
 */
#include <algorithm>

#include "str.hh"

std::string mt::trim_left(const std::string& str)
{
	std::string::size_type pos = str.find_first_not_of(" \t");
	return pos == std::string::npos ? "" : str.substr(pos);
}

std::string mt::trim_right(const std::string& str)
{
	std::string::size_type pos = str.find_last_not_of(" \t");
	return pos == std::string::npos ? "" : str.substr(0, pos + 1);
}

std::string mt::trim(const std::string& str)
{
	return trim_right(trim_left(str));
}


std::string mt::reverse(std::string str,std::string::size_type pos)
{
	std::string::iterator final=(pos == std::string::npos)?str.end():str.begin()+pos;
	std::reverse(str.begin(),final);
	return str;
}

void mt::find_replace(std::string& str,const std::string& from, const std::string& to)
{
	std::string::size_type pos = 0;
	while (true)
	{
		pos = str.find(from,pos);
		if (pos == std::string::npos) break;
	
		str.replace(pos,from.length(),to);
		pos += to.length();
	}
}

