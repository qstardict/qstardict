/* siktable.cc
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#include <fstream>
#include <stdexcept>

#include "lgk.hh"
#include "siktable.hh"

using namespace mt;

namespace
{
    const int class_mask =  0xffff >> 1; //CLEAR_END_OF_CLASS
    enum sik_codes
    {
	max_end = 1<<14,
	prich_start = max_end | 1, //start of the prich classes 
	go_next = max_end | 2, //go to the next class //SIK_NEXT_CLASS
	skip = max_end | 3, //no such form //SIK_NO_END
	same = max_end | 4, // same form //SIK_SAME_END
	geborren = max_end | 5, //unused //SIK_GEBORREN
	finish = 1<<15//end of class marker //SIK_MAX_END_NUMBER
    };

    /**
     * convert lgk code into number of special sik
     */
    //temporary removed language mention, all code must be language indepented
    //else if (lgk == mt::adjective || (lgk == mt::adverb && m_lang == mt::russian))
    int lgk_to_sik(int lgk)
    {
	int sik = -1;
        lgk = mt::speech_part(lgk);

        if (lgk == sparts::noun || lgk == sparts::numeral || lgk == sparts::pronoun)
	    sik = 700;
	else if (lgk == sparts::verb)
	    sik = 705;
        else if (mt::is_prich(lgk) || lgk == sparts::adjective || lgk == sparts::adverb)
	    sik = 710;

	return sik;
    }
    
}

siktable::siktable(const std::string& path):
    max_ends_(0)
{
    std::ifstream is(path.c_str(),std::ios_base::in|std::ios_base::binary);
    if (!is)
	throw std::runtime_error("siktable: unable to open "+path);

    is.read(reinterpret_cast<char*>(&max_ends_),sizeof(max_ends_));
    
    //read rest data to the vector
    data_.assign(std::istreambuf_iterator<char>(is.rdbuf()),
		 std::istreambuf_iterator<char>());
}

const siktable::sik_type *siktable::sik(int num) const
{
    if (num >= (max_ends_ -1) || num < 0)
	return 0;
    const sik_type *start = reinterpret_cast<const sik_type *>(&data_[0]);
    sik_type pos = start[num];
    if (!pos || static_cast<unsigned>(pos) > data_.size())
	return 0;
    return start+pos;
}

bool siktable::check_form(int lgk,int formnum) const
{
  const sik_type *masks = sik(lgk_to_sik(lgk));
  if (masks)
  {
    sik_type andmask = masks[(formnum+1)*2];
    sik_type value = masks[(formnum + 1) * 2 + 1] & class_mask;
    return (lgk & andmask & 0x1f)  == value;
  }
  return true;
}


bool siktable::check(int siknum,int endnum, int lgk) const
{
    bool in_prich = false;
    int formnum = 0; //current form number
    const sik_type *ends=sik(siknum);
    if (!ends) return false;

    while(true)
    {
	sik_type curr_end = *ends & class_mask;
	if (curr_end < max_end)
	{
	    if ((in_prich && check(curr_end,endnum,sparts::adjective|mt::adj_bits::has_short)) ||
	       (curr_end == endnum && check_form(lgk,formnum)))
		return true;
	    ++formnum;
	}
	else
	{
	    switch (curr_end)
	    {
		case prich_start:
		    in_prich = true;
		    break;
		case go_next:
		    if (check(*(ends+1)&class_mask,endnum,lgk))
			return true;
		    ends++;//totally will be +=2
		    break;;
		case skip:
		    ++formnum;
		    break;
		case same:
		    --formnum;
		    break;
		case geborren:
		    break;
		default:
		    throw std::runtime_error("siktable: unknown control symbol");
	    }
	}
	//it's possible final element and we must process it
	if ((*ends & finish) == finish)
	    break;
	ends++;
    }
    return false;
}
