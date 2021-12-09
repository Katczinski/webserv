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
	int pos = line.find('#');
	if (pos != -1) {
		line.erase(pos);
	}
	for (std::string::iterator it = line.begin(); it != line.end(); ++it) {
		if (*it == '\n') {
			line.replace(it, it + 1, " ");
		} else if (*it == '\t') {
			line.replace(it, it + 1, " ");
		} else if (*it == '\r') {
			line.replace(it, it + 1, " ");
		}
	}
	pos = line.find(';');
	if (pos != -1) {
		line.insert(pos, 1, ' ');
	}
	if (!line.empty() && line.find('}') == std::string::npos && line.find('{') == std::string::npos) {
		if (line.find(';') == std::string::npos) {
			throw ft::ParserException("Parser Error: expected ';'");
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
		if (!line.empty()) {
			ft::split(content, line, ' ');
		}
	}
	file.close();
	return 0;

}

void ft::checkValidPort(std::string& port) {
	// if there is no ',' at the end of the port
	if (port[port.length() - 1] != ',') {
		throw ft::ParserException("!Parser Error: port in config file is incorrect");
	}
	// all ports, except last
	port = port.substr(0, port.length() - 1);
}
std::vector<std::string> ft::checkMultiplePort(str_iter begin, const std::vector<std::string>& content) {
	std::string value;
	std::vector<std::string> ret;
	for (str_iter it = begin; it != content.end(); ++it) {
		if (*it == "listen") {
			++it;
			if (*it == ";") {
				throw ft::ParserException("Parser Error: bad config file");
			}
			while (*it != ";") {
				ret.push_back(*it);
				++it;
			}
			break;
		}
	}
	if (ret.size() > 1) {
		for (str_iter it = ret.begin(); it != ret.end(); ++it) {
			if (it != (ret.end() - 1)) {
				checkValidPort(*it);
			}
			for (std::string::iterator iter = (*it).begin(); iter != (*it).end(); ++iter) {
				if (!std::isdigit(*iter)) {
					throw ft::ParserException("Parser Error: port in config file is incorrect");
				}
			}
		}
	}
	return ret;
}

void ft::checkContent(const std::vector<std::string>& content) {
	if (std::count(content.begin(), content.end(), "}") - std::count(content.begin(), content.end(), "{")) {
		throw ft::ParserException("Parser Error: invalid number of brackets");
	}
	bool flag = false;
	for (const_str_iter it = content.begin(); it != content.end(); ++it) {
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
	size_t countPorts;

	if (ft::readFile(content, path)) {
		throw ft::ParserException("Parser Error: could not open file");
	}
	ft::checkContent(content);
	str_iter it = content.begin();
	for (str_iter it = content.begin(); it != content.end(); ++it) {
		if (*it == "server") {
			if (*(++it) != "{") {
				throw ft::ParserException("Parser Error: expected '{'");
			}
			std::vector<std::string> multiplePorts = ft::checkMultiplePort(it, content);
			configs.push_back(ft::Config(it, content));
			if (multiplePorts.size() > 1) {
				size_t i = 1;
				while (i < multiplePorts.size()) {
					ft::Config newConfig = configs.back();
					newConfig.setPort(multiplePorts[i]);
					configs.push_back(newConfig);
					++i;
				}
			}
		}
	}
			// std::cout << "size: " << configs.size() << std::endl;;
			// std::cout << "port: " << configs.front().getPort() << std::endl;
			// std::cout << "port 2: " << configs[1].getPort() << std::endl;
			// std::cout << "host: " << configs.front().getHost() << std::endl;
			// std::cout << "index: " << configs.front().getIndex().front() << std::endl;
			// std::cout << "server_name: " << configs.front().getServName() << std::endl;
			// std::cout << "root: " << configs.front().getRoot() << std::endl;
            //  std::cout << "404 path: " << configs.front().getErrPages(404) << std::endl;
            //  std::cout << "405 path: " << configs.front().getErrPages(405) << std::endl;
            //  std::cout << "Loc Index: " << configs.front().findKeyLocation("/")->second.getIndex().front() << std::endl;
            //  std::cout << "Allow method: " << configs.front().findKeyLocation("/")->second.getMethods().front() << std::endl;
            //  std::cout << "Max Body: " << configs.front().findKeyLocation("/")->second.getMaxBody() << std::endl;
            //  std::cout << "CGI extension: " << configs.front().findKeyLocation("/")->second.getCgiExtension() << std::endl;
            //  std::cout << "CGI path: " << configs.front().findKeyLocation("/")->second.getCgiPath() << std::endl;
            //  std::cout << "Loc root: " << configs.front().findKeyLocation("/")->second.getRoot() << std::endl;
            //  std::cout << "Allow method: " << configs.front().findKeyLocation("/")->second.getMethods().front() << std::endl;
            //  std::cout << "Max Body: " << configs.front().findKeyLocation("/")->second.getMaxBody() << std::endl;
            //  std::cout << "CGI extension: " << configs.front().findKeyLocation("/")->second.getCgiExtension() << std::endl;
            //  std::cout << "CGI path: " << configs.front().findKeyLocation("/")->second.getCgiPath() << std::endl;
            //  std::cout << "Loc root: " << configs.front().findKeyLocation("/")->second.getRoot() << std::endl;
            //  std::cout << "size = " << configs.front().getLocation().size() << std::endl;
			//  std::cout << "==========================\n";
			//  std::cout << "Loc Index: " << configs.front().findKeyLocation("hz")->second.getIndex().front() << std::endl;
            //  std::cout << "Allow method: " << configs.front().findKeyLocation("hz")->second.getMethods().front() << std::endl;
            //  std::cout << "Max Body: " << configs.front().findKeyLocation("hz")->second.getMaxBody() << std::endl;
            //  std::cout << "CGI extension: " << configs.front().findKeyLocation("hz")->second.getCgiExtension() << std::endl;
            //  std::cout << "CGI path: " << configs.front().findKeyLocation("hz")->second.getCgiPath() << std::endl;
            //  std::cout << "Loc root: " << configs.front().findKeyLocation("hz")->second.getRoot() << std::endl;
            //  std::cout << "Allow method: " << configs.front().findKeyLocation("hz")->second.getMethods().front() << std::endl;
            //  std::cout << "Max Body: " << configs.front().findKeyLocation("hz")->second.getMaxBody() << std::endl;
            //  std::cout << "CGI extension: " << configs.front().findKeyLocation("hz")->second.getCgiExtension() << std::endl;
            //  std::cout << "CGI path: " << configs.front().findKeyLocation("hz")->second.getCgiPath() << std::endl;
            //  std::cout << "Loc root: " << configs.front().findKeyLocation("hz")->second.getRoot() << std::endl;
			//  std::cout << "********************************************************************\n";
			
	return configs;
}
