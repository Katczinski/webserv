#include "Config.hpp"

// Default Constructor
ft::Config::Config() : _host(), _port(), _server_name(), _root(), _error_pages(), _locations() {}

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


