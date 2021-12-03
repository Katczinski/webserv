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

		std::string const getRoot(void) const;
		std::vector<std::string> const getIndex(void) const;
		std::vector<std::string> const getMethods(void) const;
		std::string const getCgiExtension(void) const;
		std::string const getCgiPath(void) const;
		std::string const getMaxBody(void) const;

		void setRoot(const std::string& root);
        void setIndex(const std::string& index);
		void setMethods(const std::string& method);
		void setCgiExtension(const std::string& CgiExtension);
		void setCgiPath(const std::string& CgiPath);
		void setMaxBody(const std::string& MaxBody);

	};
}



#endif