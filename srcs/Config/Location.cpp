#include "Location.hpp"

typedef std::vector<std::string>::iterator str_iter;

// Default Constructor
ft::Location::Location() : _root(), _index(), _allowed_methods(), _cgi_extension(),
							_cgi_path(), _max_body(), _autoindex(false) {}

ft::Location::Location(str_iter& begin, std::vector<std::string>& content) : _root(), _index(), _allowed_methods(), _cgi_path(),
																			_cgi_extension(), _max_body(), _autoindex(false) {
	for (; begin != content.end() && *begin != "}"; ++begin) {
		if (*begin == "root") {
			setRoot(begin, content);
		}
		if (*begin == "index") {
			setIndex(begin, content);
		}
		if (*begin == "allow_method") {
			setMethods(begin, content);
		}
		if (*begin == "cgi_extension") {
			setCgiExtension(begin, content);
		}
		if (*begin == "cgi_path") {
			setCgiPath(begin, content);
		}
		if (*begin == "autoindex") {
			setAutoindex(begin, content);
		}
		if (*begin == "max_body_size") {
			setMaxBody(begin ,content);
		}
	}
}

// Copy Constructor
ft::Location::Location(const ft::Location& other) {
	*this = other;
}

// Destructor
ft::Location::~Location() {}

// Assign
ft::Location& ft::Location::operator=(const ft::Location& other) {
	if (this != &other) {
		_root = other._root;
		_index = other._index;
		_allowed_methods = other._allowed_methods;
		_cgi_extension = other._cgi_extension;
		_cgi_path = other._cgi_path;
		_max_body = other._max_body;
	}
	return *this;
}

std::string const ft::Location::getRoot(void) const {
	return this->_root;
}

std::vector<std::string> const ft::Location::getIndex(void) const {
	return this->_index;
}

std::vector<std::string> ft::Location::getMethods(void) const {
	return this->_allowed_methods;
}

std::string const ft::Location::getCgiExtension(void) const {
	return this->_cgi_extension;
}
std::string const ft::Location::getCgiPath(void) const {
	return this->_cgi_path;
}

std::string const ft::Location::getMaxBody(void) const {
	return this->_max_body;
}

bool const ft::Location::getAutoindex(void) const {
	return this->_autoindex;
}

void ft::Location::setRoot(str_iter begin, std::vector<std::string>& content) {
	if (*(begin + 1) == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 2) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
	_root= *(begin + 1);
}

void ft::Location::setIndex(str_iter begin, std::vector<std::string>& content) {
	str_iter it = begin + 1;
	if (*it == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	while (*it != ";") {
		_index.push_back(_root + *it);
		++it;
	}
}

void ft::Location::setMethods(str_iter begin, std::vector<std::string>& content) {
	str_iter it = begin + 1;
	if (*it == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	while (*it != ";") {
		_allowed_methods.push_back(*it);
		++it;
	}
}

void ft::Location::setCgiExtension(str_iter begin, std::vector<std::string>& content) {
	if (*(begin + 1) == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 2) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
	_cgi_extension = *(begin + 1);
}

void ft::Location::setCgiPath(str_iter begin, std::vector<std::string>& content) {
	if (*(begin + 1) == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 2) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
	_cgi_path = *(begin + 1);
}

void ft::Location::setMaxBody(str_iter begin, std::vector<std::string>& content) {
	if (*(begin + 1) == ";") {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 2) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
	_max_body = *(begin + 1);
}

void ft::Location::setAutoindex(str_iter begin, std::vector<std::string>& content) {
	std::string value = *(begin + 1);
	if (value == ";" || (value != "on" && value != "off")) {
		throw ft::ParserException("Parser Error: bad config file");
	}
	if (*(begin + 2) != ";") {
		throw ft::ParserException("Parser Error: expected ';'");
	}
	if (value == "on") {
		_autoindex = true;
	}
}