#include "WebServer.hpp"

ft::WebServer::WebServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10)
{
    launch();
}

void    ft::WebServer::accepter()
{
    struct sockaddr_in  address = get_socket()->get_address();
    int addrlen = sizeof(address);
    _new_socket = accept(get_socket()->get_sock(), (struct sockaddr*)&address, (socklen_t*)&addrlen);
    read(_new_socket, _buffer, 30000);
}

void    ft::WebServer::handler()
{
    std::cout << "Here: ";
    std::cout << _buffer << std::endl;
}

void    ft::WebServer::responder()
{
    char *hello = strdup("Hello from server");
    write(_new_socket, hello, strlen(hello));
    close(_new_socket);
    free(hello);
}

void    ft::WebServer::launch()
{
    while (true)
    {
        std::cout << "===== WAITING =====\n";
        accepter();
        handler();
        responder();
        std::cout << "====== DONE ======\n";
    }
}