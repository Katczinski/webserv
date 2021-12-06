#include "CGI.hpp"

int     main()
{
    setenv("REQUEST_METHOD", "POST", 0);
    setenv("CONTENT_LENGTH", "12", 0);
    setenv("QUERY_STRING", "http://.../cgi-bin/guestbook.cgi?user=bob&rec=0", 0);
    ft::CGI cgi;
    unsetenv("REQUEST_METHOD");
    unsetenv("CONTENT_LENGTH");
    unsetenv("QUERY_STRING");
    return (0);
}