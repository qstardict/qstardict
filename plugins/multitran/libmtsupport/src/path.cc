/* path.cc
 * This file is part of libmtsupport library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libbtree as the file COPYING.
 */

#include "path.hh"

const std::string& mt::path_separator()
{
#ifdef WIN32
	static const std::string sep = "\\";
#else
	static const std::string sep = "/";
#endif
	return sep;
}
