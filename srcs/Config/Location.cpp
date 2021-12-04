#include "Location.hpp"

typedef std::vector<std::string>::iterator str_iter;

// Default Constructor
ft::Location::Location() : _root(), _index(), _allowed_methods(), _cgi_extension(), _cgi_path(), _max_body() {}

ft::Location::Location(str_iter begin, std::vector<std::string>& content) : _root(), _index(), _allowed_methods(),
															_cgi_extension(), _cgi_path(), _max_body() {

	setRoot(begin, content);
	setIndex(begin, content);
	setMethods(begin, content);
	setCgiExtension(begin, content);
	setCgiPath(begin, content);
	setMaxBody(begin ,content);
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

std::vector<std::string> const ft::Location::getMethods(void) const {
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

void ft::Location::setRoot(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "root") {
			std::string value = *(it + 1);
			_root = value;
			while (*it != ";") {
				++it;
			}
		}
	}
}

void ft::Location::setIndex(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "index") {
			++it;
			while (*it != ";") {
				_index.push_back(*it);
				++it;
			}
		}
	}
}

void ft::Location::setMethods(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "allow_method") {
			++it;
			while (*it != ";") {
				_allowed_methods.push_back(*it);
				++it;
			}
		}
	}
}

void ft::Location::setCgiExtension(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "cgi_extension") {
			std::string value = *(it + 1);
			_cgi_extension = value;
			while (*it != ";") {
				++it;
			}
		}
	}
}

void ft::Location::setCgiPath(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "cgi_path") {
			std::string value = *(it + 1);
			_cgi_path = value;
			while (*it != ";") {
				++it;
			}
		}
	}
}

void ft::Location::setMaxBody(str_iter begin, std::vector<std::string>& content) {
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "client_max_body_size") {
			std::string value = *(it + 1);
			_max_body = value;
			while (*it != ";") {
				++it;
			}
		}
	}
}