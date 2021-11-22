#pragma once
#ifndef CONNECTINGSOCKET_H__
#define CONNECTINGSOCKET_H__

#include "SimpleSocket.hpp"

namespace ft
{
    class ConnectingSocket : public SimpleSocket
    {
        public:
            ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);

            int         connect_to_network(int sock, struct sockaddr_in address);
    };
}
#endif