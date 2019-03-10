/* dict_path.hh
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#ifndef DICT_PATH_HH__
#define DICT_PATH_HH__

#include "lang_info.hh"
#include "mtquery_dll.hh"

namespace mt
{
    /**
     * helper to construct path to the dictionary
     */
    LIBMTQUERY_API std::string dict_path(lang_code langin,lang_code langout);
}

#endif
