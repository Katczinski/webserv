#include "Location.hpp"

typedef std::vector<std::string>::iterator str_iter;

// Default Constructor
ft::Location::Location() : _root(), _index(), _allowed_methods(), _cgi_extension(), _cgi_path(), _max_body() {}

ft::Location::Location(str_iter begin, std::vector<std::string>& content) : _root(), _index(), _allowed_methods(),
																			_cgi_extension(), _cgi_path(), _max_body() {
	for (str_iter it = begin; it != content.end() && *it != "}"; ++it) {
		if (*it == "root") {
			setRoot(it, content);
		}
		if (*it == "index") {
			setIndex(it, content);
		}
		if (*it == "allow_method") {
			setMethods(it, content);
		}
		if (*it == "cgi_extension") {
			setCgiExtension(it, content);
		}
		if (*it == "cgi_path") {
			setCgiPath(it, content);
		}
		if (*it == "client_max_body_size") {
			setMaxBody(it ,content);
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
		_index.push_back(*it);
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