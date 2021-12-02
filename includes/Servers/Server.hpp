#ifndef SERVER_H__
#define SERVER_H__

#include "Config.hpp"
#include "parser.hpp"
#include "Socket.hpp"
#include "Client.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <vector>
#include "Response.hpp"

namespace ft
{
    class Server
    {
        private:
            ft::Socket                              _socket;
            int                                     _server;
            typedef std::vector<pollfd>::iterator   iterator;

        public:
            Server(std::string ip, std::string port);
            // void                            run();
            // int                             receive(int fd);
            // void                            respond(int fd);
            int                             newConnection();
            void                            closeConnection(int client_index);
            const std::vector<Client>       &getClient() const;
            const int                       getServer() const;
            std::vector<pollfd>             getPollFds();
            ft::Socket                      getSocket();

    };
}


#endif