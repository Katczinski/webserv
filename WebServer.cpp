#include "WebServer.hpp"

ft::WebServer::WebServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10)
{
    launch();
}

void    ft::WebServer::accepter()
{
    struct sockaddr_in  address = get_socket()->get_address();
    int addrlen = sizeof(address);
    _new_socket = accept(get_socket()->get_sock(), reinterpret_cast<struct sockaddr*>(&address), (socklen_t*)&addrlen);
    read(_new_socket, _buffer, 30000);
}

void    ft::WebServer::handler()
{
    std::cout << "Here: ";
    std::cout << _buffer << std::endl;
}

void    ft::WebServer::responder()
{
    char *hello = strdup("HTTP/1.1 400 OK\nContent-Type: text/plain\nContent-Length: 15\n\nHello world!123");
    write(_new_socket, hello, strlen(hello));
    close(_new_socket);
    free(hello);
}

void    ft::WebServer::launch()
{
    fd_set current_socket;
    struct sockaddr_in  address = get_socket()->get_address();
    int addrlen = sizeof(address);
    std::cout << "Here " << std::endl;

    if((_new_socket = accept(get_socket()->get_sock(), reinterpret_cast<struct sockaddr*>(&address), (socklen_t*)&addrlen)) < 0)
        std::cout << "Govno" << std::endl;;
    // accepter();
    int tmp = get_socket()->get_sock();
    int fd_mask = get_socket()->get_sock();
    while (true)
    {
        std::cout << "===== WAITING =====\n";
        int new_socket_client;
        FD_ZERO(&current_socket);
        FD_SET(tmp, &current_socket);
        if(select(fd_mask + 1, &current_socket, NULL, NULL, NULL) < 0){
            std::cout << "Select govno " << std::endl;
            return;
        }
        for (int i = 0; i < fd_mask; i++)
        {

            if(FD_ISSET(i, &current_socket))
            {
                if(i == tmp)
                {
                    new_socket_client = accept(get_socket()->get_sock(), reinterpret_cast<struct sockaddr*>(&address), (socklen_t*)&addrlen);
                    FD_SET(new_socket_client, &current_socket);
                }
                if(fd_mask < new_socket_client)
                    fd_mask = new_socket_client;
            }
            sleep(1);
        }
        read(_new_socket, _buffer, 30000); 
        

        handler();
        responder();
        std::cout << "====== DONE ======\n";
    }
}