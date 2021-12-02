#ifndef PARSER_HPP
#define PARSER_HPP

#include "Config.hpp"
#include "Server.hpp"
#include <iostream>
#include <vector>
namespace ft
{
    class Config;
	void parser(char* path, std::vector<ft::Config>& configs);
	std::string lineJoin(std::string& line);
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