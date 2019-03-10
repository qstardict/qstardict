/* test3.cc
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#include <iostream>
#include <stdexcept>

#include "bmap.hh"

    struct dup_print
    {
	bool operator()(const mt::bdata_pair&)
	{
	    std::cout<<"another dup\n";
	    return false;
	}
    };

int main()
{
	try
	{
		mt::btree b("/usr/share/multitran/english/stem.eng");

		std::cout << "find result air:" << std::boolalpha<<
		b.find(reinterpret_cast<mt::mem_pos >("air"), 3)<< std::endl;
		std::cout<<b.find_dups<dup_print>(dup_print())<<std::endl;
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "catch:" << e.what() << std::endl;
	}
}
