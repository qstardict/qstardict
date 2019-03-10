/* lgk.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#include "lgk.hh"

using namespace mt;

int mt::speech_part(int lgk)
{
    lgk &= 0xff;
    if (!lgk) return 0;

    if (lgk <= sparts::predicative)
	return lgk;

    int n = 0;

    while (!(lgk&0x80)) { lgk<<=1; ++n; }
    return 1<<(8-n-1);
}

bool mt::is_perfect(int lgk)
{
    if (!is_verb(lgk)) return false;
    return ((lgk & (verb_bits::perfect|verb_bits::imperfect)) ==  verb_bits::perfect);
}
