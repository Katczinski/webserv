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
        std::string                                 _host;
        std::string                                 _port;
        std::string                                 _server_name;
        std::string                                 _root;
        std::vector<std::string>                    _index;
        std::map<int, std::string>                  _error_pages;
        std::map<std::string, Location>             _locations;
        // typedef std::vector<std::string>::iterator  portIterator;
        typedef std::vector<std::string>::iterator str_iter;
        typedef std::map<int, std::string>::iterator iter_int_str;
    public:
        Config();
        Config(str_iter& begin, std::vector<std::string>& content);
        Config(const Config& other);
        ~Config();
        Config& operator=(const Config& other);

        std::string const getHost(void) const;
        std::string const getPort(void) const;
        // portIterator 	               portBegin(void);
        // portIterator 	               portEnd(void);
        std::string const getServName(void) const;
        std::string const getRoot(void) const;
        std::vector<std::string> const getIndex(void) const;
        std::string const getErrPages(int key) const;
        std::map<std::string, ft::Location> getLocation(void) const;
        std::map<std::string, ft::Location>::iterator findKeyLocation(std::string key);

        void setHost(str_iter& begin, std::vector<std::string>& content);
        void setPort(str_iter& begin, std::vector<std::string>& content);
        void setPort(std::string port);
        void setServName(str_iter& begin, std::vector<std::string>& content);
        void setRoot(str_iter& begin, std::vector<std::string>& content);
        void setIndex(str_iter& begin, std::vector<std::string>& content);
        void setErrPages(str_iter& ibegin, std::vector<std::string>& content);
        void setLocation(str_iter& begin, std::vector<std::string>& content, std::string server_root);
    };
}

#endif