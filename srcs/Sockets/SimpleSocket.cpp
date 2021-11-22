#include "SimpleSocket.hpp"

ft::SimpleSocket::SimpleSocket(int domain, int service,
                                int protocol, int port, u_long interface)
{
    //Define address structure
    _address.sin_family = domain;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = htonl(interface);
    //Establish connection
    _sock = socket(domain, service, protocol);
    test_connection(_sock);
}

void                ft::SimpleSocket::test_connection(int item_to_test)
{
    //Confirm that the socket or connection has been properly established
    if (item_to_test < 0)
    {
        perror("Failed to connect");
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in  ft::SimpleSocket::get_address() { return (_address); }
int                 ft::SimpleSocket::get_sock() { return (_sock); }
int                 ft::SimpleSocket::get_connection() { return (_connection); }
void                ft::SimpleSocket::set_connection(int con) { _connection = con; }