#ifndef PARSER_HPP
#define PARSER_HPP

#include "Config.hpp"
#include "Server.hpp"
#include <iostream>
#include <algorithm>
#include <vector>

namespace ft
{
    class Config;
    typedef  std::vector<std::string>::iterator str_iter;
    typedef std::vector<std::string>::const_iterator const_str_iter;

	std::vector<ft::Config> parser(char* path);
    std::vector<ft::Config> parseServer(std::vector<std::string>& content);
    void split(std::vector<std::string>& content, const std::string& line, char c);
    void checkContent(const std::vector<std::string>&);
    void checkValidPort(std::string& port);
    std::vector<std::string> checkMultiplePort(str_iter begin, const std::vector<std::string>& content);
    void lineJoin(std::string& line);
    void split(std::vector<std::string>& content, const std::string& line, char c);
    int readFile(std::vector<std::string>& content, char* path);
    std::vector<std::string> checkMultiplePort(std::vector<std::string>& content);
	class ParserException : public std::exception {
    private:
        std::string _message;
    public:
        ParserException(std::string message) throw() ;
        const char* what(void) const throw();
        virtual ~ParserException() throw();
    };
}

#endif