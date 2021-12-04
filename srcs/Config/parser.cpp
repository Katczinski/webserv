#include "parser.hpp"

typedef  std::vector<std::string>::iterator str_iter;
typedef  std::map<std::string, ft::Location>::iterator const_iter_str_loc;

ft::ParserException::ParserException(std::string message) throw() : _message(message) {}

const char* ft::ParserException::what(void) const throw() {
	return _message.c_str();
}

ft::ParserException::~ParserException() throw() {}

void ft::split(std::vector<std::string>& content, const std::string& line, char c) {
	size_t size = line.size();
	size_t start = 0;
	size_t end;
	while (start < size) {
		// skip whitespace and find the beginning of a word
		while (start < size && isspace(line[start])) {
			++start;
		}
		if (start == size) {
			break;
		}
		// find the end a word
		end = start + 1;
		while (end < size && !isspace(line[end])) {
			++end;
		}
		content.push_back(line.substr(start, end - start));
		start = end;
	}
}

void ft::lineJoin(std::string& line) {
	// find comments and erase
	int pos = line.find('#');
	if (pos != -1) {
		line.erase(pos);
	}
	//replace all \n and tabs with spaces
	for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
		if (*it == '\n') {
			line.replace(it, it + 1, " ");
		} else if (*it == '\t') {
			line.replace(it, it + 1, " ");
		//add whitespace before ';'
		} else if (*it == ';') {
			line.insert(it, ' ');
			++it;
		}
	}
}

int ft::readFile(std::vector<std::string>& content, char* path) {
	std::string line;
	std::string filename;
    if (!path) {
        filename = "srcs/Config/file/default.conf";
    } else {
        filename = path;
    }
	std::ifstream file(filename.c_str());
	if (!file) {
		return 1;
	}
	while(std::getline(file, line)) {
		ft::lineJoin(line);
		if (line != "") {
			ft::split(content, line, ' ');
		}
	}
	file.close();
	return 0;

}

void ft::checkContent(const std::vector<std::string>& content) {
	typedef std::vector<std::string>::const_iterator const_iter;
	if (std::count(content.begin(), content.end(), "}") - std::count(content.begin(), content.end(), "{")) {
		throw ft::ParserException("Parser Error: invalid number of brackets");
	}
	bool flag = false;
	for (const_iter it = content.begin(); it != content.end(); ++it) {
		if (*it == "server") {
			flag = true;
		}
	}
	if (!flag) {
		throw ft::ParserException("Parser Error: not a server");
	}
	if (content.front() != "server") {
		throw ft::ParserException("Parser Error: bad config file");
	}
}

std::vector<ft::Config> ft::parser(char* path) {
	std::vector<std::string> content;
	std::vector<ft::Config>configs;

	if (ft::readFile(content, path)) {
		throw ft::ParserException("Parser Error: could not open file");
	}
	ft::checkContent(content);

	for (str_iter it = content.begin(); it != content.end(); ++it) {
		if (*it == "server") {
			ft::Config newConfig(it, content);
			configs.push_back(newConfig);
		}
	}
	return configs;
}
