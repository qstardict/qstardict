/* alphabet.hh
 * This file is part of libfacet library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libfacet as the file COPYING.
 */

#ifndef MT_ALPHABET_HH__
#define MT_ALPHABET_HH__

#include <string>
#include <locale>

#include "facet_dll.hh"

namespace mt
{
    /**
     * language alphabet information
     * I don't use isalpha,'cause we need different logic:
     * e.g. isalpha for 'f' for russian returns true, but we need 'false' answer.
     * So we need don't superset (language symbols with additional symbols), but
     * sharp set of the letters
     */
    class alphabet: public std::locale::facet
    {
     public:
		LIBFACET_API alphabet(const std::string& langname,
				 const std::string& datapath="/usr/local/share/multitran",size_t i = 0);
		LIBFACET_API virtual ~alphabet() {}
	
		LIBFACET_API bool isalpha(char c) const;
		LIBFACET_API const std::string& letters() const;
		const std::string& parts() const;
		const std::string& partends() const;
		
		LIBFACET_API static std::locale::id	id;
     private:
		std::string abcd_;
		std::string vowel_;
		std::string hissing_;
		std::string partends_;
		std::string parts_;
    };

}


#endif
