#include "ListeningSocket.hpp"

ft::ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int backlog) : BindingSocket(domain, service, protocol, port, interface)
{
    _backlog = backlog;
    start_listening();
    test_connection(_listening);
}

void            ft::ListeningSocket::start_listening()
{
    _listening = listen(get_sock(), _backlog);
}