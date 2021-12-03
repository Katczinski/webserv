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

std::map<std::string, std::string> const ft::Config::getErrPages(void) const {
	return _error_pages;
}

void ft::Config::setHost(const std::string& host) {
	this->_host = host;
}

void ft::Config::setPort(const std::string& port) {
	this->_port = port;
}

void ft::Config::setServName(const std::string& servName) {
	this->_server_name = servName;
}

void ft::Config::setRoot(const std::string& root) {
	this->_root = root;
}

void ft::Config::setErrPages(const std::string& key, const std::string& value) {
	this->_error_pages[key] = value;
}


