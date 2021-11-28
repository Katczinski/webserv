#include "Server.hpp"

#define MAX_EVENTS 10

ft::Server::Server()
{
    try
    {
        _socket.listenSocket();
        _server = _socket.getSock();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what()<<std::endl;
		exit(1);
    }
}
void        ft::Server::closeConnection(int client_index)
{
    close(_client[client_index].getFd());
    std::cout << "erasing index " << client_index << std::endl;
    _client.erase(_client.begin() + client_index);
}

int        ft::Server::receive(int fd)
{
    char buff[30000];
    int ret = recv(fd, buff, 30000, 0);
    // if (ret == 0){
    //     closeConnection(client_index);
    //     return (0);
    // }
    std::cout << buff << std::endl;
    return (ret);
}

void        ft::Server::respond(int fd)
{
    char *hello = strdup("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
    write(fd, hello, strlen(hello));
    free(hello);
}

const std::vector<ft::Client> &ft::Server::getClient() const
{
    return (_client);
}

int ft::Server::newConnection()
{
    sockaddr_in addr = _socket.getAddress();
	socklen_t size = sizeof(sockaddr_in);
	try {
	    int sock =_socket.acceptSocket(&addr, &size);
        // _client.push_back(Client(sock));
        return (sock);
	}catch (std::exception &e){
		std::cerr << e.what()<<std::endl;
	}
    return (-1);
}

void        ft::Server::run()
{
    int                 efd;
    struct epoll_event  ev, ep_event[MAX_EVENTS];
    efd = epoll_create1(0);
    ev.events = EPOLLIN | EPOLLOUT;
    ev.data.fd = _server;
    
    epoll_ctl(efd, EPOLL_CTL_ADD, _server, &ev);

    int nfds = 0;

    for (;;)
    {
        nfds = epoll_wait(efd, ep_event, MAX_EVENTS, -1);
        for (int i = 0; i < nfds; i++)
        {
            if (ep_event[i].data.fd == _server)
            {
                int fd_new = newConnection();
                std::cout << "New connection on fd " << fd_new << std::endl;
                ev.events = EPOLLIN;
                ev.data.fd = fd_new;
                epoll_ctl(efd, EPOLL_CTL_ADD, fd_new, &ev);

            }
            else
            {
                if (!receive(ep_event[i].data.fd))
                {
                    std::cout << ep_event[i].data.fd << " closed by client\n";
                    epoll_ctl(efd, EPOLL_CTL_DEL, ep_event[i].data.fd, &ev);
                    close(ep_event[i].data.fd);
                }
                respond(ep_event[i].data.fd);
            }
        }
    }
}
