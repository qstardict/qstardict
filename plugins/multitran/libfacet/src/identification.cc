/* identification.cc
 * This file is part of libfacet library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libfacet as the file COPYING.
 */

#ifndef __linux__
#include <langinfo.h>
#include <locale.h>
#endif

#include "identification.hh"

using namespace mt;

std::locale::id	mt::identification::id;

identification::identification(const std::string& name,size_t i):
    std::locale::facet(i)
{
    if (name == "C" || name == "POSIX")
    {
        name_ = "english";
        codeset_ = "ISO-8859-1";
    }
    else
    {
#ifndef __linux__
        name_ = name.substr(0,name.find("_"));
#else
        setlocale(LC_ALL,name.c_str());
        name_ = nl_langinfo(_NL_IDENTIFICATION_LANGUAGE);
        codeset_ = nl_langinfo(CODESET);
        setlocale(LC_IDENTIFICATION,"C"); //return back to C locale 
#endif
    }
    if (name_ == "ru")
      name_ = "rus";
    if (name == "en")
      name_ = "eng";
    name_[0] = static_cast<char>(tolower(name_[0]));//lowercase for the first letter
}

const std::string& identification::str() const
{
    return name_;
}

const std::string& identification::codeset() const
{
    return codeset_;
}
