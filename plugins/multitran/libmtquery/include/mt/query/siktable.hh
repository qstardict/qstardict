/* siktable.hh
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#ifndef MT_SIKTABLE_HH__
#define MT_SIKTABLE_HH__

#include <vector>
#include <string>

//for typedefs
#include "bdata.hh"

#include "mtquery_dll.hh"

namespace mt
{
    class siktable
    { /**< database of "word change class" - sik */
	typedef ui16_t sik_type;
	public:
	    LIBMTQUERY_API siktable(const std::string& path);
	    LIBMTQUERY_API ~siktable() {}
	    LIBMTQUERY_API const sik_type *sik(int num) const;
	    bool check(int siknum, int endnum,int lgk) const; /**< check end number according it's lgk */
	private:
	    bool check_form(int lgk,int formnum) const; /**< check form */

	    std::vector<char> data_;
	    sik_type max_ends_; /**< maximum possible sik number */
    };
}

#endif
