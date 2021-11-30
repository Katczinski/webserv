#include "Config.hpp"

// Default Constructor
ft::Config::Config() : _id(0), _listen(0), _server_name(0), _defaultServer(false) {}

// Copy Constructor
ft::Config::Config(const Config& other) {
	*this = other;
}

ft::Config::~Config() {}

ft::Config& ft::Config::operator=(const Config& other) {
	if (this != &other) {
		_id = other._id;
		_listen = other._listen;
		_server_name = other._server_name;
		_defaultServer = other._defaultServer;
	}
	return *this;
}


