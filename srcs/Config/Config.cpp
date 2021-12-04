#include "Config.hpp"

typedef std::vector<std::string>::iterator str_iter;

// Default Constructor
ft::Config::Config() : _host(), _port(), _server_name(), _root(), _error_pages(), _locations() {}

ft::Config::Config(str_iter begin, std::vector<std::string>& content) :	_host(), _port(), _server_name(),
														_root(), _error_pages(), _locations() {
	setHostPort(begin, content);
	setServName(begin, content);
	setRoot(begin, content);
	setErrPages(begin, content);
	setLocation(begin, content);
}

// Copy Constructor
ft::Config::Config(const ft::Config& other) {
	*this = other;
}

// Destructor
ft::Config::~Config() {}

// Assign
ft::Config& ft::Config::operator=(const ft::Config& other) {
	if (this != &other) {
		_host = other._host;
		_port = other._port;
		_server_name = other._server_name;
		_root = other._root;
		_error_pages = other._error_pages;
		_locations = other._locations;
	}
	return *this;
}

std::string const ft::Config::getHost(void) const {
	return this->_host;
}

std::string const ft::Config::getPort(void) const {
	return this->_port;
}

std::string const ft::Config::getServName(void) const {
	return this->_server_name;
}

std::string const ft::Config::getRoot(void) const {
	return this->_root;
}

std::string const ft::Config::getErrPages(int key) const {
	std::map<int, std::string>::const_iterator it;
	it = _error_pages.find(key);
	if (it == _error_pages.end()) {
		return "";
	}
	return (*it).second;
}

std::map<std::string, ft::Location> const ft::Config::getLocation(void) const {
	return this->_locations;
}

std::map<std::string, ft::Location>::iterator ft::Config::findKeyLocation(std::string key) {
	return this->_locations.find(key);
}

void ft::Config::setHostPort(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "listen") {
			std::string value = *(it + 1);
			int delim = value.find(':');
			if (delim != -1) {
				this->_host = value.substr(0, delim);
				this->_port = value.substr(delim + 1, value.size());
			} else {
				delim = value.find('.');
				if (delim != -1) {
					this->_host = value;
					this->_port = "8080";
				} else {
					this->_host = "localhost";
					this->_port = value;
				}
			}
			while (*it != ";") {
				++it;
			}
		}
	}
}

void ft::Config::setServName(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "server_name") {
			// start = it;
			std::string value = *(it + 1);
			_server_name = value;
			while (*it != ";") {
				++it;
			}
		}
	}
}

void ft::Config::setRoot(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "root") {
			// start = it;
			std::string value = *(it + 1);
			_root = value;
			while (*it != ";") {
				++it;
			}
			break;
		}
	}
}

void ft::Config::setErrPages(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "error_page") {
			_error_pages[std::atoi((*(it + 1)).c_str())] =  *(it + 2);
		}
	}
}

void ft::Config::setLocation(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "location") {
			ft:Location newLocation(begin, content);
			_locations[*(it + 1)] = newLocation;
		}
	}
}
