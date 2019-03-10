/* test5.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#include <iostream>

#include "article.hh"


int main()
{
    mt::mem_vector packed_code(3);
    
    int code=791651;
//    int code=528819;
    
    packed_code[0] = code & 0x0000ff;
    packed_code[1] = (code & 0x00ff00)>>8;
    packed_code[2] = (code & 0xff0000)>>16;

    mt::bmap<mt::mem_vector,mt::article> b("/usr/share/multitran/eng_rus/dict.ert");
    
    mt::article a = b.get(packed_code);

    std::cout<<"orig:"<<a.orig()<<std::endl;
    std::cout<<"translated:"<<a.translated()<<std::endl;
    std::cout<<"subject:"<<a.subject()<<std::endl;
}
