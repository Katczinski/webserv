#pragma once
#ifndef SIMPLESOCKET_H__
#define SIMPLESOCKET_H__

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

namespace ft
{
    class SimpleSocket
    {
        private:
            int                 _sock;
            int                 _connection;
            struct sockaddr_in  _address;
        public:
            SimpleSocket(int domain, int service, int protocol, int port, u_long interface);
            virtual int         connect_to_network(int sock, struct sockaddr_in address) = 0;
            virtual void        test_connection(int);
            struct sockaddr_in  get_address();
            int                 get_sock();
            int                 get_connection();
            void                set_connection(int domain);
    };
}

#endif