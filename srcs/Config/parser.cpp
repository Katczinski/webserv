#include "parser.hpp"

typedef  std::vector<std::string>::iterator iter_str;
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

void ft::parseListen(std::string& value, ft::Config& config) {
	int delim = value.find(':');
	if (delim != -1) {
		config.setHost(value.substr(0, delim));
		config.setPort(value.substr(delim + 1, value.size()));
	} else {
		delim = value.find('.');
		if (delim != -1) {
			config.setHost(value);
			config.setPort("8080");
		} else {
			config.setHost("localhost");
			config.setPort(value);
		}
	}
}

std::vector<ft::Config> ft::parseServer(std::vector<std::string>& content) {
	std::vector<ft::Config> configs;
	iter_str it = content.begin();
	iter_str serv_it;
	for (it = content.begin(); it != content.end(); ++it) {
		if (*it == "server") {
			ft::Config newConfig;
			++it;
			if (*it != "{") {
				throw ft::ParserException("Parser Error: expected '{'");
			}
			for (serv_it = it; serv_it != content.end(); ++serv_it) {
				if (*serv_it == "listen") {
					++serv_it;
					parseListen(*(serv_it), newConfig);
					++serv_it;
					if (*(serv_it) != ";") {
						throw ft::ParserException("Parser Error: expected ';'");
					}
				}
				if (*serv_it == "server_name") {
					++serv_it;
					newConfig.setServName(*(serv_it));
					++serv_it;
					if (*(serv_it) != ";") {
						throw ft::ParserException("Parser Error: expected ';'");
					}
				}
				if (*serv_it == "root") {
					++serv_it;
					newConfig.setRoot(*(serv_it));
					++serv_it;
					if (*(serv_it) != ";") {
						throw ft::ParserException("Parser Error: expected ';'");
					}
				}
				if (*serv_it == "error_page") {
					++serv_it;
					newConfig.setErrPages(std::atoi((*(serv_it)).c_str()), *(serv_it + 1));
					serv_it += 2;
					if (*(serv_it) != ";") {
						throw ft::ParserException("Parser Error: expected ';'");
					}
				}
				if (*serv_it == "location") {
					++serv_it;
					std::string key_loc = *serv_it;
					++serv_it;
					if (*(serv_it) != "{") {
						throw ft::ParserException("Parser Error: expected '{'");
					}
					ft::Location location;
					newConfig.setLocation(key_loc, location);
					++serv_it;
					while (serv_it != content.end() && *serv_it != "}") {
						if (*serv_it == "root") {
							newConfig.getLocation(key_loc)->second.setRoot(*(serv_it + 1));
							if (*(serv_it + 2) != ";") {
								throw ft::ParserException("Parser Error: expected ';'");
							}
						} else if (*serv_it == "index") {
							++serv_it;
							while (serv_it != content.end() && *serv_it != ";") {
								newConfig.getLocation(key_loc)->second.setIndex(*serv_it);
								++serv_it;
							}
						} else if (*serv_it == "allow_method") {
							++serv_it;
							while (serv_it != content.end() && *serv_it != ";") {
								newConfig.getLocation(key_loc)->second.setMethods(*serv_it);
								++serv_it;
							}
						} else if (*serv_it == "client_max_body_size") {
							newConfig.getLocation(key_loc)->second.setMaxBody(*(serv_it + 1));
							if (*(serv_it + 2) != ";") {
								throw ft::ParserException("Parser Error: expected ';'");
							}
						} else if (*serv_it == "cgi_extension") {
							newConfig.getLocation(key_loc)->second.setCgiExtension(*(serv_it + 1));
							if (*(serv_it + 2) != ";") {
								throw ft::ParserException("Parser Error: expected ';'");
							}
						} else if (*serv_it == "cgi_path") {
							newConfig.getLocation(key_loc)->second.setCgiPath(*(serv_it + 1));
							if (*(serv_it + 2) != ";") {
								throw ft::ParserException("Parser Error: expected ';'");
							}
						}
						++serv_it;
					}
				}
			}
			configs.push_back(newConfig);
		}
	}
	return configs;
}

std::vector<ft::Config> ft::parser(char* path) {
	std::vector<std::string> content;
	std::vector<ft::Config>configs;

	if (ft::readFile(content, path)) {
		throw ft::ParserException("Parser Error: could not open file");
	}
	ft::checkContent(content);
	configs = parseServer(content);

	
	return configs;
}
