#include "Server.hpp"

int main(int argc, char* argv[])
{
    if (argc > 2) {
        std::cerr << "Error: invalid number of arguments" << std::endl;
        return 1;
    } else {
        try {
            // We check that the file at the specified path exists and parse it
            std::vector<ft::Config>configs;
            if (argc == 2) {
                configs = ft::parser(argv[1]);
            // Parse the default config
            } else {
                configs = ft::parser(0);
            }

            /*
            ** Временно! Для теста парсера
            */
            // std::cout << "Host: " << configs.front().getHost() << std::endl;
            // std::cout << "Port: " << configs.front().getPort() << std::endl;
            // std::cout << "server_name:" << configs.front().getServName() << std::endl;
            // std::cout << "root: " << configs.front().getRoot() << std::endl;
            // std::map<std::string, std::string>::const_iterator it;;
            // for (it = configs.front().getErrPages().begin(); it != configs.front().getErrPages().end(); ++it) {
            //     std::cout << "Error: " << it->first << " = " << it->second << std::endl;
            // }

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