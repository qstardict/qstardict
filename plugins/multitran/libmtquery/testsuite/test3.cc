/* test3.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#include <iostream>
#include <stdexcept>

#include "lemm.hh"
#include "lang_info.hh"

//test words recognition (cat /usr/share/words|test3)
//simplest spell checker ;)
int main()
{
	try
	{
	    std::string line;
	    while (std::getline(std::cin,line))
	    {
		if (mt::lemm(mt::lower_str(mt::english,line),mt::english).empty())
		    std::cout<<line<<std::endl;
	    }
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "catch:" << e.what() << std::endl;
	}
}
