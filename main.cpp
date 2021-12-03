#include "Cluster.hpp"

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
            // std::cout << "404 path: " << configs.front().getErrPages(404) << std::endl;
            // std::cout << "405 path: " << configs.front().getErrPages(405) << std::endl;
            ft::Cluster cluster;
            cluster.push_back(ft::Server("127.0.0.1", "8080"));
            cluster.push_back(ft::Server("127.0.0.1", "8000"));
            cluster.run();
        } catch(const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return (0);
}