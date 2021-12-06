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
            ft::Cluster cluster;
            if (argc == 2) {
                configs = ft::parser(argv[1]);
            // Parse the default config
            } else {
                configs = ft::parser(0);
            }
            cluster.setup(configs);
            // cluster.push_back(ft::Server("127.0.0.1", "8080"));
            // cluster.push_back(ft::Server("127.0.0.1", "8000"));
            cluster.run();
        } catch(const std::exception& e) {
            std::cerr << e.what() << std::endl;
        }
    }
    return (0);
}
