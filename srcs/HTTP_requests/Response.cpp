#include "Response.hpp"

std::string indexxx = "on";
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
    this->full_log["Transfer-Encoding"] = "";
    this->full_log["200"] = "OK";
    this->full_log["404"] = "Not found";
    this->full_log["400"] = "Bad Request";
    this->full_log["405"] = "Method Not Allowed";
    this->is_content_length = false;
    this->is_chunked = false;
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
    this->full_log["Transfer-Encoding"] = "";
    this->full_log["200"] = "OK";
    this->full_log["404"] = "Not found";
    this->full_log["400"] = "Bad Request";
    this->full_log["405"] = "Method Not Allowed";
    this->is_content_length = false;
    this->is_chunked = false;
}
std::string ft::Response::AutoIndexPage(void)
{
    std::cout <<"============DIR=======\n" << this->full_log["Dirrectory"] << std::endl;
    std::string dir_name = "/mnt/c/Users/Alex/Desktop/ft_server/webserver/srcs" + this->full_log["Dirrectory"];
    std::string req = "";
    DIR *dir = opendir(dir_name.c_str());
    struct dirent *ent;
    if(!dir)
    {
        std::cout << "Cant open dirr" << std::endl;
        return req;
    }
    req = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\r\n<title>" +this->full_log["Dirrectory"]+"</title>\r\n</head>\r\n";
    while ((ent=readdir(dir)) != NULL) {
        req +="<body>\r\n<p><a href=\"http://localhost:8080/";
        req += ent->d_name;
        req += "\">";
        req += ent->d_name;
        req += "</a></p>";
    }
    // "<body>\\r\n<p><a href=\"http://localhost:8080/\">Поисковая система Яндекс</a></p>\r\n</body>";  
    req += "\r\n</body>";
    closedir(dir);
    std::cout << "==================================Here==================================\n" << req << std::endl;
    return req;
}

bool ft::Response::answer(int i, int fd, ft::Config& conf)
{
    time_t now = time(0);
    std::string time = ctime(&now);
    std::string head;
    std::ostringstream body; 
    if(i == 404)
    {
        std::ifstream input (conf.getErrPages(404).c_str());
        body << input.rdbuf(); 
        // body = "<html>\r\n<head><title>404 Not Found</title></head>\r\n<body>\r\n<center><h1>404 Not Found</h1></center>\r\n<hr><center>Ne horosho</center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 404 Not found\r\nServer: WebServer/1.0\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().size()))+"\r\nConnection: "+this->full_log["Connection"]+"\r\n\r\n";
        head += body.str();
        // std::cout << head << std::endl;
        if(send(fd, head.c_str(), head.size(), 0) == -1)
        this->full_log["Connection"] = "close";

    }
    else if(i == 400)
    {
        // std::ifstream input ("/mnt/c/Users/Alex/Desktop/ft_server/webserver/srcs/Pages/index.html");
        // body << input.rdbuf(); 
        // body = "<html>\r\n<head><title>400 Bad Request</title></head>\r\n<body>\r\n<center><h1>400 Bad Request</h1>\r\n</center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 400 Bad request\r\nServer: WebServer/1.0\r\nDate: "+time+"Content-Type: text/html\r\nContent-Lenght: "+(ft::to_string(body.str().size()))+"\r\nConnection: "+this->full_log["Connection"]+"\r\n\r\n";
        head += body.str();
        // std::cout << head << std::endl;
        send(fd, head.c_str(), head.size(), 0);
        this->full_log["Connection"] = "close";
    }
    else if(i == 405)
    {
        std::ifstream input (conf.getErrPages(405).c_str());
        body << input.rdbuf(); 
        // body = "<h1>405 Try another method!</h1>\r\n";
        head = "HTTP/1.1 405 Method Not Allowed\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().size()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n\r\n";
        head += body.str();
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 200)
    {
        // std::cout << "PATH " << conf.getErrPages(505).c_str() << std::endl;
        std::string body_1;
        std::cout << "DIMA DIR====================\n" << conf.getErrPages(200) << std::endl;
        if(!indexxx.compare("on"))
        {
            //  body_1 = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\r\n<title>Тут имя папки</title>\r\n</head>\r\n<body>\
            //  \r\n<p><a href=\"http://localhost:8080/\">Поисковая система Яндекс</a></p>\r\n</body>";    
            body_1 =  this->AutoIndexPage();
        }
        else
        {
            std::ifstream input ("/mnt/c/Users/Alex/Desktop/ft_server/webserver/srcs/Pages/index.html");
            body << input.rdbuf(); 
        }
        head = "HTTP/1.1 200 OK\r\nLocation: http://"+this->full_log["Host"]+this->full_log["Dirrectory"]+"\r\nContent-Type: text/html\r\nDate: "\
        +time+"Server: WebServer/1.0\r\nContent-Length: " + (ft::to_string(body_1.length()))+"\r\nConnection: "+this->full_log["Connection"]+"\r\nSet-Cookie: name=echiles" +"\r\n\r\n";
        std::cout << head << std::endl;
        
        head += body_1;
        send(fd, head.c_str(), head.size(), 0);
        // this->full_log["Connection"] = "close";
    }
    else if(i == 505)
    {
        std::ifstream input (conf.getErrPages(505).c_str());
        body << input.rdbuf(); 
        // body = "<html>\r\n<head><title>505 HTTP Version Not Supported</title></head>\r\n<body>\r\n<center><h1>505 HTTP Version Not Supported</h1></center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 505 HTTP Version Not Supported\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().length()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n\r\n";
        head += body.str();
        // std::cout << head << std::endl;
        send(fd, head.c_str(), head.size(), 0);
    }
    if(i == 200)
        return true;
    return false;
}

