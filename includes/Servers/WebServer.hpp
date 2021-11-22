#ifndef WEBSERVER_H__
#define WEBSERVER_H__

#include "SimpleServer.hpp"
#include <unistd.h>
namespace ft
{
    class WebServer : public SimpleServer
    {
        private:
            char        _buffer[30000];
            int         _new_socket;
            void        accepter();
            void        handler();
            void        responder();
        public:
            WebServer();
            void        launch();
    };
}


#endif