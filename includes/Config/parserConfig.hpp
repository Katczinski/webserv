#ifndef PARSERCONFIG_HPP
#define PARSERCONFIG_HPP

#include <fstream>
#include "Config.hpp"

namespace ft
{
	void parserConfig(char* path);
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