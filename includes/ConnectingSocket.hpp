#ifndef CONNECTINGSOCKET_H__
#define CONNECTINGSOCKET_H__

#include "SimpleSocket.hpp"

namespace ft
{
    class ConnectingSocket : public SimpleSocket
    {
        public:
            ConnectingSocket(int domain = AF_INET, int service = SOCK_STREAM,
                        int protocol = 0, int port = 8080, u_long interface = INADDR_ANY);

            int         connect_to_network(int sock, struct sockaddr_in address);
    };
}
#endif