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
            std::string                         _path;
            std::string                         getExt(const std::string& path, char delim);
            std::string                         getHost(const std::string& path);
        public:
            CGI(ft::Response& req, ft::Config& conf);
            ~CGI();
            void                                formHeader(std::string& header);
            void                                init_env(ft::Response& req);
            void                                CGI_read(long fd);
            void                                parseQString(const char *qstring);
            void                                execute(ft::Response& req, int fd);

    };
}

#endif 