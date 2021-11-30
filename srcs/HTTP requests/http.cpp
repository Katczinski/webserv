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
std::string methods = "GET, DELETE, POST";
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
        head = "HTTP/1.1 404 Not found\r\nServer: WebServer/1.0\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: 148\r\nConnection: close\r\nX-ServerBuilded: abirthda, agigi, echiles\r\n\n";
        body = "<html>\r\n<head><title>404 Not Found</title></head>\r\n<body>\r\n<center><h1>404 Not Found</h1></center>\r\n<hr><center>kittenx</center>\r\n</body>\r\n</html>\r\n";
        std::cout << head << body << std::endl;
        send(fd, head.c_str(), head.size(), 0);
        send(fd, body.c_str(), head.size(), 0);
        req.full_log["Connection"] = "close";
    }
    else if(i == 400)
    {
        head = "HTTP/1.1 400 Bad request\r\nServer: WebServer/1.0\r\nDate: "+time+"Content-Type: text/html\r\nContent-Lnght: 122\r\nConnection: close\r\nX-ServerBuilded: abirthda, agigi, echiles\r\n\n";
        body = "<html>\r\n<head><title>400 Bad Request</title></head>\r\n<body>\r\n<center><h1>400 Bad Request</h1>\r\n</center>\r\n</body>\r\n</html>";
        std::cout << head << body << std::endl;
        send(fd, head.c_str(), head.size(), 0);
        send(fd, body.c_str(), head.size(), 0);
        req.clear();
        req.full_log["Connection"] = "close";
    }
    else if(i == 405)
    {
        head = "HTTP/1.1 405 Method Not Allowed\r\nDate: "+time+"Content-Type: text/html\r\nContent-Lenght: 34\r\nAllow: "+methods+"\r\nConnection: "\
        +req.full_log["Connection"]+"\r\nServer: WebServer/1.0\r\nX-ServerBuilded: abirthda, agigi, echiles\r\n\n";
        body = "<h1>405 Try another method!</h1>\r\n";
        // std::cout << head << body << std::endl;
        send(fd, head.c_str(), head.size(), 0);
        send(fd, body.c_str(), head.size(), 0);

    }
    else if(i == 200)
    {
        head = "HTTP/1.1 200 OK\r\nLocation: http://"+server_name+req.full_log["Dirrectory"]+"\r\nContent-Type: text/html\r\nDate: "\
        +time+"Server: WebServer/1.0\r\nContent-Lengh: 12\r\nConnection: "+req.full_log["Connection"]+"\r\nX-ServerBuilded: abirthda, agigi, echiles\r\n\n";
        body = "Hello World!";
        // std::cout << head << body << std::endl;
        send(fd, head.c_str(), head.size(), 0);
        send(fd, body.c_str(), head.size(), 0);
    }
    if(i == 200)
        return true;
    return false;
}

bool general_header_check(std::vector<std::string> header, ft::Response& req, int fd)
{
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
    // else if(!header[1].compare(0, 8, "http:://"))
        // req.full_log["Host"] = header[1]; // Добавить проверку на существование хоста/ половину серверов почемуу то не подерживает
    if(header[2].compare(0, 7, "HTTP/1.") && !(header[2].size() == 8 && (header[2][7] == '0' || header[2][7] == '1' )))
        return(request(req,400,fd));
    return true;
}
bool check_url(ft::Response& req) // добавить чек нескольких серверов
{
    if(!(req.full_log["Host"]).length())
        return true;
    std::string servers_name = server_name + pemennaya_ot_Dimbl;
    std::string server_name_compare = req.full_log["Host"] + pemennaya_ot_Dimbl;
    if(servers_name.compare(server_name_compare))
        return true;
    // std::cout << "\"" << req.full_log["Host"] + pemennaya_ot_Dimbl << "\"" << std::endl;
    // std::cout << "\"" << servers_name << "\"" <<std::endl;
    return false;
}

bool ft_http_req(ft::Response& req, char* buf, int fd)
{
    std::cout << "Buffer " << buf << std::endl;
    if(buf[0] == '\r')
        return false;
    std::string buffer;
    std::vector<std::string> splited_words; // возможно переписать на char*, ибо реквест с сервера будет в буфер, а он статичный и чаровский массив
    // чтение файла
    // std::ofstream out("test.txt");
    // out << fd << std::endl;
    // std::istream file("test.txt"); // тут скорее всего будет чтение их FD от клиента, но я ХЗ на самом деле от Вовы крч FD
    // std::string buf1 = buf;
    // std::getline(buf1, buffer, '\n');
    std::string buf1 = buf;
    std::istringstream is(buf1);
    std::getline(is, buffer);
    ft_split(buf, ' ', splited_words); // проверяем первую строку
    if(general_header_check(splited_words, req, fd))
    {
        while(std::getline(is, buffer, '\n'))
        {
            if(!buffer.compare(0, 5, "Host:"))
            {
                if(req.full_log["Host"] == "")
                    req.full_log["Host"]= buffer.substr((buffer[5] == ' ') ? 6 : 5);
            }
            else if(!buffer.compare(0, 5, "Date:"))
            {
                if(req.full_log["Date"] == "")
                    req.full_log["Date"] = buffer.substr((buffer[5] == ' ') ?  6 : 5);
            }
            else if(!buffer.compare(0, 11, "Connection:"))
                req.full_log["Connection"] = buffer.substr((buffer[11] == ' ') ?  12 : 11);
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
            else
            {
                req.full_log["Body"]+= buffer;
            }
        }
        if(check_url(req))
            return(request(req,404,fd));
        if(!req.full_log["ZAPROS"].compare(0,4,"POST"))
        {
            if(methods.find("POST") == std::string::npos) // заменить переменню Allowed из парсера Димы
                return(request(req, 405,fd));
        }
    }
    else
    {
        return(request(req, 405,fd));
    }
   return(request(req, 200,fd));
    // for (std::map<std::string, std::string>::iterator it = req.full_log.begin();  it != req.full_log.end(); it++)
    // {
        // if((*it).second != "")
            // std::cout << (*it).first <<" " <<  (*it).second << std::endl;
    // }
    
}   
