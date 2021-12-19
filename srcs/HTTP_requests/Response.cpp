#include "Response.hpp"

std::string indexxx = "off";
ft::Response::~Response() {}
ft::Response::Response(ft::Response const& other)
{
    *this = other;
}
ft::Response& ft::Response::operator=(ft::Response const& other)
{
    if(this != &other)
    {
        this->full_log = other.full_log;
        this->full_buffer = other.full_buffer;
        this->is_content_length = other.is_content_length;
        this->is_multy = other.is_multy;
        this->is_chunked = other.is_chunked;
    }
    return *this;
}
ft::Response::Response()
{
    this->full_log["ZAPROS"] = "";
    this->full_log["Dirrectory"] = "";
    this->full_log["Connection"] = "Keep-Alive";
    this->full_log["Host"] = "";
    this->full_log["Content-Type"] = "";
    this->full_log["Content-Length"] = "";
    this->full_log["Transfer-Encoding"] = "";
    this->full_log["Body"] = "";
    this->full_log["boundary"] = "";
    this->full_log["for_methods_location"] = "";
    this->is_content_length = false;
    this->is_chunked = false;
    this->is_multy = false;

}

void ft::Response::clear()
{
    this->full_log["ZAPROS"] = "";
    this->full_log["Dirrectory"] = "";
    this->full_log["Connection"] = "Keep-Alive";
    this->full_log["Host"] = "";
    this->full_log["Content-Type"] = "";
    this->full_log["Content-Length"] = "";
    this->full_log["Transfer-Encoding"] = "";
    this->full_log["Body"] = "";
    this->full_log["boundary"] = "";
    this->full_log["for_methods_location"] = "";
    this->body_length = 0;
    this->is_content_length = false;
    this->is_chunked = false;
    this->is_multy = false;
}
std::string ft::Response::AutoIndexPage(ft::Config& conf, std::ostringstream& body)
{
    std::string dir_name = conf.getRoot();
    dir_name.erase(dir_name.end() -1);

    // if(!current_dirrectory.empty() && this->full_log["Dirrectory"] != "/")
    //     dir_name += current_dirrectory;)
    // if (!opendir((dir_name + this->full_log["Dirrectory"]).c_str())) {
        // this->full_log["Dirrectory"] = prev_dirrectory + this->full_log["Dirrectory"];
    // }
    dir_name += this->full_log["Dirrectory"];
    this->full_log["Location"] += this->full_log["Directory"];
    std::cout << "Dirrectory===========================================================\n" << dir_name << std::endl;

    std::string req;
    DIR *dir = opendir(dir_name.c_str());
    struct dirent *ent;
    if(this->full_log["Dirrectory"].find(".") != std::string::npos)
    {
        if(this->full_log["Dirrectory"].find(".png") != std::string::npos)
            this->full_log["Content-Type"] = "image/png";
        else if(this->full_log["Dirrectory"].find(".jpeg") != std::string::npos)
            this->full_log["Content-Type"] = "image/jpeg";
        else if(this->full_log["Dirrectory"].find(".gif") != std::string::npos)
            this->full_log["Content-Type"] = "image/gif";
        else if(this->full_log["Dirrectory"].find(".mp4") != std::string::npos)
            this->full_log["Content-Type"] = "video/mp4";
        std::ifstream input (dir_name.c_str());
        body << input.rdbuf();
        return body.str();
    }
    if(!dir)
    {
        std::cout << "Cant open dirr" << std::endl;
        return req;
    }
    // else
        // this->full_log["Location"] += "/";
    req = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\r\n<title>" +this->full_log["Dirrectory"]+"</title>\r\n</head>\r\n";
    while ((ent=readdir(dir)) != NULL) {
        req +="<body>\r\n<p><a href=\"http://" + conf.getHost();
        req += ":";
        req += "8080";
        req += "/";
        req += ent->d_name;
        req += "\">";
        req += ent->d_name;
        req += "</a></p>";
    }
    req += "\r\n</body>";
    closedir(dir);
    // prev_dirrectory = this->full_log["Dirrectory"];
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
        std::ifstream input (conf.getErrPages(400).c_str());
        body << input.rdbuf(); 
        // std::ifstream input ("/mnt/c/Users/Alex/Desktop/ft_server/webserver/srcs/Pages/index.html");
        // body << input.rdbuf(); 
        // body = "<html>\r\n<head><title>400 Bad Request</title></head>\r\n<body>\r\n<center><h1>400 Bad Request</h1>\r\n</center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 400 Bad request\r\nServer: WebServer/1.0\r\nDate: "+time+"Content-Type: text/html\r\nContent-Lenght: "+(ft::to_string(body.str().size()))+"\r\nConnection: "+this->full_log["Connection"]+"\r\n\r\n";
        head += body.str();
        std::cout << head << std::endl;
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
        std::cout << head << std::endl;
        head += body.str();
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 200)
    {
        this->full_log["Content-Type"] = "text/html";
        this->full_log["Location"] =  "http://"+this->full_log["Host"];

        std::string reeal_body;
        // conf.getLocation()[this->full_log["Dirrectory"];
        // if((conf.getLocation()[this->full_log["Dirrectory"]]).getAutoindex())  
            // reeal_body = this->AutoIndexPage(conf, body);
        // else
        // {
        int i = 0;

        while(i < conf.getIndex().size())
        {

            std::ifstream input (conf.getIndex()[i].c_str());// проверять, если буфер == 0, то попробовать следующий, выкинуть 403
            if(input.is_open())
            {
                body << input.rdbuf();
                break;
            }
            i++;
        }
        if(i == conf.getIndex().size())
            return this->answer(403,fd,conf);
        reeal_body = body.str();
        // }
        head = "HTTP/1.1 200 OK\r\nLocation: " +this->full_log["Location"]+"\r\nContent-Type: " + this->full_log["Content-Type"] +"\r\nDate: "\
        +time+"Server: WebServer/1.0\r\nContent-Length: " + (ft::to_string(reeal_body.size()))+"\r\nConnection: "+this->full_log["Connection"]; //+"\r\n";
        // if(!this->prev_dirrectory.empty())
        //     head += "Refer:  http://localhost:8080/error_pages";
        // head += "Accept-Ranges: none";
        head += "\r\n\r\n";
        std::cout << head << std::endl;
        head += reeal_body;
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 505)
    {
        std::ifstream input (conf.getErrPages(505).c_str());
        body << input.rdbuf(); 
        // body = "<html>\r\n<head><title>505 HTTP Version Not Supported</title></head>\r\n<body>\r\n<center><h1>505 HTTP Version Not Supported</h1></center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 505 HTTP Version Not Supported\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().length()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n";
        head += body.str();
        // std::cout << head << std::endl;
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 403)
    {
        std::ifstream input (conf.getErrPages(403).c_str());
        body << input.rdbuf(); 
        // body = "<html>\r\n<head><title>505 HTTP Version Not Supported</title></head>\r\n<body>\r\n<center><h1>505 HTTP Version Not Supported</h1></center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 403 Forbidden\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().length()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n";
        head += body.str();
        // std::cout << head << std::endl;
        send(fd, head.c_str(), head.size(), 0);
    }
    if(i == 200)
        return true;
    return false;
}

