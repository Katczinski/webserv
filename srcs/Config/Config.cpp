#include "Config.hpp"

// Default Constructor
ft::Config::Config() : _host(), _port(), _server_name(), _root(), _index(), _error_pages(), _locations() {}

ft::Config::Config(str_iter begin, std::vector<std::string>& content) : _host(), _port(), _server_name(),
																		_root(), _index(), _error_pages(), _locations() {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "listen") {
			if (!_host.empty()) {
				throw ft::ParserException("Parser Error: host is duplicated");
			}
			setHost(it, content);
		}
		if (*it == "port") {
			if (!_port.empty()) {
				throw ft::ParserException("Parser Error: port is duplicated");
			}
			setPort(it, content);
		}
		if (*it == "server_name") {
			if (!_server_name.empty()) {
				throw ft::ParserException("Parser Error: server_name is duplicated");
			}
			setServName(it, content);
		}
		if ((*it == "index")) {
			// if (!_index.empty()) {
			// 	throw ft::ParserException("Parser Error: index is duplicated");
			// }
			setIndex(it, content);
		}
		if ((*it == "root"))  {
			// if (!_root.empty()) {
			// 	throw ft::ParserException("Parser Error: root is duplicated");
			// }
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
		_index = other._index;
	}
	return *this;
}

std::string const ft::Config::getHost(void) const {
	return this->_host;
}

ft::Config::portIterator		ft::Config::portBegin(void){
	return this->_port.begin();
}

ft::Config::portIterator		ft::Config::portEnd(void){
	return this->_port.end();
}

std::vector<std::string> const ft::Config::getPort(void) const {
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

void ft::Config::setHost(str_iter begin, std::vector<std::string>& content) {
	std::string value = *(begin + 1);
	if (value == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 2) != ";") {
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
void ft::Config::setPort(str_iter begin, std::vector<std::string>& content) {
	str_iter it = begin + 1;
	if (*it == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	while (*it != ";") {
		_port.push_back(*it);
		++it;
	}
	for (std::vector<std::string>::iterator it = _port.begin(); it != _port.end(); ++it) {
		// example 8080, 8090;
		if (_port.size() > 1) {
			// if there is no ',' at the end of the port
			if ((*it)[(*it).length() - 1] != ',' && it != _port.end() - 1) {
				throw ft::ParserException("Parser Error: port in config file is incorrect");
			}
			// all ports, except last
			if (it != _port.end() - 1) {
				*it = (*it).substr(0, (*it).size() - 1);
			}
		}
		for (std::string::iterator iter = (*it).begin(); iter != (*it).end(); ++iter) {
			if (!std::isdigit(*iter)) {
				throw ft::ParserException("Parser Error: port in config file is incorrect");
			}
		}
	}
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
	char dir[1024];
	getcwd(dir, 1024);

	std::string value = *(begin + 1);
	if (value == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 2) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
	_root = dir + value;
}

void ft::Config::setIndex(str_iter begin, std::vector<std::string>& content) {
	str_iter it = begin + 1;
	if (*it == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	while (*it != ";") {
		_index.push_back(_root + *it);
		++it;
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
		_error_pages[std::atoi((*(begin + 1)).c_str())] = _root + *(begin + 2);
	}
}

void ft::Config::setLocation(str_iter begin, std::vector<std::string>& content) {
	ft:Location newLocation(begin, content);
	_locations.insert(std::make_pair(*(begin + 1), newLocation));
}
