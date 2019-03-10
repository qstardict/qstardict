/* test2.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#include <iostream>
#include <stdexcept>

#include "siktable.hh"

#ifdef WIN32
const char datapath[]="c:\\to_see\\multitran\\mtdata\\english\\siktable.eng";
#else
const char datapath[]="/usr/share/multitran/english/siktable.eng";
#endif

int main()
{
	try
	{
	    mt::siktable s(datapath);
	    std::cout<<"35:"<<s.sik(35)[0]<<std::endl;
	    std::cout<<"35:"<<s.sik(35)[1]<<std::endl;
	    std::cout<<"36:"<<s.sik(36)[0]<<std::endl;
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "catch:" << e.what() << std::endl;
	}

	int ch;
	std::cin>>ch;
}
