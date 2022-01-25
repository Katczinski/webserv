#include <iostream>

int main()
{
    std::string size = "<html>\r\n<head><title>301 Moved Permanently</title></head>\r\n<body>\r\n<center><h1>301 Moved Permanently</h1></center>\r\n<hr><center>WebServer/1.0 (Ubuntu recomended)</center>\r\n</body>\r\n</html>\r\n";
    std::cout << size.size() << std::endl;
}