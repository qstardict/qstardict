/* singleton.hh
 * This file is part of libmtquery library
 * Copyright (c) 2004 by Stanislav Ievlev
 *
 * This file is covered by the GNU Library General Public License,
 * which should be included with libmtquery as the file COPYING.
 */

#ifndef MT_SINGLETON_HH__
#define MT_SINGLETON_HH__

#include <string>
#include <map>
#include "shared_ptr.hh"

namespace mt
{

    template <typename ClassT,typename KeyClassT = std::string>
    class singleton_array
    { /**< simple singleton for array of the objects */
	typedef shared_ptr<ClassT> class_ptr;
	typedef std::map<KeyClassT,class_ptr > map_type;
	
        public:
	    static ClassT& instance(const KeyClassT& key)
	    {
		static map_type objs_;
		if (!objs_[key].get()) objs_[key] = new ClassT(key);
		return *objs_[key].get();
	    }
    
	private:
	//all this operations are unavailable to ordinal users
	    singleton_array();
	    singleton_array(const singleton_array& s);
	    singleton_array& operator=(const singleton_array& s);
	    ~singleton_array();
    };
    
}

#endif
