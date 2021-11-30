#include "parserConfig.hpp"

ft::ParserException::ParserException(std::string message) throw() : _message(message) {}

const char* ft::ParserException::what(void) const throw() {
	return _message.c_str();
}

ft::ParserException::~ParserException() throw() {}


void ft::parserConfig(char* path) {
	std::string filename;
    if (!path) {
        filename = "srcs/Config/file/default.conf";
    } else {
        filename = path;
    }
	std::ifstream file(filename.c_str());
	std::string line;
	if (!file) {
		throw ft::ParserException("Parser Error: could not open file");
	}
	// while(std::getline(file, line)) {
		
	// }
}




