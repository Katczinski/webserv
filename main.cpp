#include "WebServer.hpp"

int main(int argc, char* argv[])
{
    if (argc > 2) {
        std::cerr << "Error: invalid number of arguments" << std::endl;
    } else {
        // We check that the file at the specified path exists and parse it
        if (argc == 2) {
            ft::parserConfig(argv[1]);
        // Parse the default config
        } else {
            ft::parserConfig(0);
        }
    }
    ft::WebServer server;
    return (0);
}