#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "Server.hpp"
#include "parserConfig.hpp"
#include <iostream>

namespace ft
{
    class Config {
    private:
        int             _id;
        int             _listen;
        std::string     _server_name;
        bool            _defaultServer;
        //location?
    public:
        Config();
        Config(const Config& other);
        ~Config();
        Config& operator=(const Config& other);
    };
}

#endif