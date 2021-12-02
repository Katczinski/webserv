#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "Response.hpp"
#include "Server.hpp"
#include <ctime>
#include <sstream>
#include <fcntl.h>

//GET, POST, DELETE

std::string server_name = "localhost:8080";
std::string pemennaya_ot_Dimbl = "/";
std::string methods = "GET, DELETE";
// 413 -- размере requeeset Больше лимита, приходит из конфига сервера
// 
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



bool request(ft::Response& req, int i, int fd)
{
    time_t now = time(0);
    std::string time = ctime(&now);
    std::string head;
    std::string body;
    if(i == 404)
    {
        body = "<html>\r\n<head><title>404 Not Found</title></head>\r\n<body>\r\n<center><h1>404 Not Found</h1></center>\r\n<hr><center>Ne horosho</center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 404 Not found\r\nServer: WebServer/1.0\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.size()))+"\r\nConnection: "+req.full_log["Connection"]+"\r\n\r\n";
        head += body;
        std::cout << head << std::endl;
        if(send(fd, head.c_str(), head.size(), 0) == -1)

        req.full_log["Connection"] = "close";

    }
    else if(i == 400)
    {
        body = "<html>\r\n<head><title>400 Bad Request</title></head>\r\n<body>\r\n<center><h1>400 Bad Request</h1>\r\n</center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 400 Bad request\r\nServer: WebServer/1.0\r\nDate: "+time+"Content-Type: text/html\r\nContent-Lnght: "+(ft::to_string(body.size()))+"\r\nConnection: "+req.full_log["Connection"]+"\r\n\r\n";
        head += body;
        std::cout << head << std::endl;
        send(fd, head.c_str(), head.size(), 0);
        req.full_log["Connection"] = "close";
    }
    else if(i == 405)
    {
        body = "<h1>405 Try another method!</h1>\r\n";
        head = "HTTP/1.1 405 Method Not Allowed\r\nDate: "+time+"Content-Type: text/html\r\nContent-Lenght: "+(ft::to_string(body.size()))+"\r\nAllow: "+methods+"\r\nConnection: "\
        +req.full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n\r\n";
        head += body;
        send(fd, head.c_str(), head.size(), 0);
        // req.full_log["Connection"] = "close";
    }
    else if(i == 200)
    {
        // req.full_log["Connection"] = "close";
        body = "<h1>Hello world!</h1>\r\n\r\n";
        head = "HTTP/1.1 200 OK\r\nLocation: http://"+req.full_log["Host"]+req.full_log["Dirrectory"]+"\r\nContent-Type: text/html\r\nDate: "+time+"Server: WebServer/1.0\r\nContent-Lengh: " + (ft::to_string(body.size()))+"\r\nConnection: "+req.full_log["Connection"]+"\r\n\r\n";
        head += body;
        std::cout << head << std::endl;
        // std::cout << "SIZE " << body.size() << std::endl;
        send(fd, head.c_str(), head.size(), 0);
    }
    if(req.full_log["Connection"] == "close")
    {
        req.clear();
        req.full_log["Connection"] = "close";
    }
    else
        req.clear();
    if(i == 200)
        return true;
    return false;
}

bool general_header_check(std::vector<std::string>& header, ft::Response& req, int fd)
{
    // for (std::vector<std::string>::iterator it = header.begin(); it != header.end(); it++)
    // {
    //     std::cout << "Header " << *it << std::endl;
    // }
    if(!header[0].compare(0, 3, "GET"))
        req.full_log["ZAPROS"] = header[0];
    else if(!header[0].compare(0,4,"POST"))
        req.full_log["ZAPROS"] = header[0];
    else if(!header[0].compare(0,6,"DELETE"))
        req.full_log["ZAPROS"] = header[0];
    else
        return(request(req,400, fd));
    if(header[1][0] != '/')
        return(request(req,400,fd));
    if(header[2].compare(0, 7, "HTTP/1.") && !(header[2].size() == 8 && (header[2][7] == '0' || header[2][7] == '1' )))
        return(request(req,400,fd));
    req.full_log["Dirrectory"] = header[1];
    return true;
}
bool check_url(ft::Response& req) // добавить чек нескольких серверов
{
    if(!((req.full_log["Host"]).length()))
        return true;
    std::string servers_name = server_name + pemennaya_ot_Dimbl;
    std::string server_name_compare = req.full_log["Host"] + (req.full_log["Dirrectory"]).c_str();
    if(servers_name.compare(server_name_compare))
        return true;
    return false;
}

bool ft_http_req(ft::Response& req, char* buf, int fd, bool flag)
{
    std::string buffer;
    std::vector<std::string> splited_words;
    std::string buf1 = buf;
    std::istringstream is(buf1);
    if(!req.full_log["ZAPROS"].size())
    {
        std::getline(is, buffer);
        ft_split(buffer, ' ', splited_words); // проверяем первую строку
        if(!general_header_check(splited_words, req, fd))
            return false;
    }
    while(std::getline(is, buffer, '\n'))
    {
        if(!buffer.compare(0, 5, "Host:"))
        {
            if(req.full_log["Host"] == "")
                req.full_log["Host"]= buffer.substr((buffer[5] == ' ') ? 6 : 5);
            req.full_log["Host"].erase(req.full_log["Host"].begin() + (req.full_log["Host"].find('\r')));
        }
        else if(!buffer.compare(0, 5, "Date:"))
        {
            if(req.full_log["Date"] == "")
                req.full_log["Date"] = buffer.substr((buffer[5] == ' ') ?  6 : 5);
        }
        else if(!buffer.compare(0, 11, "Connection:"))
        {
            req.full_log["Connection"] = buffer.substr((buffer[11] == ' ') ?  12 : 11);
            if(req.full_log["Connection"].compare(0, 10, "Keep-Alive") && req.full_log["Connection"].compare(0, 5, "close"))
                req.full_log["Connection"] = "Keep-Alive";
        }
        else if(!buffer.compare(0, 13, "Content-Type:"))
        {
            if(req.full_log["Content-Type"] == "")
                req.full_log["Content-Type"] = buffer.substr((buffer[13] == ' ') ?  14 : 13);
        }
        else if( !buffer.compare(0, 15, "Content-Lenght:"))
        {
            if(req.full_log["Content-Lenght"] == "")
                req.full_log["Content-Lenght"] = buffer.substr((buffer[15] == ' ') ?  16 : 15);
        }
        else if(!buffer.compare(0, 7, "Server:"))
        {
            if(req.full_log["Server"] == "")
                req.full_log["Server"] = buffer.substr((buffer[7] == ' ') ?  8 : 7);
        }
        else if(!buffer.compare(0, 11, "User-Agent:"))
        {
            if(req.full_log["User-Agent"] == "")
                req.full_log["User-Agent"] = buffer.substr((buffer[11] == ' ') ? 12 : 11);
        }
        else if(buffer[0] == '\r' && flag == false)
            flag = true;
        else if(buffer[0] != '\r')
            req.full_log["Body"]+= buffer;
        if(flag == true)
        {
            flag = false;
            if(!req.full_log["ZAPROS"].compare(0,4,"POST"))
            {
                if(methods.find("POST") == std::string::npos) // заменить переменню Allowed из парсера Димы
                    return(request(req, 405,fd));
            }
            if(!req.full_log["Host"].size())
                return(request(req, 400, fd));
            else if(check_url(req))
                return(request(req,404,fd));
            return(request(req, 200,fd));
        }
    }
    return true;
}
