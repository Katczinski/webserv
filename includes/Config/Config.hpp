#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Server.hpp"
#include "parser.hpp"
#include "Location.hpp"
#include <iostream>
#include <vector>
#include <map>

namespace ft
{
    class Location;
    class Config {
    private:
        std::string                         _host;
        std::string                         _port;
        std::string                         _server_name;
        std::string                         _root;
        std::map<int, std::string>          _error_pages;
        std::map<std::string, Location>     _locations;
    public:
        Config();
        Config(const Config& other);
        ~Config();
        Config& operator=(const Config& other);
    };
}

#endif