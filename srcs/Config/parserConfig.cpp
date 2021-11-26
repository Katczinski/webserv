#include "Config.hpp"

void ft::parserConfig(char* path) {
	std::string file;
    if (!path) {
        file = "srcs/Config/file/default.conf";
    } else {
        file = path;
    }
}

