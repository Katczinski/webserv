#pragma once
#ifndef CGI_H__
#define CGI_H__

// #include "Server.hpp"
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "Server.hpp"

namespace ft
{
    class Response;
    class CGI
    {
        private:
            std::map<std::string, std::string>  _env;
            char**                              _cenv;
            std::string                         _data;
            char**                              _argv;
            std::string                         getExt(const std::string& path, char delim);
            std::string                         getHost(const std::string& path);
        public:
            CGI(ft::Response& req);
            ~CGI();
            void                                init_env(ft::Response& req);
            void                                CGI_read();
            void                                CGI_write(const std::string& body);

            void                                parseQString(const char *qstring);
            std::string                         execute(ft::Response& req);

    };
}

#endif 