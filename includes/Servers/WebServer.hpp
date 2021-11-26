#ifndef WEBSERVER_H__
#define WEBSERVER_H__

#include "SimpleServer.hpp"
#include "Config.hpp"
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <vector>
namespace ft
{
    class WebServer : public SimpleServer
    {
        private:
            char                _buffer[30000];
            fd_set              _fds;
            int                 _max;
            std::vector<int>    _connected;
            int                 _new_socket;
            void                accepter();
            void                handler();
            void                responder();
        public:
            WebServer();
            void                launch();
            void                build_select_list();
            void                read_socks();
    };
}


#endif