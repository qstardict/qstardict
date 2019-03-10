#include <fstream>

#include "str.hh"

#include "file_map.hh"

using namespace mt;

file_map::file_map(const std::string& path)
{
    std::ifstream is(path.c_str());
    std::string line;
    while (mt::getline(is,line))
    {
	std::string::size_type pos1 = line.find(";");
	if (pos1 == std::string::npos) continue;
	std::string::size_type pos2 = line.find(";",pos1+1);
	if (pos2 == std::string::npos) continue;
	std::string::size_type pos3 = line.find(";",pos2+1);
	if (pos3 == std::string::npos) continue;

	std::string number(mt::trim(line.substr(0,pos1)));
	full_names_[number]=mt::trim(line.substr(pos1+1,pos2-pos1-1));
	short_names_[number]=mt::trim(line.substr(pos2+1,pos3-pos2-1));
    }
}
    
std::string file_map::short_name(const std::string& index) const
{
    map_type::const_iterator it = short_names_.find(index);
    return (it == short_names_.end())?"":it->second;
}

std::string file_map::full_name(const std::string& index) const
{
    map_type::const_iterator it = full_names_.find(index);
    return (it == full_names_.end())?"":it->second;
}

std::string file_map::any_name(const std::string& index) const
{
    std::string sname = short_name(index);
    return (sname.empty())?full_name(index):sname;
}