bool ft::Response::post_request(ft::Config& config)
{
    std::string buffer;
    std::string real_body;
    std::string filename;
    bool is_bound = false;
    bool is_body = false;
    std::vector<std::string> for_filename;
    size_t i = 0;
    size_t for_split = 0;
    if(this->full_log["Body"].find("--"+this->full_log["boundary"]) != std::string::npos &&  this->full_log["Body"].find("--"+this->full_log["boundary"]+"--") != std::string::npos)
    {
        while(for_split < this->body_length)
        {
            buffer.clear();
            while(this->full_log["Body"][for_split] != '\n')
            {
                buffer += this->full_log["Body"][for_split];
                for_split++;
            }
            if(!buffer.compare(("--"+this->full_log["boundary"])+"--\r"))
            {
                is_body = true;
                is_bound = false;
            }
            if(!buffer.compare(("--"+this->full_log["boundary"]+'\r')))
                is_bound = true;
            if(is_bound)
            {
                if(!buffer.compare(0, 31, "Content-Disposition: form-data;"))
                {
                    ft_split(buffer, ';', for_filename);
                    std::vector<std::string>::iterator it = for_filename.begin();
                    while (it != for_filename.end())
                    {
                        i = (*it).find("filename=");
                        if(i != std::string::npos)
                        {
                            (*it).erase((*it).begin() + (*it).find('\r'));  
                            filename = (*it).substr(i + 9, (*it).size());
                            while ((i = filename.find('"')) != std::string::npos)
                            {
                                filename.erase(filename.begin() + i);
                            }
                        }
                        it++;
                    }   
                }
                else if(!buffer.compare(0, 1, "\r") && !filename.empty())
                {
                    is_bound = false;
                    is_body = true;
                }
            }
            else if(is_body)
            {
                if(!buffer.compare(("--"+this->full_log["boundary"]+'\r')))
                {
                    std::ofstream nope((config.getRoot() + filename).c_str(), std::ios_base::app);
                    nope << real_body;
                    nope.close();
                    is_bound = true;
                }
                else if(!buffer.compare(("--"+this->full_log["boundary"])+"--\r"))
                {
                    real_body.erase(real_body.end()-1);
                    std::ofstream nope((config.getRoot() + "dowlands/"+ filename).c_str(), std::ios_base::app);
                    real_body.erase(real_body.end() - 1);
                    nope << real_body;
                    nope.close();
                    break;
                }
                else
                    real_body += (buffer + "\n");
            }
        for_split++;
        }
        return true;
    }
    return false;
}

bool ft::Response::general_header_check(std::string str, int fd, ft::Config& conf)
{
    std::vector<std::string> header;
    size_t i = 0;
    if(!this->full_log["ZAPROS"].size())
    {
        ft_split(str, ' ', header);
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
    {
        this->is_content_length = false;
        this->is_chunked = false;
        this->is_multy = false;
    }
    if(!this->full_log["ZAPROS"].compare(0,4,"POST"))
    {
        if(methods.find("POST") == std::string::npos)
            return(405);
        if(this->full_log["Content-Type"].empty())
            return(400);
        this->body_length =  ft::ft_atoi(this->full_log["Content-Length"]);
        if(!this->body_length)
            this->is_content_length = false;
        if(this->is_chunked && !this->is_multy)
        {
            this->is_content_length = false;
            this->body_length = 0;
        }
        if(this->is_multy)
            this->is_chunked = false;
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