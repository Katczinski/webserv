#include "Cluster.hpp"
#include "Response.hpp"

ft::Cluster::Cluster() : _connected(NULL), _size(0), _capacity(0) {}

int        ft::Cluster::receive(int fd, std::map<size_t, ft::Response>& all_connection, ft::Config& config, char* buff)
{
    // char buff[300001] = {0};
    int ret = recv(fd, buff,  11000000, 0);
    buff[ret] = '\0';
    if(ret <= 0)
        return 0;
    if((!all_connection[fd].full_buffer.size() && strcmp(buff, "\r\n")) && (!all_connection[fd].is_content_length && !all_connection[fd].is_chunked))
    {
        all_connection[fd].full_buffer+=buff;
        if(!all_connection[fd].general_header_check(fd, config))
            return (1);
    }
    else if(all_connection[fd].full_buffer.size() || all_connection[fd].is_content_length || all_connection[fd].is_chunked)
        all_connection[fd].full_buffer+=buff;
    if(all_connection[fd].full_buffer.find("\r\n\r\n") != std::string::npos && !all_connection[fd].is_content_length && !all_connection[fd].is_chunked)
    {
        std::cout <<  "BUFFER========================\n"<< all_connection[fd].full_buffer << std::endl;
        if(!http_header(all_connection[fd], all_connection[fd].full_buffer, fd, config))
        {            
            all_connection[fd].clear();
            all_connection[fd].full_buffer.clear();
            return ret;
        }
        all_connection[fd].full_buffer.clear();
    }
    if(all_connection[fd].full_log["Host"].size() &&  !all_connection[fd].is_content_length && !all_connection[fd].is_chunked && !all_connection[fd].is_multy)
    {
        int i = (all_connection[fd].full_log["Connection"].compare(0, 5, "close")) ? 1 : 0;
        // all_connection[fd].full_log["Dirrectory"] = "/cgi-bin/hello";
        std::cout << "=================" << all_connection[fd].full_log["Dirrectory"] << "===========\n";
        if (all_connection[fd].full_log["Dirrectory"].find("/cgi-bin/") != std::string::npos)
        {
            ft::CGI cgi(all_connection[fd]);
            std::string response = cgi.execute(all_connection[fd], fd);
            std::cout << "I'm here\n";
            // send(fd, response.c_str(), response.length(), 0);
        }
        else
            all_connection[fd].answer(200, fd, config);

        all_connection[fd].full_log.clear();
        if(all_connection[fd].full_buffer.size())
            http_header(all_connection[fd], all_connection[fd].full_buffer, fd, config);
        all_connection[fd].full_buffer.clear();
        return (i);
    }
    else if(all_connection[fd].is_content_length)
    {
        std::cout << "Я ТУТ № 1 =========================\n" <<" BODY_SIZEE " << all_connection[fd].full_log["Body"].size() << " BUFFER_SIZE " << all_connection[fd].full_buffer.size() << " BODY_L " << all_connection[fd].body_length << std::endl;
        if(all_connection[fd].full_buffer.size() >= all_connection[fd].body_length)
        {
            std::cout << "Я ТУТ № 2 =========================\n";

            all_connection[fd].full_log["Body"] = all_connection[fd].full_buffer.substr(0, all_connection[fd].body_length);
            all_connection[fd].full_buffer = all_connection[fd].full_buffer.substr(all_connection[fd].body_length, all_connection[fd].full_buffer.size()) += "\r\n\r\n";
        }
        if(all_connection[fd].full_log["Body"].size() == all_connection[fd].body_length)
        {
            std::cout << "--------------------------------------------------------------------------\n";
            if(all_connection[fd].is_multy)
            {
                std::cout << "Я ТУТ № 3 =========================\n";

                if(!all_connection[fd].post_request(config))
                    all_connection[fd].answer(400,fd, config);
            }
                int i = (all_connection[fd].full_log["Connection"].compare(0, 5, "close")) ? 1 : 0;
                //вот тут функция на body; body лежит в all_connection[fd].full_log["Body"]

                ft::CGI cgi(all_connection[fd]);
                std::string response = cgi.execute(all_connection[fd], fd);
                std::cout << "I'm there\n";

                // send(fd, response.c_str(), response.length(), 0);
                // all_connection[fd].answer(200,fd, config); // временный ответ-затычка
                all_connection[fd].clear();
                return(i);
        }
    }
    else if(all_connection[fd].is_chunked)
    {
        //исполняется пока не будет chunked == 0\r\n
        if(!all_connection[fd].body_length)
        {
            int i = 0;
            while(i < (strlen(buff) - 2))
            {
                if(!isdigit(buff[i]))
                {
                    all_connection[fd].answer(400, fd, config);
                    all_connection[fd].clear();
                    all_connection[fd].full_buffer.clear();
                    return ret;
                }
                i++;
            }
            all_connection[fd].body_length = ft_atoi(buff);
        }
        else if(all_connection[fd].body_length)
        {
            if(all_connection[fd].full_buffer.size() >= all_connection[fd].body_length)
            {
                all_connection[fd].full_log["Body"] += all_connection[fd].full_buffer.substr(0, all_connection[fd].body_length);
                all_connection[fd].body_length = 0;
                all_connection[fd].full_buffer.clear();
            }
        }
        if(all_connection[fd].full_buffer.find("0\r\n\r\n") != std::string::npos || 
        (all_connection[fd].full_buffer.find("\r\n\r\n") != std::string::npos && !all_connection[fd].body_length))
        {
            std::cout << "U HERE MY FRINED " << std::endl;
            if(!all_connection[fd].full_log["ZAPROS"].compare(0, 3, "GET"))
                all_connection[fd].answer(200, fd, config);
            else
                std::cout << "ISPOLNENIT POST ZAPROSA " << std::endl;  // исполнение тут, body =  all_connection[fd].full_log["Body"]
            all_connection[fd].full_buffer.clear();
            all_connection[fd].clear();
            return ret;
        }
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
        _connected[_size].revents = 0;
        _capacity = _size = 1;
    }
    else
    {
        if (_size >= _capacity)
        {
            _connected = (struct pollfd*)realloc(_connected, (_capacity * 2) * sizeof(struct pollfd));
            _capacity *= 2;
        }
        _connected[_size].events = POLLIN;
        _connected[_size].fd = fd;
        _connected[_size].revents = 0;
        _size++;
    }
}

