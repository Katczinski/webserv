#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <map>
#include <iostream>
namespace ft
{
    class Response
    {
    public:
        std::map<std::string, std::string> full_log;
        Response();
        ~Response(){};
        void clear();
    };

}
bool ft_http_req(ft::Response& req, char* buf, int fd, bool flag);

#endif