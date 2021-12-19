#pragma once
#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <map>
#include <iostream>
#include <fstream>
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
    private:

    public:
        std::string full_buffer; // это полный текст всех хэдеров, он меняется очень много раз
        std::map<std::string, std::string> full_log; // фул лог, смотри конструктор по умолчанию
        bool is_content_length; // если есть Content-length и нет chunked
        bool is_chunked; // Content-length: chunked
        bool is_multy; // Content-type: multipary/*
        bool is_body;
        size_t body_length; // если есть Content-length в запросе и ОТСУТСВУЕТ chunked (is_chunked = false). При чанкеде вручную body-length взять надо будет, this->full_log["Body"].size();
        // методы
        Response();
        ~Response();
        Response(Response const& other);
        Response& operator=(Response const& other); // конец Coplien формы
        void clear(); // очищает хедеры запроса текущего
        bool answer(int i, int fd,  ft::Config& conf); // тут куются ответы, скорее всего переделаю
        bool general_header_check(std::string str, int fd, ft::Config& conf); // проверка главного хэдера
        int req_methods_settings(std::vector<std::string> str); // проверка на то, какой метод пришел и что я могу с этим сделать
        std::string AutoIndexPage(ft::Config& conf, std::ostringstream& body); // неработающий автоиндекс
        bool post_request(ft::Config& config);
    };
    template<typename T>
    std::string to_string(const T& value) // что либо в строку
    {
    	std::ostringstream oss;
    	oss << value;
    	return oss.str();
    }
    template<typename T>
    size_t ft_atoi(T& str)
    {
        size_t i = 0;
        std::stringstream ss;
        ss << str;
        ss >> i;
        return i;
    }
}
bool http_header(ft::Response& req, std::string buf1, int fd,  ft::Config& conf);
void ft_split(std::string const &str, const char delim, std::vector<std::string> &out);
int  ft_hex_to_dec(std::string str); // 16 -> 10
#endif