void        ft::Cluster::erase_poll(int index)
{
    while (index < _size - 1)
    {
        _connected[index].fd = _connected[index + 1].fd;
        _connected[index].events = _connected[index + 1].events;
        _connected[index].revents = _connected[index + 1].revents;
        index++;
    }
    _size--;
    if (_size < _capacity / 4)
    {
        _connected = (struct pollfd*)realloc(_connected, (_capacity / 2) * sizeof(struct pollfd));
        _capacity /= 2;
    }
}

void        ft::Cluster::push_back(const ft::Server& server)
{
    push_poll(server.getServer());
    _servers.push_back(server);
}

void        ft::Cluster::setConfig(std::vector<ft::Config> configs)
{
    this->_configs = configs;
}

void        ft::Cluster::setup()
{
    for (std::vector<ft::Config>::iterator it = _configs.begin(); it != _configs.end(); it++)
    {
        push_back(ft::Server(it->getHost(), it->getPort()));
        std::cout << it->getHost() << ":" << it->getPort() << " is ready\n";
    }
}

void        ft::Cluster::run()
{
    std::map<size_t, ft::Response>  all_connection;
    std::map<int, ft::Config*>      config_map;
    char* buff = (char*)malloc(sizeof(char) * 11000000);
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
                    config_map[new_fd] = &_configs[l];
                    std::cout << "New connection on FD " << new_fd << std::endl;
                }
                else
                {
                    //config_map[_connected[i].fd] - ключ = фд, валью = конфиг
                    if (!receive(_connected[i].fd, all_connection, *config_map[_connected[i].fd], buff))
                    {
                        std::cout << "Connection " << _connected[i].fd << " closed\n";
                        config_map.erase(_connected[i].fd);
                        close(_connected[i].fd);
                        erase_poll(i);
                    }
                }
            }
        }
    }
}