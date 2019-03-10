/* typographic.hh
 * This file is part of libfacet library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libfacet as the file COPYING.
 */

#ifndef MT_TYPOGRAPHIC_HH__
#define MT_TYPOGRAPHIC_HH__

#include <string>
#include <locale>

#include "facet_dll.hh"

namespace mt
{
    /**
     * typographic simplifications for varios languages
     */
     class typographic: public std::locale::facet
     {
        public:
			LIBFACET_API typographic(const std::string& langname,size_t i = 0);
			LIBFACET_API virtual ~typographic() {}

			LIBFACET_API std::string to_simpler(char c) const; /**< return simpler version of appropriate letter */
	
			LIBFACET_API static std::locale::id	id;
		private:
			std::string	lang_;
     };
}

#endif
