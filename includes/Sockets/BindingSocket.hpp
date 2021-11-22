#pragma once
#ifndef BINDINGSOCKET_H__
#define BINDINGSOCKET_H__

#include "SimpleSocket.hpp"

namespace ft
{
    class BindingSocket : public SimpleSocket
    {
        public:
            BindingSocket(int domain, int service, int protocol, int port, u_long interface);

            int         connect_to_network(int sock, struct sockaddr_in address);
    };
}


#endif