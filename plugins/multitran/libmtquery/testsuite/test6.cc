#include <iostream>

#include "file_map.hh"


int main()
{
    mt::file_map m("/usr/share/multitran/subjects.txt");
    std::cout<<m.short_name("3")<<std::endl;
    std::cout<<m.full_name("3")<<std::endl;
    std::cout<<m.any_name("3")<<std::endl;
    std::cout<<m.any_name("14")<<std::endl;
}
