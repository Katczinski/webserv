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
        std::map<std::string, std::string>  _error_pages;
        std::map<std::string, Location>     _locations;
    public:
        Config();
        Config(const Config& other);
        ~Config();
        Config& operator=(const Config& other);

        std::string const getHost(void) const;
        std::string const getPort(void) const;
        std::string const getServName(void) const;
        std::string const getRoot(void) const;
        std::map<std::string, std::string> const getErrPages(void) const;
        void setHost(const std::string& host);
        void setPort(const std::string& port);
        void setServName(const std::string& servName);
        void setRoot(const std::string& root);
        void setErrPages(const std::string& key, const std::string& value);
    };
}

#endif