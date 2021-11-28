#pragma once
#ifndef CLIENT_H__
#define CLIENT_H__

#include <fcntl.h>

namespace ft
{
    class Client
    {
        private:
            int             _fd;
        public:
            Client(int fd);
            int             getFd();
    };
}

#endif