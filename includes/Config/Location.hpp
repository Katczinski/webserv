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

typedef std::vector<std::string>::iterator str_iter;

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
		Location(str_iter begin, std::vector<std::string>& content);
		Location(const Location& other);
		~Location(void);
		Location& operator=(const Location& other);

		std::string const getRoot(void) const;
		std::vector<std::string> const getIndex(void) const;
		std::vector<std::string> getMethods(void) const;
		std::string const getCgiExtension(void) const;
		std::string const getCgiPath(void) const;
		std::string const getMaxBody(void) const;

		void setRoot(str_iter iter, std::vector<std::string>& content);
        void setIndex(str_iter iter, std::vector<std::string>& content);
		void setMethods(str_iter iter, std::vector<std::string>& content);
		void setCgiExtension(str_iter iter, std::vector<std::string>& content);
		void setCgiPath(str_iter iter, std::vector<std::string>& content);
		void setMaxBody(str_iter iter, std::vector<std::string>& content);

	};
}



#endif