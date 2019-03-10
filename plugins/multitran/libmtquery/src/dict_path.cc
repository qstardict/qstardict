/* dict_path.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#include "dict_path.hh"
#include "path.hh"

using namespace mt;

std::string mt::dict_path(lang_code langin,lang_code langout)
{
    std::string str = std::min(short_lang_name(langin),short_lang_name(langout))+"_"+
		      std::max(short_lang_name(langin),short_lang_name(langout))+path_separator()+
		      "dict.";
    return str;
}
