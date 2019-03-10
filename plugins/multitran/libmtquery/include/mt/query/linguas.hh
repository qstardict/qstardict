#ifndef MTQUERY_LINGUAS_HH__
#define MTQUERY_LINGUAS_HH__

#include <vector>
#include <utility>

#include "lang_info.hh"
#include "config.hh"

namespace mt
{
    typedef std::pair<lang_code,lang_code> lang_pair;
    
    typedef std::vector<lang_pair> lang_pair_vector;
    /**
     * list of dictionaries available in system
     * @todo add real scanning of /usr/share/multitran for available dictionaries
     */
    struct linguas: public lang_pair_vector
    {
	linguas();
    };
}


#endif
