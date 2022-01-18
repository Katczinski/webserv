#include "Cluster.hpp"
#include "Response.hpp"

ft::Cluster::Cluster() : _connected(NULL), _size(0), _capacity(0) {}
ft::Cluster::~Cluster() {}
ft::Cluster::Cluster(const ft::Cluster& other) { *this = other; }
ft::Cluster&            ft::Cluster::operator=(const ft::Cluster& other)
{
    _servers = other._servers;
    _configs = other._configs;
    _connected = other._connected;
    _size = other._size;
    _capacity = other._capacity;
    return *this;
}

int        ft::Cluster::receive(int fd, std::map<size_t, ft::Response>& all_connection, ft::Config& config)
{
    std::vector<char> buf1(4000900, 0);
    ssize_t ret = recv(fd, &buf1[0], buf1.size(), 0);
    if(ret < 0)
    {
		all_connection[fd].answer(500,fd,config);
		return 1;
	}
	else if(ret == 0)
	{
        if(!all_connection[fd].is_file_large && !all_connection[fd].is_body_left)
		{
            all_connection[fd].answer(408,fd,config);
            return 0;
        }
        return 1;
    }
    size_t i = 0;
    while((i < ret && all_connection[fd].full_log["Host"].empty()) || (i < ret && all_connection[fd].is_chunked)) // записываем если нет хэдеров
    {
        if(buf1[0] == '\r' && all_connection[fd].full_buffer.empty() && !all_connection[fd].is_chunked)
            i++;
        else
        {
            std::cout << buf1[i];
            all_connection[fd].full_buffer += buf1[i++];
            if(all_connection[fd].full_buffer.find("\r\n\r\n") != std::string::npos) // считываем хэдер пришедший, если еще не был в парсере
                break;
        }
    }
    if(!all_connection[fd].full_buffer.empty() && !all_connection[fd].general_header_check(all_connection[fd].full_buffer.substr(0, 2100), fd, config)) // смотрим general header, max размер URL 2048
        return (1);
    if(all_connection[fd].full_buffer.find("\r\n\r\n") != std::string::npos && !all_connection[fd].is_content_length && !all_connection[fd].is_chunked)
    {
        if(!http_header(all_connection[fd], all_connection[fd].full_buffer, fd, config)) // пармис хэдеры, в случаем ошибки уходим в if и чистим все
        {
            all_connection[fd].clear();
            all_connection[fd].full_buffer.clear();
            return ret;
        }
        while (i < ret) // если есть body - записываем
        {
            all_connection[fd].full_log["Body"] += buf1[i++];
        }
        all_connection[fd].full_buffer.clear(); // чистим пришедшее сообщение из буфера
    }
    if(!all_connection[fd].full_log["Host"].empty() &&  !all_connection[fd].is_content_length && !all_connection[fd].is_chunked &&
        !all_connection[fd].is_multy && !all_connection[fd].is_delete) // Ответ на get
    {
        if (all_connection[fd].full_log["Dirrectory"].find("/cgi-bin/") != std::string::npos) // CGI
        {
            ft::CGI cgi(all_connection[fd], config);
            cgi.execute(all_connection[fd], fd);
            // send(fd, response.c_str(), response.length(), 0);
        }
        else // любой другой запрос с нормальный полем Host
            all_connection[fd].answer(200, fd, config);
        if(!all_connection[fd].full_buffer.empty())
            http_header(all_connection[fd], all_connection[fd].full_buffer, fd, config);
        size_t ans = ((all_connection[fd].full_log["Connection"].find("close") != std::string::npos) ? 0 : 1);
        all_connection[fd].full_buffer.clear();
        if(!all_connection[fd].is_body_left)
            all_connection[fd].full_log.clear();
        return (ans);
    }
    else if (all_connection[fd].is_delete) // если удалить что-нибудь
    {
        if (!remove((config.getRoot() + all_connection[fd].full_log["Dirrectory"]).c_str()))
            all_connection[fd].answer(204,fd, config);
        else
            all_connection[fd].answer(404,fd, config);
        all_connection[fd].full_buffer.clear();
        all_connection[fd].clear();
    }
    else if(all_connection[fd].is_content_length) // если есть длинна тела и запрос POST
    {
        size_t j = 0;
        while (i == 0 &&  j < ret) // записываем body, если он приходит частями
        {
            all_connection[fd].full_log["Body"] += buf1[j];
            ++j;
        }
        if(all_connection[fd].full_log["Body"].size() > all_connection[fd].body_length) // если длинна тела выше, чем заявлена, то записываем все по длинне, остальное оставляем в буфере на следующий запрос
        {
            all_connection[fd].full_buffer = all_connection[fd].full_log["Body"].substr((all_connection[fd].full_log["Body"].size() - all_connection[fd].body_length), all_connection[fd].full_log["Body"].size());
            all_connection[fd].full_log["Body"] = all_connection[fd].full_log["Body"].substr(0, all_connection[fd].body_length);
        }
        if(all_connection[fd].full_log["Body"].size() == all_connection[fd].body_length) // выполняем действия с body
        {
            if(all_connection[fd].is_multy) // если загрузка файла на сервер через кнопку на главной
            {
                if(!all_connection[fd].post_download_request(config))
                    all_connection[fd].answer(400,fd, config);
            }
            if (all_connection[fd].full_log["Dirrectory"].find("/cgi-bin/") != std::string::npos) // CGI
            {
                ft::CGI cgi(all_connection[fd], config);
                cgi.execute(all_connection[fd], fd);
            }
            else
                all_connection[fd].answer(301,fd, config); // временный ответ-затычка
            size_t ans = ((all_connection[fd].full_log["Connection"].compare(0, 5, "close")) ? 0 : 1); // проверяем хэдер Connection: close
            all_connection[fd].clear();
            return(ans);
        }
    }
    else if(all_connection[fd].is_chunked) // протестил не всё
    {
        //исполняется пока не будет chunked == 0\r\n
        if(!all_connection[fd].body_length) // записываем размер чанка, если он не \r\n
        {
            std::string buffer;
            int i = 0;
            while(i < ret)
                buffer += buf1[i++];
            if(buffer[0] != '\r')
            {
                all_connection[fd].body_length = ft_hex_to_dec(buffer);
                all_connection[fd].full_buffer.clear();
            }
        }
        if(all_connection[fd].full_buffer.find("0\r\n\r\n") != std::string::npos || 
        (all_connection[fd].full_buffer.find("\r\n\r\n") != std::string::npos && !all_connection[fd].body_length)) // чекам на конец или двойной пропуск
        {
            all_connection[fd].answer(200,fd,config); // ответ заглушка
            all_connection[fd].full_buffer.clear();
            all_connection[fd].clear();
            return ret;
        }
        else if(all_connection[fd].body_length) // записываем BODY по RFC ограничивая Content-leght'ом
        {
            if(all_connection[fd].full_buffer.size() > all_connection[fd].body_length)
            {
                all_connection[fd].full_log["Body"] += all_connection[fd].full_buffer.substr(0, all_connection[fd].body_length);
                all_connection[fd].body_length = 0;
                all_connection[fd].full_buffer.clear();
            }
            else
            {
                all_connection[fd].full_log["Body"] += all_connection[fd].full_buffer;
                if(all_connection[fd].full_buffer.size() >= all_connection[fd].body_length)
                    all_connection[fd].body_length = 0;
                else
                    all_connection[fd].body_length -= all_connection[fd].full_buffer.size();
            }
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
        _connected[_size].events = POLLIN | POLLOUT;
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
        _connected[_size].events = POLLIN | POLLOUT;
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
	signal(SIGPIPE, SIG_IGN);
    
    std::vector<char> buffer(40000000);
    for (;;)
    {
        if ((poll(_connected, _size, -1)) == -1)
        {
			std::cerr << "POLL RETURNED ERROR" << std::endl;
			throw ProcessError();
		}
        //loop through all the connections
        for (int i = 0; i < _size; i++)
        {  
            //check if event is registered
            if (/*!all_connection[_connected[i].fd].is_body_left && !all_connection[_connected[i].fd].is_file_large &&*/ _connected[i].revents & POLLIN) //|| _connected[i].revents & POLLOUT) // смотрю если ли чтение/запись, я чет забыл почему именно так оставил, хотя ответ снизу
            {
                std::cout << "YA TUT " << std::endl;
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
                    //config_map[_connected[i].fd].getHost() - ключ =a фд, валью = конфиг
                    if (!receive(_connected[i].fd, all_connection, *config_map[_connected[i].fd])) // вот тут парсер
                    {
                        std::cout << "Connection " << _connected[i].fd << " closed by host\n";
                        config_map.erase(_connected[i].fd);
                        close(_connected[i].fd);
                        erase_poll(i);
                    }
					// close(_connected[i].fd);
                }
            }
			if(all_connection[_connected[i].fd].is_body_left && _connected[i].revents & POLLOUT)
            {
                int l = is_listening(_connected[i].fd);
                if (l >= 0)
                {
                    int new_fd = _servers[l].newConnection();
                    push_poll(new_fd);
                    config_map[new_fd] = &_configs[l];
                    std::cout << "New connection on FD " << new_fd << std::endl;
                }
                int how = 0;
				struct pollfd   pfd;
				std::string telo;
                long count = 0;
                if(!all_connection[_connected[i].fd].body.str().empty())
                    telo = all_connection[_connected[i].fd].body.str();
                if(all_connection[_connected[i].fd].is_file_large)
                {
                    all_connection[_connected[i].fd].input.read(&buffer[0], 40000001);
                    count = all_connection[_connected[i].fd].input.gcount();
                    if(count) 
                    {
                        long i = 0;
                        while(i < count)
                            telo += buffer[i++];
                    }
                    else
                        all_connection[_connected[i].fd].is_file_large = false;

                }
                while(!telo.empty())
                {
                    pfd.fd = _connected[i].fd;
                    pfd.events = 0 | POLLOUT;
                    how =  poll(&pfd, 1, -1);
                    if(how == 0)
                    {
                        std::cout << "TIMEOUT  CONNECTION FD: " << _connected[i].fd << std::endl;
                        break;
                    }
                    if(pfd.revents & POLLERR)
                    {
                        std::cout << "OSHIBKA POLA: " << _connected[i].fd << std::endl;
                        _connected[i].events = POLLIN;
                        all_connection[_connected[i].fd].is_body_left = false;
                        all_connection[_connected[i].fd].body.str("");
                        all_connection[_connected[i].fd].body.str().clear();
                        all_connection[_connected[i].fd].body.clear();
                        all_connection[_connected[i].fd].clear();
                        all_connection[_connected[i].fd].full_buffer.clear();
                        all_connection[_connected[i].fd].input.close();
                        all_connection[_connected[i].fd].answer(500, _connected[i].fd, *config_map[_connected[i].fd]);
                        config_map.erase(_connected[i].fd);
                        close(_connected[i].fd);
                        erase_poll(i);
                        //кинуть 500
                    }
                    how = send(_connected[i].fd, telo.c_str(), telo.size(), 0);
                    if(how <= 0)
                    {
                        // if(all_connection[_connected[i].fd].send_err)
                        // {
                            // all_connection[_connected[i].fd].body.str(telo);
                            // all_connection[_connected[i].fd].send_err = false;
                        // }
                        all_connection[_connected[i].fd].is_file_large = false;
                        std::cout << "SEND ERROR " << std::endl;
                        // if(how < 0 )
                            // all_connection[_connected[i].fd].answer(500, _connected[i].fd, *config_map[_connected[i].fd]);
                        // config_map.erase(_connected[i].fd);
                        // close(_connected[i].fd);
                        // erase_poll(i);
                        break;
                    }
                    else
                    {
                        telo.erase(0, how);
                        if(!all_connection[_connected[i].fd].body.str().empty())
                        {
                            all_connection[_connected[i].fd].body.str("");
                            all_connection[_connected[i].fd].body.str().clear();
                        }
                        std::cout << "HOW  " << how << std::endl;
                    }
                }
                if(!all_connection[_connected[i].fd].is_file_large)
                {
                    all_connection[_connected[i].fd].is_body_left = false;
				    all_connection[_connected[i].fd].input.close();
                    all_connection[_connected[i].fd].clear();
				    all_connection[_connected[i].fd].full_buffer.clear();
                    config_map.erase(_connected[i].fd);
				    close(_connected[i].fd);
                    erase_poll(i);
                }
				all_connection[_connected[i].fd].body.str("");
				all_connection[_connected[i].fd].body.str().clear();
				all_connection[_connected[i].fd].body.clear();
            }
        }
    }
}