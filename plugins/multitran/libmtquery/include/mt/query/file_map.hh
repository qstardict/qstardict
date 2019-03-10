#ifndef MT_FILEMAP_HH__
#define MT_FILEMAP_HH__

#include <map>
#include <string>

#include "mtquery_dll.hh"

namespace mt
{

/**  simple interface to multitran databases in ".txt" format, e.g., subjects txt */
class file_map
{
    typedef std::map<std::string,std::string> map_type;
    public:
	LIBMTQUERY_API file_map(const std::string& path);
	LIBMTQUERY_API ~file_map() {}
	LIBMTQUERY_API std::string short_name(const std::string& index) const;
	/**< return short term name */
	LIBMTQUERY_API std::string full_name(const std::string& index) const;
	/**< return full term name */
	LIBMTQUERY_API std::string any_name(const std::string& index) const;
	/**< return short term name or full if short name is undefined */
	private:
	    map_type	short_names_;
	    map_type	full_names_;
};

}

#endif
