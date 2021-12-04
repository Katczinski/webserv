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
            // std::cout << "Loc root: " << configs.front().findKeyLocation("/")->second.getRoot() << std::endl;
            // std::cout << "Index: " << configs.front().findKeyLocation("/")->second.getIndex().front() << std::endl;
            // std::cout << "Allow method: " << configs.front().findKeyLocation("/")->second.getMethods().front() << std::endl;
            // std::cout << "Max Body: " << configs.front().findKeyLocation("/")->second.getMaxBody() << std::endl;
            // std::cout << "CGI extension: " << configs.front().findKeyLocation("/")->second.getCgiExtension() << std::endl;
            // std::cout << "CGI path: " << configs.front().findKeyLocation("/")->second.getCgiPath() << std::endl;
            // std::cout << "Loc root: " << configs.front().findKeyLocation("/")->second.getRoot() << std::endl;
            // std::cout << "size = " << configs.front().getLocation().size() << std::endl;
            // std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
            // std::cout << "Index: " << configs.front().findKeyLocation("/ggg/")->second.getIndex().front() << std::endl;
            // std::cout << "Allow method: " << configs.front().findKeyLocation("/")->second.getMethods().front() << std::endl;
            // std::cout << "Max Body: " << configs.front().findKeyLocation("/ggg/")->second.getMaxBody() << std::endl;
            // std::cout << "CGI extension: " << configs.front().findKeyLocation("/ggg/")->second.getCgiExtension() << std::endl;
            // std::cout << "CGI path: " << configs.front().findKeyLocation("/ggg/")->second.getCgiPath() << std::endl;



            // for (size_t i = 0; i < configs.size(); ++i) {
                // std::cout << "Server #" << i + 1 << "-> Host: " << configs[i].getHost() << std::endl;
                // std::cout << "Server #" << i + 1 << "-> Port: " << configs[i].getPort() << std::endl;
                // std::cout << "Server #" << i + 1 << "-> Serv Name: " << configs[i].getServName() << std::endl;
                // std::cout << "Server #" << i + 1 << "-> Root " << configs[i].getRoot() << std::endl;
                // std::cout << "Server #" << i + 1 << "-> Error pages: " << configs[i].getErrPages(405) << std::endl;
                // std::cout << "Server #" << i + 1 << "-> Error pages: " << configs[i].getErrPages(404) << std::endl;
                // for (std::map<std::string, ft::Location>::const_iterator it = configs[i].getLocation().begin();
                //                 it != configs[i].getLocation().end(); ++it) {
                //     std::cout << "Server #" << i + 1 << " loc " << it->first << " : root "<< it->second.getRoot() << std::endl;
                //     std::cout << "Server #" << i + 1 << " loc " << it->first << " : max body "<< it->second.getMaxBody() << std::endl;
                //     std::cout << "Server #" << i + 1 << " loc " << it->first << " : Index ";
                //     for (std::vector<std::string>::const_iterator iter = it->second.getIndex().begin();
                //             iter != it->second.getIndex().end(); ++iter) {
                //     std::cout << " " << *iter << " ";
                //     }
                //     std::cout << std::endl;
                //     std::cout << "Server #" << i + 1 << " loc " << it->first << " : Allow method ";
                //     for (std::vector<std::string>::const_iterator iterat = it->second.getIndex().begin();
                //             iterat != it->second.getIndex().end(); ++iterat) {
                //     std::cout << " " << *iterat << " ";
                //     }
                //     std::cout << std::endl;
                //     std::cout << "Server #" << i + 1 << " loc " << it->first << " : CGI extension "<< it->second.getCgiExtension() << std::endl;
                //     std::cout << "Server #" << i + 1 << " loc " << it->first << " : CGI path "<< it->second.getCgiPath() << std::endl;
                //     std::cout << "======================================================================================" << std::endl;
                // }

            // }
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