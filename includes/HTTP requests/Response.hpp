#pragma once
#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <map>
#include <iostream>
#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include "Server.hpp"
#include <ctime>
#include <sstream>
namespace ft
{
    class Response
    {
    // private:
        // std::string current_server;
        // std::string current_methods;
    public:
        std::string full_buffer;
        std::map<std::string, std::string> full_log;
        bool is_content_length;
        size_t body_length;
        Response();
        ~Response(){};
        void clear();
        bool answer(int i, int fd);
        bool general_header_check(std::vector<std::string>& header, int fd);
        // size_t get_length(int fd);

    };
    template<typename T>
    std::string to_string(const T& value)
    {
    	std::ostringstream oss;
    	oss << value;
    	return oss.str();
    }
}
bool http_header(ft::Response& req, std::string buf1, int fd);
void ft_split(std::string const &str, const char delim, std::vector<std::string> &out);

#endif