/* alphabet.cc
 * This file is part of libfacet library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libfacet as the file COPYING.
 */

//for debug
#include <iostream>

#include <fstream>

#include "str.hh"
#include "path.hh"

#include "alphabet.hh"

using namespace mt;

std::locale::id mt::alphabet::id;

alphabet::alphabet(const std::string& langname,const std::string& datapath,size_t i):
    std::locale::facet(i)
{
	std::ifstream	is((datapath+path_separator() + langname + path_separator() +
			    "abc."+ langname.substr(0,3)).c_str());

	std::string buff;
	while(mt::getline(is,buff))
	{
		buff = mt::trim(buff);
		std::string line;
		mt::getline(is,line);//get next line with contents
    
		if ("abcd" == buff)
			abcd_ = line;
		else if ("vowel" == buff)
			vowel_ = line;
		else if ("hissing" == buff)
			hissing_ = line;
		else if ("partends" == buff)
			partends_ = line;
		else if ("parts" == buff)
			parts_ = line;
	}
}

/** @todo may me use more fast structure for alphabet  */
bool alphabet::isalpha(char c) const
{
	return abcd_.find(c) != std::string::npos;
}

const std::string& alphabet::letters() const
{
	return abcd_;
}

const std::string& alphabet::parts() const
{
	return partends_;
}

const std::string& alphabet::partends() const
{
	return parts_;
}
