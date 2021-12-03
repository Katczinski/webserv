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