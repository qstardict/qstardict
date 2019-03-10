/* test1.cc
 * This file is part of libfacet library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libfacet as the file COPYING.
 */

#include <iostream>

#include "typographic.hh"
#include "identification.hh"
#include "alphabet.hh"

int main()
{

	mt::typographic t1("russian"),t2("english");
    
	std::cout<<"l:"<<t1.to_simpler('¸')<<std::endl;    
	std::cout<<"l:"<<t1.to_simpler('F')<<std::endl; 
    
	mt::identification id("C");
	std::cout<<"name:"<<id.str()<<" "<<id.codeset()<<std::endl;

	mt::identification id1(std::locale("").name());
	std::cout<<"name:"<<id1.str()<<" "<<id1.codeset()<<std::endl;

#ifdef WIN32
	const std::string dbpath="..\\..\\testsuite";
#else
	const std::string dbpath="./";
#endif

	mt::alphabet	abc(id1.str(),dbpath);
	std::cout<<"alphabet: "<<abc.letters()<<std::endl;
	std::cout<<"is alpha ('z'): "<<std::boolalpha<<abc.isalpha('z')<<std::endl;
	std::cout<<"is alpha ('ÿ'): "<<std::boolalpha<<abc.isalpha('ÿ')<<std::endl;

	std::string line;
	std::getline(std::cin,line);

}
