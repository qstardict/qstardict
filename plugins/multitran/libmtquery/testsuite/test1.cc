/* test1.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#include <iostream>
#include <stdexcept>

#include "lemm.hh"


int main()
{
	try
	{
		std::cout<<"sostojat',состоять:"<<mt::lemm("состоять",mt::russian).size()<<std::endl;
		std::cout<<"zelenaja,зелёная:"<<mt::lemm("зелёная",mt::russian).size()<<std::endl;
		std::cout<<"drunkly:"<<mt::lemm("drunkly",mt::english).size()<<std::endl;
/*
		std::cout<<"drunkly:"<<mt::lemm("drunkly",mt::english).size()<<std::endl;
		std::cout<<"o:"<<mt::lemm("o",mt::english).size()<<std::endl;
		std::cout<<"vindictively:"<<mt::lemm("vindictively",mt::english).size()<<std::endl;
		std::cout<<"proportionately:"<<mt::lemm("proportionately",mt::english).size()<<std::endl;
	    std::cout<<"proportionate:"<<mt::lemm("proportionate",mt::english).size()<<std::endl;
	    std::cout<<"vindictive:"<<mt::lemm("vindictive",mt::english).size()<<std::endl;
	    std::cout<<"tested:"<<mt::lemm("tested",mt::english).size()<<std::endl;
	    std::cout<<"action:"<<mt::lemm("action",mt::english).size()<<std::endl;
	    std::cout<<"went:"<<mt::lemm("went",mt::english).size()<<std::endl;
	    std::cout<<"listing:"<<mt::lemm("listing",mt::english).size()<<std::endl;
	    std::cout<<"listings:"<<mt::lemm("listings",mt::english).size()<<std::endl;
*/
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "catch:" << e.what() << std::endl;
	}
	catch(...)
	{
		std::cerr<<"unknown exception..."<<std::endl;
	}
	std::string line;
	std::getline(std::cin,line);
}
