/* identification.hh
 * This file is part of libfacet library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libfacet as the file COPYING.
 */

#ifndef MT_IDENTIFICATION_HH__
#define MT_IDENTIFICATION_HH__

#include <string>
#include <locale>

#include "facet_dll.hh"

namespace mt
{

    class identification: public std::locale::facet
    { /**< language english name and encoding */
     public:
	LIBFACET_API identification(const std::string& name,size_t i = 0);
	LIBFACET_API virtual ~identification() {}
	
	LIBFACET_API const std::string& str() const; /**< language's name in english */
	LIBFACET_API const std::string& codeset() const; /**< language's codeset */
	
	LIBFACET_API static std::locale::id	id;
     private:
	std::string name_;
	std::string codeset_;
    };
}


#endif
