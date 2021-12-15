#include <iostream>
#include <stdio.h>

int main()
{
    printf("HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-length: 12\r\n\r\n");
    printf("Hello world!");
    return (0);
}