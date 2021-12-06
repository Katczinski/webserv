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

namespace ft
{
    class CGI
    {
        private:
            std::map<std::string, const char*>  _env;
            char*                               _data;
        public:
            CGI();
            ~CGI();
            void                                init_env();
            void                                CGI_read(long length);
            std::map<std::string, std::string>  parseQString(const char *qstring);

    };
}

#endif 