#include "Cluster.hpp"

ft::Cluster::Cluster() : _connected(NULL), _size(0) {}

int        ft::Cluster::receive(int fd, ft::Response& req)
{
    char buff[30000] = {0};
    bool flag = false;
    int ret = recv(fd, buff, 30000, 0);
    buff[ret] = '\0';
    // проверка на \r\n\r\n
    if(ret <= 0)
        return 0;
    if(!req.full_buffer[fd].size() && strcmp(buff, "\r\n"))
    {
        std::cout << "IMN HERE =========================" << std::endl;
        req.full_buffer[fd]+=buff;
    }
    else if(req.full_buffer[fd].size())
        req.full_buffer[fd]+=buff;
    if(req.full_buffer[fd].find("\r\n\r\n") != std::string::npos)
    {
        ft_http_req(req, req.full_buffer[fd], fd, flag);
        req.full_buffer.erase(fd);
        if(!req.full_log["Connection"].compare(0, 5, "close"))
        {
            req.clear();
            return 0;
        }
        req.clear();
    }
    return (ret);
}

int         ft::Cluster::is_listening(int fd)
{
    for (int i = 0; i < _servers.size(); i++)
    {
        if (fd == _servers[i].getServer())
            return (i);
    }
    return (-1);
}

void        ft::Cluster::push_poll(int fd)
{
    if (!_connected)
    {
        _connected = (struct pollfd*)malloc(sizeof(struct pollfd));
        _connected[_size].events = POLLIN;
        _connected[_size].fd = fd;
        _connected[_size++].revents = 0;
        
    }
    else
    {
        _connected = (struct pollfd*)realloc(_connected, (_size + 1) * sizeof(struct pollfd));
        _connected[_size].events = POLLIN;
        _connected[_size].fd = fd;
        _connected[_size].revents = 0;
        _size++;
    }
}

void        ft::Cluster::erase_poll(int index)
{
    while (index < _size)
    {
        _connected[index].fd = _connected[index + 1].fd;
        _connected[index].events = _connected[index + 1].events;
        _connected[index].revents = _connected[index + 1].revents;
        index++;
    }
    _size--;
    // _connected = (struct pollfd*)realloc(_connected, (_size) * sizeof(struct pollfd));

}

void        ft::Cluster::push_back(const ft::Server& server)
{
    push_poll(server.getServer());
    _servers.push_back(server);
}

void        ft::Cluster::run()
{
    ft::Response req;
    for (;;)
    {
        if ((poll(_connected, _size, 2)) <= 0)
            continue ;
        //loop through all the connections
        for (int i = 0; i < _size; i++)
        {
            //check if event is registered
            if (_connected[i].revents & POLLIN)
            {
                //check if it's registered on the one of the listening sockets
                int l = is_listening(_connected[i].fd);
                if (l >= 0)
                {
                    int new_fd = _servers[l].newConnection();
                    push_poll(new_fd);
                    std::cout << "New connection on FD " << new_fd << std::endl;
                }
                else
                {
                    if (!receive(_connected[i].fd, req))
                    {
                        std::cout << "Connection " << _connected[i].fd << " closed\n";
                        close(_connected[i].fd);
                        erase_poll(i);
                    }
                }
            }
        }
    }
}