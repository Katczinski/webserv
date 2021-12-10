#include "Config.hpp"

// Default Constructor
ft::Config::Config() : _host(), _port(), _server_name(), _root(), _index(), _error_pages(), _locations() {}

ft::Config::Config(str_iter& begin, std::vector<std::string>& content) : _host(), _port(), _server_name(),
																		_root(), _index(), _error_pages(), _locations() {
	for (; begin != content.end() - 1 && *begin != "server"; ++begin) {
		if (*begin == "listen") {
			if (!_port.empty()) {
				throw ft::ParserException("Parser Error: host is duplicated");
			}
			setPort(begin, content);
		}
		if (*begin == "host") {
			if (!_host.empty()) {
				throw ft::ParserException("Parser Error: port is duplicated");
			}
			setHost(begin, content);
		}
		if (*begin == "server_name") {
			if (!_server_name.empty()) {
				throw ft::ParserException("Parser Error: server_name is duplicated");
			}
			setServName(begin, content);
		}
		if ((*begin == "index")) {
			if (!_index.empty()) {
				throw ft::ParserException("Parser Error: index is duplicated");
			}
			setIndex(begin, content);
		}
		if ((*begin == "root"))  {
			if (!_root.empty()) {
				throw ft::ParserException("Parser Error: root is duplicated");
			}
			setRoot(begin, content);
		}
		if (*begin == "error_page") {
			setErrPages(begin, content);
		}
		if (*begin == "location") {
			if (*(begin + 2) != "{") {
				throw ft::ParserException("Parser Error: expected '{'");
			}
			setLocation(begin, content, _root);
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
		_index = other._index;
	}
	return *this;
}

std::string const ft::Config::getHost(void) const {
	return this->_host;
}

// ft::Config::portIterator		ft::Config::portBegin(void){
// 	return this->_port.begin();
// }

// ft::Config::portIterator		ft::Config::portEnd(void){
// 	return this->_port.end();
// }

std::string const ft::Config::getPort(void) const {
	return this->_port;
}

std::string const ft::Config::getServName(void) const {
	return this->_server_name;
}

std::string const ft::Config::getRoot(void) const {
	return this->_root;
}

std::vector<std::string> const ft::Config::getIndex(void) const {
	return this->_index;
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

void ft::Config::setHost(str_iter& begin, std::vector<std::string>& content) {
	std::string value = *(++begin);
	if (value == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 1) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
	_host = value;
	if (_host != "localhost") {
		size_t count = 0;
		// example .0.0.1  ||  127.0.0.
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
		if (count != 3) {
			throw ft::ParserException("Parser Error: host in config file is incorrect");
		}
	}
}

void ft::Config::setPort(std::string port) {
	this->_port = port;
}

void ft::Config::setPort(str_iter& begin, std::vector<std::string>& content) {
	size_t count = 0;
	str_iter it = ++begin;
	if (*it == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	while (*it != ";") {
		++count;
		++it;
	}
	std::string val = *begin;
	if (count != 1) {
		// if there is no ',' at the end of the port
		if (val[val.length() - 1] != ',') {
			throw ft::ParserException("!Parser Error: port in config file is incorrect");
		}
		// all ports, except last
		val = val.substr(0, val.length() - 1);
	}
	for (std::string::iterator iter = val.begin(); iter != val.end(); ++iter) {
		if (!std::isdigit(*iter)) {
			throw ft::ParserException("Parser Error: port in config file is incorrect");
		}
	}
	_port = val;
}

void ft::Config::setServName(str_iter& begin, std::vector<std::string>& content) {
	_server_name = *(++begin);
	if (_server_name == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 1) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
}

void ft::Config::setRoot(str_iter& begin, std::vector<std::string>& content) {
	char dir[1024];
	getcwd(dir, 1024);

	std::string value = *(++begin);
	if (value == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 1) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
	_root = dir + value;
}

void ft::Config::setIndex(str_iter& begin, std::vector<std::string>& content) {
	str_iter it = ++begin;
	if (*it == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	while (*it != ";") {
		_index.push_back(_root + *it);
		++it;
	}
}

void ft::Config::setErrPages(str_iter& begin, std::vector<std::string>& content) {
	if (*(begin + 1) == ";" || *(begin + 2) == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	iter_int_str it = _error_pages.find(atoi((*(++begin)).c_str()));
	if (*(begin + 2) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
	if (it == _error_pages.end()) {
		_error_pages[std::atoi((*(begin)).c_str())] = _root + *(begin + 1);
	}
}

void ft::Config::setLocation(str_iter& begin, std::vector<std::string>& content, std::string server_root) {
	std::string key = *(++begin);
	ft:Location newLocation(begin, content, server_root);
	_locations.insert(std::make_pair(key, newLocation));
}
