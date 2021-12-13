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
#include "Config.hpp"
#include <dirent.h>

namespace ft
{
    class Config;
    class Response
    {
    public:
        std::string full_buffer;
        std::map<std::string, std::string> full_log;
        std::string current_dirrectory;
        std::string prev_dirrectory;
        bool is_content_length;
        bool is_chunked;
        bool is_multy;
        size_t body_length;
        // методы
        Response();
        ~Response(){};
        void clear();
        bool answer(int i, int fd,  ft::Config& conf);
        bool general_header_check(int fd, ft::Config& conf);
        int req_methods_settings(std::vector<std::string> str);
        std::string AutoIndexPage(ft::Config& conf, std::ostringstream& body);
    };
    template<typename T>
    std::string to_string(const T& value)
    {
    	std::ostringstream oss;
    	oss << value;
    	return oss.str();
    }
}
bool http_header(ft::Response& req, std::string buf1, int fd,  ft::Config& conf);
void ft_split(std::string const &str, const char delim, std::vector<std::string> &out);
size_t ft_atoi(std::string& str);
size_t ft_atoi(char* str);

#endif