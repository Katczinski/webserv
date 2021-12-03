#include "Location.hpp"

// Default Constructor
ft::Location::Location() : _root(), _index(), _allowed_methods(), _cgi_extension(), _cgi_path(), _max_body() {}

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

void ft::Location::setRoot(const std::string& root) {
	this->_root = root;
}

void ft::Location::setIndex(const std::string& index) {
	this->_index.push_back(index);
}

void ft::Location::setMethods(const std::string& method) {
	this->_allowed_methods.push_back(method);
}

void ft::Location::setCgiExtension(const std::string& CgiExtension) {
	this->_cgi_extension = CgiExtension;

}

void ft::Location::setCgiPath(const std::string& CgiPath) {
	this->_cgi_path = CgiPath;
}

void ft::Location::setMaxBody(const std::string& MaxBody) {
	this->_max_body = MaxBody;
}