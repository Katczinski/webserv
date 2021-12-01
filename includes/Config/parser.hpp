#ifndef PARSER_HPP
#define PARSER_HPP

#include "Config.hpp"
#include <iostream>

namespace ft
{
	void parser(char* path);
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