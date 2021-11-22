#ifndef SIMPLESERVER_H__
#define SIMPLESERVER_H__

#include "websockets.hpp"

namespace ft
{
    class SimpleServer
    {
        private:
            ListeningSocket         *_socket;
            virtual void            accepter() = 0;
            virtual void            handler() = 0;
            virtual void            responder() = 0;
        public:
            SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog);
            virtual void            launch() = 0;
            ListeningSocket*        get_socket();
    };
}

#endif