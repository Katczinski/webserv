#ifndef SERVER_H__
#define SERVER_H__

#include "Config.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <vector>

namespace ft
{
    class Server
    {
        private:
            ft::Socket                  _socket;
            int                         _server;
            std::vector<Client>         _client;
        public:
            Server();
            void                        run();
            int                         receive(int fd);
            void                        respond(int fd);
            int                         newConnection();
            void                        closeConnection(int client_index);
            const std::vector<Client>   &getClient() const;
    };
}


#endif