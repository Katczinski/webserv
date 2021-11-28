#include "Client.hpp"

ft::Client::Client(int fd) : _fd(fd)
{
    fcntl(_fd, F_SETFL, O_NONBLOCK);
}

int         ft::Client::getFd() { return (_fd); }