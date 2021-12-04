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
    typedef  std::vector<std::string>::iterator iter;

	std::vector<ft::Config> parser(char* path);
    std::vector<ft::Config> parseServer(std::vector<std::string>& content);
    void checkContent(const std::vector<std::string>&);
    void lineJoin(std::string& line);
    void split(std::vector<std::string>& content, const std::string& line, char c);
    int readFile(std::vector<std::string>& content, char* path);
	class ParserException : public std::exception {
    private:
        std::string _message;
    public:
        ParserException(std::string message) throw();
        const char* what(void) const throw();
        virtual ~ParserException() throw();
    };
}

#endif