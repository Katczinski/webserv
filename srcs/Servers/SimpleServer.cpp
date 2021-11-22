#include "SimpleServer.hpp"

ft::SimpleServer::SimpleServer(int domain, int service, int protocol, int port, u_long interface, int backlog)
{
    _socket = new ListeningSocket(domain, service, protocol, port, interface, backlog);
}

ft::ListeningSocket*        ft::SimpleServer::get_socket() { return (_socket); }