#include "linguas.hh"

using namespace mt;

linguas::linguas()
{
    push_back(std::make_pair(english,russian));
    push_back(std::make_pair(russian,english));
}
