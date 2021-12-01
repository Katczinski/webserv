#include "Server.hpp"

int main(int argc, char* argv[])
{
    if (argc > 2) {
        std::cerr << "Error: invalid number of arguments" << std::endl;
        return 1;
    } else {
        try {
            // We check that the file at the specified path exists and parse it
            if (argc == 2) {
                ft::parser(argv[1]);
            // Parse the default config
            } else {
                ft::parser(0);
            }
            std::vector<ft::Server>servers;
            servers.push_back(ft::Server());
            for (size_t i = 0; i < servers.size(); i++)
                servers[i].run();
        } catch(const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return (0);
}