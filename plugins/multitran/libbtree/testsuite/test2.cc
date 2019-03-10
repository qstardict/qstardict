/* test2.cc
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#include <iostream>
#include <stdexcept>

#include "bmap.hh"

#ifdef WIN32
	const char edatapath[]="..\\..\\testsuite\\english\\";
	const char rdatapath[]="..\\..\\testsuite\\russian\\";
#else
	const char edatapath[]="english/";
	const char rdatapath[]="russian/";
#endif

int main()
{
	try
	{
//		mt::bmap<std::string,mt::i16_t> b("/usr/share/multitran/english/end.eng");
//		std::cout<<b.get("sya")<<std::endl;

		mt::bmap<std::string,mt::i16_t> b(edatapath+std::string("end.eng"));
		std::cout<<b.get("de")<<std::endl;
		
//		mt::bmap<mt::i16_t,std::string> b1("/usr/share/multitran/english/endnum.eng");
//		std::cout<<b1.get(364)<<std::endl;

		mt::bmap<mt::i16_t,std::string> b1(edatapath+std::string("endnum.eng"));
		std::cout<<b1.get(5)<<std::endl;

	}
	catch (std::runtime_error& e)
	{
		std::cerr << "catch:" << e.what() << std::endl;
	}

	std::string line;
	std::getline(std::cin,line);
}
