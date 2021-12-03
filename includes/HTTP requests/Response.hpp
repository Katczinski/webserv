#pragma once
#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <map>
#include <iostream>
#include <sstream>
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
    template<typename T>
    std::string to_string(const T& value)
    {
    	std::ostringstream oss;
    	oss << value;
    	return oss.str();
    }
}
bool ft_http_req(ft::Response& req, char* buf, int fd, bool flag);

#endif