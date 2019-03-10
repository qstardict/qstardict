/* test1.cc
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#include <iostream>
#include <stdexcept>

#include "btree.hh"
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
		mt::btree b(edatapath+std::string("end.eng"));

		std::cout << "find result #li:" << std::boolalpha<<
		b.find(reinterpret_cast<mt::mem_pos >("#li"), 3)<< std::endl;

		std::cout << "find result #ni:" << std::boolalpha<<
		b.find(reinterpret_cast<mt::mem_pos >("#ni"), 3)<< std::endl;

		std::cout << "find result de:" << std::boolalpha<<
		b.find(reinterpret_cast<mt::mem_pos >("de"), 2)<< std::endl;
		std::cout<<"result:"<<*reinterpret_cast<const mt::i16_t*>(b.get().second.data_)<<std::endl;

		mt::btree b1(rdatapath+std::string("end.rus"));

		std::cout << "find result ÂÎÑ:" << std::boolalpha<<
		b1.find(reinterpret_cast<mt::mem_pos >("ÂÎÑ"), 3) << std::endl;
		
		std::cout << "find result גא:" << std::boolalpha <<
		b1.find(reinterpret_cast<mt::mem_pos >("גא"), 2)<< std::endl;

		std::cout<<"i16_t size:"<<sizeof(mt::i16_t)<<std::endl;
		}
	catch (std::runtime_error& e)
	{
		std::cerr << "catch:" << e.what() << std::endl;
	}
	std::string line;
	std::getline(std::cin,line);
}
