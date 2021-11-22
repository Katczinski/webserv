#pragma once
#ifndef LISTENINGSOCKET_H__
#define LISTENINGSOCKET_H__

#include "BindingSocket.hpp"

namespace ft
{
    class ListeningSocket : public BindingSocket
    {
        private:
            int                 _backlog;
            int                 _listening;
        public:
            ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog);

            void                start_listening();
    };
}

#endif