#ifndef PARSER_HPP
#define PARSER_HPP

#include "Config.hpp"
#include "Server.hpp"
#include <iostream>
#include <algorithm>

namespace ft
{
    class Config;

	std::vector<ft::Config> parser(char* path);
    std::vector<ft::Config> parseServer(std::vector<std::string>& content);
    void checkContent(const std::vector<std::string>&);
    void lineJoin(std::string& line);
    void split(std::vector<std::string>& content, const std::string& line, char c);
    int readFile(std::vector<std::string>& content, char* path);
    void parseListen(std::string& value, ft::Config& config);
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