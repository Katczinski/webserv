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
#include "Location.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
namespace ft
{
    class Location;
    class Config;
    class Response
    {
    private:
       
    public:
        std::string full_buffer; // это полный текст всех хэдеров, он меняется очень много раз
        std::map<std::string, std::string> full_log; // фул лог, тут хэдеры
        bool is_content_length; // если есть Content-length и нет chunked
        bool is_chunked; // Content-length: chunked
        bool is_multy; // Content-type: multipary/*
        bool is_body;
        bool is_redir;
        bool is_delete; // Если метод DELETE
        size_t body_length; // если есть Content-length в запросе и ОТСУТСВУЕТ chunked (is_chunked = false). При чанкеде вручную body-length взять надо будет, this->full_log["Body"].size();
        bool is_body_left;
		Location* current_location;
    	std::ostringstream body; 
		size_t range;
        // get/set
        // void setFullBuffer(std::string& str,  bool clear);
        // std::string const getFullBuffer() const;
        // void setFullLog(std::string& key, std::string& val, bool append);
        // std::map<std::string, std::string> const getFullLog() const;
        // void setIsContentLength(bool set);
        // bool const getIsContentLength() const;
        // void setIsChunked(bool set);
        // bool const getIsChunked() const;
        // void setIsMulty(bool set);
        // bool const getIsMulty() const;
        // void setIsBody(bool set);
        // bool const getIsBody() const;
        // void setIsAutoIndex(bool set);
        // bool const getIsAutoIndex() const;
        // void setBodyLength(size_t length);
        // size_t const getBodyLength() const;
        // методы
        Response();
        ~Response();
        Response(Response const& other);
        Response& operator=(Response const& other); // конец Coplien формы
        void clear(); // очищает хедеры запроса текущего
        bool answer(int i, int fd,  ft::Config& conf); // тут куются ответы, скорее всего переделаю
        bool general_header_check(std::string str, int fd, ft::Config& conf); // проверка главного хэдера
        int req_methods_settings(std::vector<std::string> str); // проверка на то, какой метод пришел и что я могу с этим сделать
        void AutoIndexPage(ft::Config& conf); // неработающий автоиндекс
        bool post_download_request(ft::Config& config);
        std::string status(int code); // в аргумент передается код ошибки, возвращается название ошибки
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
bool    http_header(ft::Response& req, std::string buf1, int fd,  ft::Config& conf);
void    ft_split(std::string const &str, const char delim, std::vector<std::string> &out);
size_t  ft_hex_to_dec(std::string& str); // 16 -> 10
#endif