#include "WebServer.hpp"
#include <fcntl.h>
ft::WebServer::WebServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10)
{
    launch();
}

void    ft::WebServer::accepter()
{
    int server = get_socket()->get_sock();
    _max = server;
    // loop looking for connections / data to handle
    for(;;) {
        FD_ZERO(&_fds);
        FD_SET(server, &_fds);
        if (server >= _max) _max = server + 1;

        for (std::vector<int>::iterator it = _connected.begin(); it != _connected.end(); ++it) {
            FD_SET(*it, &_fds);
            if (*it >= _max) _max = *it + 1;
        }
        if (select(_max, &_fds, NULL, NULL, NULL) > 0) {
            // something is readable
            struct sockaddr_in  addr = get_socket()->get_address();
            int addrlen = sizeof(addr);
            if (FD_ISSET(server, &_fds)) {
                // it's the listener
                _connected.push_back(accept(server, (struct sockaddr *)&addr, (socklen_t*)&addrlen));
                std::cout << "HERE: " << *(_connected.end() - 1) << std::endl;
            }
            for (std::vector<int>::iterator it = _connected.begin(); it != _connected.end(); ++it) {
                if (FD_ISSET(*it, &_fds)) {
                    // handle data on this connection
                    fcntl(*it, F_SETFD, O_NONBLOCK);
                    read(*it, _buffer, 30000);
                    handler();
                    _new_socket = *it;
                    responder();
                    FD_CLR(*it, &_fds);
                }
            }
        }
    }
}

void    ft::WebServer::handler()
{
    // read(get_socket()->get_sock(), _buffer, 30000);
    std::cout << _buffer << std::endl;
}

void    ft::WebServer::responder()
{
    char *hello = strdup("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
    write(_new_socket, hello, strlen(hello));
    // close(_new_socket);
    free(hello);
}

void    ft::WebServer::launch()
{

        accepter();
        // handler();
        // responder();

}