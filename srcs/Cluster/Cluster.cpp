#include "Cluster.hpp"
#include "Response.hpp"
ft::Cluster::Cluster() : _connected(NULL), _size(0) {}

int        ft::Cluster::receive(int fd, std::map<size_t, ft::Response>& all_connection)
{
    char buff[30000] = {0};
    int ret = recv(fd, buff,  30000, 0);
    buff[ret] = '\0';
    if(ret <= 0)
        return 0;
    if((!all_connection[fd].full_buffer.size() && strcmp(buff, "\r\n")) || all_connection[fd].is_content_length || all_connection[fd].is_chunked) // поменять на нижний иф запись при чанкеде и размере
    {
        all_connection[fd].full_buffer+=buff;
        if(!all_connection[fd].general_header_check(fd))
            return (1);
    }
    else if(all_connection[fd].full_buffer.size())
        all_connection[fd].full_buffer+=buff;
    if(all_connection[fd].full_buffer.find("\r\n\r\n") != std::string::npos && !all_connection[fd].is_content_length && !all_connection[fd].is_chunked)
    {
        std::cout << "Here=========================================\n " << all_connection[fd].full_buffer << std::endl;
        if(!http_header(all_connection[fd], all_connection[fd].full_buffer, fd))
        {
            all_connection[fd].clear();
            all_connection[fd].full_buffer.clear();
            return ret;
        }
        all_connection[fd].full_buffer.clear();
    }
    else if(all_connection[fd].is_content_length)
    {
        all_connection[fd].full_log["Body"] += all_connection[fd].full_buffer;
        std::string tmp;
        if(all_connection[fd].full_buffer.size() > all_connection[fd].body_length)
        {
            tmp = all_connection[fd].full_buffer.substr(0, all_connection[fd].body_length);
            all_connection[fd].full_buffer = all_connection[fd].full_buffer.substr(all_connection[fd].body_length, all_connection[fd].full_buffer.size()) += "\r\n\r\n";
            std::cout << "Here buff_size " << all_connection[fd].full_buffer.size() << " cont_length" << all_connection[fd].body_length << std::endl;
        }
        if(all_connection[fd].full_buffer.size() == all_connection[fd].body_length)
        {
            //вот тут функция на body; body лежит в tmp
            all_connection[fd].is_content_length = false;
            all_connection[fd].answer(200,fd);
            all_connection[fd].clear();
            if(all_connection[fd].full_buffer.compare(all_connection[fd].full_log["Body"]))
                all_connection[fd].full_buffer.clear();
            return(1);
        }
        if(tmp.size() == all_connection[fd].body_length)
            all_connection[fd].is_content_length = false;
    }
    else if(all_connection[fd].is_chunked)
    {
        //исполняется пока не будет chunked == 0\r\n
        all_connection[fd].full_log["Body"] += all_connection[fd].full_buffer;
        // исполнение?????
        std::cout << "Here " << all_connection[fd].full_log["Body"] << std::endl;
        if(all_connection[fd].full_log["Body"].find("0\r\n\r\n") != std::string::npos)
        {
            std::cout << "Heree " << std::endl;
            all_connection[fd].is_chunked = false;
            all_connection[fd].full_buffer.clear();
        }
    }
    if(all_connection[fd].full_log["Host"].size() &&  !all_connection[fd].is_content_length && !all_connection[fd].is_chunked)
    {   
        all_connection[fd].answer(200, fd);
        all_connection[fd].full_log.clear();
        if(all_connection[fd].full_buffer.size())
            http_header(all_connection[fd], all_connection[fd].full_buffer, fd);
        all_connection[fd].full_buffer.clear();
        return (1);
    }
    return(ret);
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
    std::map<size_t, ft::Response> all_connection;
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
                    if (!receive(_connected[i].fd, all_connection))
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