#include "Server.hpp"

#define NUM_FDS 10

int ret = 0;
ft::Server::Server()
{
    try
    {
        _socket.listenSocket(ip, port);
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
    ft::Response req;
    char buff[30000];
    bool flag = false;
    // fcntl(fd, F_SETFL, O_NONBLOCK);
    while((ret = recv(fd, buff, 30000, 0)) > 0)
    {
        buff[ret] = '\0';
        if(ret == 2 && !req.full_log["ZAPROS"].size())
            break;
        else
            ft_http_req(req, buff, fd, flag);
        if(req.full_log["Connection"] == "close" && req.full_log["ZAPROS"] == "")
            return 0;
    }
    std::cout << "RET " << ret << std::endl;
    if(ret <= 0)
        return 0;
    return (ret);
}

// void        ft::Server::respond(int fd)
// {
//     char *hello = strdup("HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!");
//     write(fd, hello, strlen(hello));
//     free(hello);
// }

// const std::vector<ft::Client> &ft::Server::getClient() const
// {
//     return (_client);
// }

int ft::Server::newConnection()
{
    sockaddr_in addr = _socket.getAddress();
	socklen_t size = sizeof(sockaddr_in);
	try {
	    int sock =_socket.acceptSocket(&addr, &size);
        // fcntl(sock, F_SETFL, O_NONBLOCK);
        // _client.push_back(Client(sock));
        return (sock);
	}catch (std::exception &e){
		std::cerr << e.what()<<std::endl;
	}
    return (-1);
}

// void        ft::Server::run()
// {
//     std::vector<struct pollfd>  pollfds;
//     struct pollfd               listener;
//     listener.fd = _server;
//     listener.events = POLLIN;
//     pollfds.push_back(listener);
//     for (;;)
//     {
//         if (poll((pollfd *)&pollfds[0], pollfds.size(), -1) <= 0)
//         {
//             perror("Poll error");
//             exit(EXIT_FAILURE);
//         }
//         std::vector<pollfd>::iterator it = pollfds.begin();
//         std::vector<pollfd>::iterator end = pollfds.end();
//         for (; it != end; it++)
//         {
//             if (it->fd <= 0)
//                 continue;
//             else if (it->revents & POLLIN)
//             {
//                 if (it->fd == _server)
//                 {
//                     int new_fd = newConnection();
//                     struct pollfd new_client;
//                     new_client.fd = new_fd;
//                     new_client.events = POLLIN;
//                     new_client.revents = 0;
//                     pollfds.push_back(new_client);
//                     std::cout << "New connection on FD " << new_fd << std::endl;
//                 }
//                 else
//                 {
//                     if (!receive(it->fd))
//                     {
//                         std::cout << it->fd << " closed connection\n";
//                         if ((close(it->fd)) == -1)
//                         {
//                             std::cout << strerror(errno) << std::endl;
//                             exit(EXIT_FAILURE);
//                         }
//                         pollfds.erase(it);

//                     }
//                     // else
//                     //     respond(it->fd);
//                 }
//             }
//             else if (it->revents & POLLERR)
//             {
//                 if (it->fd == _server)
//                 {
//                     perror("Listening socket error");
//                     exit(EXIT_FAILURE);
//                 }
//                 else
//                 {
//                     it = pollfds.erase(it);
//                     close(it->fd);
//                 }

//             }
//         }
//     }
// }

const int            ft::Server::getServer() const
{
    return (_server);
}

ft::Socket      ft::Server::getSocket()
{
    return (_socket);
}
// void        ft::Server::run()
// {

//     int                 efd;
//     struct epoll_event  ev, ep_event[MAX_EVENTS];
//     efd = epoll_create1(0);
//     ev.events = EPOLLIN | EPOLLOUT;
//     ev.data.fd = _server;
    
//     epoll_ctl(efd, EPOLL_CTL_ADD, _server, &ev);

//     int nfds = 0;

//     for (;;)
//     {
//         nfds = epoll_wait(efd, ep_event, MAX_EVENTS, -1);
//         for (int i = 0; i < nfds; i++)
//         {
//             if (ep_event[i].data.fd == _server)
//             {
//                 int fd_new = newConnection();
//                 std::cout << "New connection on fd " << fd_new << std::endl;
//                 ev.events = EPOLLIN;
//                 ev.data.fd = fd_new;
//                 epoll_ctl(efd, EPOLL_CTL_ADD, fd_new, &ev);

//             }
//             else
//             {
//                 if (!receive(ep_event[i].data.fd)) // чтение
//                 {
//                     std::cout << ep_event[i].data.fd << " closed by client\n";
//                     epoll_ctl(efd, EPOLL_CTL_DEL, ep_event[i].data.fd, &ev);
//                     close(ep_event[i].data.fd);
//                 }
//                 // respond(ep_event[i].data.fd); // ответ
//             }
//         }
//     }
// }