bool ft::Response::general_header_check(int fd, ft::Config& conf)
{
    std::vector<std::string> header;
    size_t i = 0;
    if(!this->full_log["ZAPROS"].size())
    {
        ft_split(this->full_buffer, ' ', header);
        if(header.size() < 3)
        {
            answer(400, fd, conf);
            this->full_buffer.clear();
            this->full_log.clear();
            return false;
        }
        if(!header[0].compare(0, 3, "GET"))
            this->full_log["ZAPROS"] = header[0];
        else if(!header[0].compare(0,4,"POST"))
            this->full_log["ZAPROS"] = header[0];
        else if(!header[0].compare(0,6,"DELETE"))
            this->full_log["ZAPROS"] = header[0];
        else
            i = 400;
        if(header[1][0] != '/')
            i = 400;
        if(header[2].compare(0, 8, "HTTP/1.0") && header[2].compare(0, 8, "HTTP/1.1"))
            i = 505;
        this->full_log["Dirrectory"] = header[1];
    }
    if(i > 0)
    {
        answer(i, fd, conf);
        this->full_buffer.clear();
        this->full_log.clear();
        return(false);
    }
    return true;

}

int ft::Response::req_methods_settings(std::vector<std::string> str)
{
    std::vector<std::string>::iterator it = str.begin();
    std::string methods;
    while (it != str.end())
    {
        methods += *it;
        it++;
    }
    if(!this->full_log["ZAPROS"].compare(0, 3, "GET"))
        this->is_content_length = false;
    if(!this->full_log["ZAPROS"].compare(0,4,"POST"))
    {
        // std::map<std::string, ft::Location> it =  conf.getLocation(); в строке пути, в локейшене все его описание
        if(methods.find("POST") == std::string::npos) // заменить переменню Allowed из парсера Димы
            return(405);
        if(!this->full_log["Content-Type"].size() || !this->full_log["Content-Length"].size())
            return(400);
        std::stringstream ss;
        ss << this->full_log["Content-Length"];
        ss >> this->body_length;
        if(!this->body_length)
            this->is_content_length = false;
        if(this->is_chunked)
            this->is_content_length = false;
    }
    return 0;
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

