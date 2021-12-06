#include "Config.hpp"

// Default Constructor
ft::Config::Config() : _host(), _port(), _server_name(), _root(), _error_pages(), _locations() {}

ft::Config::Config(str_iter begin, std::vector<std::string>& content) : _host(), _port(), _server_name(),
																		_root(), _error_pages(), _locations() {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "listen" && _host.empty() && _port.empty()) {
			setHostPort(it, content);
		}
		if (*it == "server_name" && _server_name.empty()) {
			setServName(it, content);
		}
		if ((*it == "root") && _root.empty()) {
			setRoot(it, content);
		}
		if (*it == "error_page") {
			setErrPages(it, content);
		}
		if (*it == "location") {
			if (*(it + 2) != "{") {
				throw ft::ParserException("Parser Error: expected '{'");
			}
			setLocation(it, content);
		}
	}
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
		throw ft::ParserException("Error: bad config file(error_page)");
	}
	return (*it).second;
}

std::map<std::string, ft::Location> ft::Config::getLocation(void) const {
	return this->_locations;
}

std::map<std::string, ft::Location>::iterator ft::Config::findKeyLocation(std::string key) {
	std::map<std::string, ft::Location>::iterator it = _locations.find(key);
	if (it == _locations.end()) {
		throw ft::ParserException("Error: path to location not found");
	}
	return it;
}

void ft::Config::checkHostPort(void) {
	size_t count = 0;
	if (_host != "localhost") {
		if (*_host.begin() == '.' || *(_host.end() - 1) == '.')  {
			throw ft::ParserException("Parser Error: host in config file is incorrect");
		}
		for (std::string::iterator it = _host.begin(); it != _host.end(); ++it) {
			if (*it == '.') {
				++count;
				if (!std::isdigit(*(it - 1))) {
					throw ft::ParserException("Parser Error: host in config file is incorrect");
				}
			}
			if (!std::isdigit(*it) && *it != '.') {
				throw ft::ParserException("Parser Error: host in config file is incorrect");
			}
		}
	}
	if (count != 3) {
		throw ft::ParserException("Parser Error: host in config file is incorrect");
	}
	for (std::string::iterator it = _port.begin(); it != _port.end(); ++it) {
		if (!std::isdigit(*it)) {
			throw ft::ParserException("Parser Error: port in config file is incorrect");
		}
	}

}

void ft::Config::setHostPort(str_iter begin, std::vector<std::string>& content) {
	std::string value = *(begin + 1);
	if (value == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 2) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
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
	ft::Config::checkHostPort();
}

void ft::Config::setServName(str_iter begin, std::vector<std::string>& content) {
	_server_name = *(begin + 1);
	if (_server_name == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 2) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
}

void ft::Config::setRoot(str_iter begin, std::vector<std::string>& content) {
	_root = *(begin + 1);
	if (_root == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 2) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
}

void ft::Config::setErrPages(str_iter begin, std::vector<std::string>& content) {
	if (*(begin + 1) == ";" || *(begin + 2) == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	iter_int_str it = _error_pages.find(atoi((*(begin + 1)).c_str()));
	if (*(begin + 3) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
	if (it == _error_pages.end()) {
		_error_pages[std::atoi((*(begin + 1)).c_str())] =  *(begin + 2);
	}
}

void ft::Config::setLocation(str_iter begin, std::vector<std::string>& content) {
	ft:Location newLocation(begin, content);
	_locations.insert(std::make_pair(*(begin + 1), newLocation));
}
