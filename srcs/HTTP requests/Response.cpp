#include "Response.hpp"


ft::Response::Response()
{
    this->full_log["ZAPROS"] = "";
    this->full_log["Dirrectory"] = "";
    this->full_log["Connection"] = "Keep-Alive";
    this->full_log["Host"] = "";
    this->full_log["Date"] = "";
    this->full_log["Content-Type"] = "";
    this->full_log["Content-Length"] = "";
    this->full_log["Server"] = "";
    this->full_log["User-Agent"] = "";
    this->full_log["200"] = "OK";
    this->full_log["404"] = "Not found";
    this->full_log["400"] = "Bad Request";
    this->full_log["405"] = "Method Not Allowed";
    this->is_content_length = false;
}

void ft::Response::clear()
{
    this->full_log["ZAPROS"] = "";
    this->full_log["Dirrectory"] = "";
    this->full_log["Connection"] = "Keep-Alive";
    this->full_log["Host"] = "";
    this->full_log["Date"] = "";
    this->full_log["Content-Type"] = "";
    this->full_log["Content-Length"] = "";
    this->full_log["Server"] = "";
    this->full_log["User-Agent"] = "";
    this->full_log["200"] = "OK";
    this->full_log["404"] = "Not found";
    this->full_log["400"] = "Bad Request";
    this->full_log["405"] = "Method Not Allowed";
    is_content_length = false;
}

bool ft::Response::answer(int i, int fd)
{
    time_t now = time(0);
    std::string time = ctime(&now);
    std::string head;
    std::string body;

    if(i == 404)
    {
        body = "<html>\r\n<head><title>404 Not Found</title></head>\r\n<body>\r\n<center><h1>404 Not Found</h1></center>\r\n<hr><center>Ne horosho</center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 404 Not found\r\nServer: WebServer/1.0\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.size()))+"\r\nConnection: "+this->full_log["Connection"]+"\r\n\r\n";
        head += body;
        std::cout << head << std::endl;
        if(send(fd, head.c_str(), head.size(), 0) == -1)
        this->full_log["Connection"] = "close";

    }
    else if(i == 400)
    {
        body = "<html>\r\n<head><title>400 Bad Request</title></head>\r\n<body>\r\n<center><h1>400 Bad Request</h1>\r\n</center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 400 Bad request\r\nServer: WebServer/1.0\r\nDate: "+time+"Content-Type: text/html\r\nContent-Lnght: "+(ft::to_string(body.size()))+"\r\nConnection: "+this->full_log["Connection"]+"\r\n\r\n";
        head += body;
        std::cout << head << std::endl;
        send(fd, head.c_str(), head.size(), 0);
        this->full_log["Connection"] = "close";
    }
    else if(i == 405)
    {
        body = "<h1>405 Try another method!</h1>\r\n";
        head = "HTTP/1.1 405 Method Not Allowed\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.size()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n\r\n";
        head += body;
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 200)
    {
        body = "<h1>Hello world!</h1>";
        head = "HTTP/1.1 200 OK\r\nLocation: http://"+this->full_log["Host"]+this->full_log["Dirrectory"]+"\r\nContent-Type: text/html\r\nDate: "+time+"Server: WebServer/1.0\r\nContent-Length: " + (ft::to_string(body.size()))+"\r\nConnection: "+this->full_log["Connection"]+"\r\n\r\n";
        head += body;
        std::cout << head << std::endl;
        send(fd, head.c_str(), head.size(), 0);
        // this->full_log["Connection"] = "close";
    }
    if(i == 200)
        return true;
    return false;
}

bool ft::Response::general_header_check(std::vector<std::string>& header, int fd)
{
    if(!header[0].compare(0, 3, "GET"))
        this->full_log["ZAPROS"] = header[0];
    else if(!header[0].compare(0,4,"POST"))
        this->full_log["ZAPROS"] = header[0];
    else if(!header[0].compare(0,6,"DELETE"))
        this->full_log["ZAPROS"] = header[0];
    else
        return(this->answer(400, fd));
    if(header[1][0] != '/')
        return(this->answer(400,fd));
    if(header[2].compare(0, 7, "HTTP/1.") && !(header[2].size() == 8 && (header[2][7] == '0' || header[2][7] == '1' )))
        return(this->answer(400,fd));
    this->full_log["Dirrectory"] = header[1];
    return true;
}


void ft_split(std::string const &str, const char delim,
            std::vector<std::string> &out)
{
    size_t start;
    size_t end = 0;
 
    while ((start = str.find_first_not_of(delim, end)) != std::string::npos)
    {
        end = str.find(delim, start);
        out.push_back(str.substr(start, end - start));
    }
}

