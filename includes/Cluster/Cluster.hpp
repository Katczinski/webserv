#pragma once
#ifndef CLUSTER_H__
#define CLUSTER_H__

#include "Server.hpp"

namespace ft
{
    class Cluster
    {
        private:
            std::vector<ft::Server>     _servers;
            struct pollfd               *_connected;
            int                         _size;
        public:
            Cluster();
            void                        push_back(const ft::Server& server);
            void                        erase_poll(int index);
            void                        push_poll(int fd);
            void                        run();
            int                         receive(int fd, ft::Response& req);
            int                         is_listening(int fd);
    };
}

#endif