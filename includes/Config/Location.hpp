#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Server.hpp"
#include "Config.hpp"
#include "parser.hpp"
#include <vector>
#include <map>
#include <utility>
#include <memory>
#include <functional>

namespace ft
{
	class Location {
	private:
		std::string							_root;
		std::vector<std::string>			_index;
		std::vector<std::string>			_allowed_methods;
		std::string							_cgi_extension;
		std::string							_cgi_path;
		std::string							_max_body;
	public:
		Location(void);
		Location(const Location& other);
		~Location(void);
		Location& operator=(const Location& other);

	};
}



#endif