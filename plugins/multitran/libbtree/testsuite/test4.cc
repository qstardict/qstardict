/* test4.cc
 * This file is part of libbtree library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#include <iostream>
#include <stdexcept>

#include "bmap.hh"

    struct append_phrase
    {
	append_phrase(mt::mem_pos first):
	    first_(first)
	{}
    
	bool operator()(const mt::bdata_pair& item)
	{
	    std::cout<<"here:"<<memcmp(item.first.data_,first_,3)<<std::endl;
	    return memcmp(item.first.data_,first_,3);
	}
	
	mt::mem_pos	first_;
    };

int main()
{
	try
	{
		mt::btree b("/usr/share/multitran/eng_rus/dict.erd");

		long code = 55106; //100
		
		unsigned char	packed_code[3];
	
		packed_code[0] = code & 0x0000ff;
		packed_code[1] = (code & 0x00ff00)>>8;
		packed_code[2] = (code & 0xff0000)>>16;

		std::cout<<std::boolalpha<<b.find(packed_code,3)<<std::endl;

		if (b.find(packed_code,3))
		    b.walk_if<append_phrase>(append_phrase(packed_code));
		
		//must be 220 strings as a result
	}
	catch (std::runtime_error& e)
	{
		std::cerr << "catch:" << e.what() << std::endl;
	}
}
