#include "parserConfig.hpp"

ft::ParserException::ParserException(std::string message) throw() : _message(message) {}

const char* ft::ParserException::what(void) const throw() {
	return _message.c_str();
}

ft::ParserException::~ParserException() throw() {}

std::string ft::lineJoin(std::string& line) {
	// find comments and erase
	int pos = line.find('#');
	if (pos != -1) {
		line.erase(pos);
	}
	// replace all \n and tabs with spaces
	for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
		if (*it == '\n') {
			line.replace(it, it + 1, " ");
		} else if (*it == '\t')
			line.replace(it, it + 1, " ");
	}
	return line;
}

void ft::parserConfig(char* path) {
	std::string filename;
    if (!path) {
        filename = "srcs/Config/file/default.conf";
    } else {
        filename = path;
    }
	std::ifstream file(filename.c_str());
	std::string line;
	std::vector<std::string> config;
	if (!file) {
		throw ft::ParserException("Parser Error: could not open file");
	}
	while(std::getline(file, line)) {
		ft::lineJoin(line);
		if (line != "") {
			config.push_back(line);
		}
	}
	std::vector<std::string>::iterator it;
	for (it = config.begin(); it != config.end(); ++it) {
		std::cout << *it;
	}
	file.close();
}




