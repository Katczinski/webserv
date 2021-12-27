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
        this->is_delete = other.is_delete;
    }
    return *this;
}
ft::Response::Response()
{
    this->is_redir = false;
    this->full_log["Connection"] = "Keep-Alive";
    this->current_location = NULL;
    this->is_content_length = false;
    this->is_chunked = false;
    this->is_multy = false;
    this->is_delete = false;
    this->body_length = 0;
}


void ft::Response::clear()
{
    this->is_redir = false;
    this->full_log.clear();
    this->full_log["Connection"] = "Keep-Alive";
    this->current_location = NULL;
    this->body_length = 0;
    this->is_content_length = false;
    this->is_chunked = false;
    this->is_delete = false;
    this->is_multy = false;
}
std::string ft::Response::AutoIndexPage(ft::Config& conf, std::ostringstream& body)
{

    std::string dir_nn = this->full_log["Dirrectory"].substr(1, this->full_log["Dirrectory"].length());
    std::string dir_name = conf.getRoot() + dir_nn; // откуда взять рут
    // std::cout << "Dirrectory===========================================================\n" << dir_name << std::endl;
    std::string req;
    DIR *dir = opendir(dir_name.c_str());
    struct dirent *ent;
    struct stat dir_check;
    if(dir_nn.find(".") != std::string::npos)
    {
        if(this->full_log["Dirrectory"].find(".png") != std::string::npos)
            this->full_log["Content-Type"] = "image/png";
        else if(this->full_log["Dirrectory"].find(".jpg") != std::string::npos)
            this->full_log["Content-Type"] = "image/jpg";
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
        return "";
    }
    else
    {
        if(*(full_log["Dirrectory"].end()-1) != '/')
        {
            full_log["Dirrectory"] += "/";
            this->is_redir = true;
        }
    }
    req = "<!DOCTYPE html>\r\n<html>\r\n<head>\r\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\r\n<title>" +this->full_log["Dirrectory"]+"</title>\r\n</head>\r\n";
    while ((ent=readdir(dir)) != NULL) {
        if(!strcmp(".", ent->d_name))
            continue;
        req +="<body>\r\n<p><a href=\"http://" + conf.getHost();
        req += ":";
        req += "8080";
        req += "/";
        req += dir_nn;
        req += ent->d_name;
        std::string str = dir_name;
        str += ent->d_name;
        if(!stat(str.c_str(), &dir_check))
        {
            if(S_ISDIR(dir_check.st_mode)){req += "/";}
        }// проверка на дирректорию, если она = добавляем / в конце
        req += "\">";
        req += ent->d_name;
        req += "</a></p>";
    }
    req += "\r\n</body>";
    closedir(dir);
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
        if (!is_delete)
        {
            std::ifstream input (conf.getErrPages(404).c_str());
            body << input.rdbuf(); 
            head = "HTTP/1.1 404 " + status(404) + "\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().size()))+"\r\nConnection: "+this->full_log["Connection"]+"\r\n\r\n";
            std::cout << head << std::endl;
            head += body.str();
            if(send(fd, head.c_str(), head.size(), 0) == -1)
                this->full_log["Connection"] = "close";
        }
        else
        {
            head = "HTTP/1.1 404 " + status(404) + "\r\nDate: "+time;
            std::cout << head << std::endl;
            send(fd, head.c_str(), head.size(), 0);
        }
    }
    else if(i == 400)
    {
        std::ifstream input (conf.getErrPages(400).c_str());
        body << input.rdbuf(); 
        head = "HTTP/1.1 400 Bad request\r\nServer: WebServer/1.0\r\nDate: "+time+"Content-Type: text/html\r\nContent-Lenght: "+(ft::to_string(body.str().size()))+"\r\nConnection: "+this->full_log["Connection"]+"\r\n\r\n";
        std::cout << head << std::endl;
        head += body.str();
        send(fd, head.c_str(), head.size(), 0);
        this->full_log["Connection"] = "close";
    }
    else if(i == 405)
    {
        std::ifstream input (conf.getErrPages(405).c_str());
        body << input.rdbuf(); 
        head = "HTTP/1.1 405 Method Not Allowed\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().size()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n\r\n";
        std::cout << head << std::endl;
        head += body.str();
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 204)
    {
        head = "HTTP/1.1 204 " + status(204) + "\r\nDate: "+time;
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 200)
    {
        this->full_log["Content-Type"] = "text/html";
        std::string reeal_body;
        if((current_location->getAutoindex()))
        {
            reeal_body = this->AutoIndexPage(conf, body);
            if(reeal_body.empty())
                return this->answer(404,fd,conf);
            this->full_log["Location"] =  "http://"+this->full_log["Host"]+this->full_log["Dirrectory"];
            if(this->is_redir)
                return this->answer(301,fd,conf);
        }
        else
        {
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
        }
        // }
        head = "HTTP/1.1 200 " + status(200) + "\r\nLocation: " +this->full_log["Location"]+"\r\nContent-Type: " + this->full_log["Content-Type"] +"\r\nDate: "\
        +time+"Server: WebServer/1.0\r\nContent-Length: " + (ft::to_string(reeal_body.size()))+"\r\nConnection: " +this->full_log["Connection"]; //+"\r\n";
        // if(!this->prev_dirrectory.empty())
        //     head += "Refer:  http://localhost:8080/error_pages";
        // head += "Accept-Ranges: none";
        head += "\r\n\r\n";
        std::cout << head << std::endl;
        head += reeal_body;
        size_t how = 0;
            how = send(fd, head.c_str(), head.size(), 0);

        // while(!head.empty())
        // {
            // if(how > 0)
                // head.erase(0, how);
        // }
    }
    else if(i == 505)
    {
        std::ifstream input (conf.getErrPages(505).c_str());
        body << input.rdbuf(); 
        head = "HTTP/1.1 505 " + status(505) + "\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().length()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n";
        std::cout << head << std::endl;
        head += body.str();
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 403)
    {
        std::ifstream input (conf.getErrPages(403).c_str());
        body << input.rdbuf();
        head = "HTTP/1.1 403 " + status(403) + "\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().length()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n";
        std::cout << head << std::endl;
        head += body.str();
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 413)
    {
        std::ifstream input(conf.getErrPages(413).c_str());
        body << input.rdbuf();
        head = "HTTP/1.1 413 " + status(413) + "\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().length()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n";
        std::cout << head << std::endl;
        head += body.str();
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 500)
    {
        std::ifstream input(conf.getErrPages(500).c_str());
        body << input.rdbuf();
        head = "HTTP/1.1 500 " + status(500) + "\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().length()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\n";
        std::cout << head << std::endl;
        head += body.str();
        send(fd, head.c_str(), head.size(), 0);
    }
    else if(i == 301)
    {
        std::cout << "IM 301 " << std::endl;
        body << "<html>\r\n<head><title>301 Moved Permanently</title></head>\r\n<body>\r\n<center><h1>301 Moved Permanently</h1></center>\r\n<hr><center>nginx/1.18.0 (Ubuntu)</center>\r\n</body>\r\n</html>\r\n";
        head = "HTTP/1.1 301 " + status(301) + "\r\nDate: "+time+"Content-Type: text/html\r\nContent-Length: "+(ft::to_string(body.str().length()))+"\r\nAllow: GET, POST" + "\r\nConnection: "\
        +this->full_log["Connection"]+"\r\nServer: WebServer/1.0\r\nLocation: " +this->full_log["Location"]+"\r\n\r\n";
        std::cout << head << std::endl;
        head += body.str();
        send(fd, head.c_str(), head.size(), 0);
        this->is_redir = false;
    }
    if(i == 200 || i == 301)
        return true;
    return false;
}

bool ft::Response::post_download_request(ft::Config& config)
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
    if(!this->full_log["ZAPROS"].compare(0, 3, "GET")) // если запрос GET - почти на все насрать
    {
        this->is_content_length = false;
        this->is_chunked = false;
        this->is_multy = false;
    }
    if(!this->full_log["ZAPROS"].compare(0,4,"POST")) // если пост
    {
        if(methods.find("POST") == std::string::npos) // если не разрешен метод = ошибка
            return(405);
        if(this->full_log["Content-Type"].empty()) // если нет Content-type - Ошибка
            return(400);
        this->body_length =  ft::ft_atoi(this->full_log["Content-Length"]); // полуучаем длинну контента
        if(!this->body_length) // если == 0, то убираем флаг на длинну контента
            this->is_content_length = false;
        if(!current_location->getMaxBody().empty() && (ft::ft_atoi(current_location->getMaxBody()) < this->body_length))
            return (413);
        if(this->is_chunked && !this->is_multy) // если есть Transfer-Encoding: chunked, то длинна контента игнорируется
        {
            this->is_content_length = false;
            this->body_length = 0;
        }
        if(this->is_multy) // если тип данных multipart/form-data - надо убарть chunked (по RFC)
            this->is_chunked = false;
    }
    if(!this->full_log["ZAPROS"].compare(0,6,"DELETE")) // если delete
    {
        if(methods.find("DELETE") == std::string::npos) // если не разрешен метод = ошибка
            return(405);
        this->is_delete = true;
        this->is_content_length = false;
        this->is_chunked = false;
        this->is_multy = false;
    }
    return 0;
}

std::string ft::Response::status(int code) {
    std::map<int, std::string> status;
    status[100] = "Continue"; //"Продолжить". Этот промежуточный ответ указывает, что запрос успешно принят и клиент может
                              // продолжать присылать запросы либо проигнорировать этот ответ, если запрос был завершён.
    status[200] = "OK"; // "Успешно". Запрос успешно обработан. Что значит "успешно", зависит от метода HTTP
    status[202] = "Accepted"; // "Принято". Запрос принят, но ещё не обработан. Это предназначено для случаев,
                             // когда запрос обрабатывается другим процессом или сервером, либо для пакетной обработки.
    status[204] = "No Content"; // "Нет содержимого". Нет содержимого для ответа на запрос, например при методе DELETE
    status[301] = "Moved Permanently"; // "Перемещён на постоянной основе". Этот код ответа значит,
                                       // что URI запрашиваемого ресурса был изменён. Возможно, новый URI будет предоставлен в ответе.
    status[307] = "Temporary Redirect"; // "Временное перенаправление". Сервер отправил этот ответ, чтобы клиент получил
                                       // запрошенный ресурс на другой URL-адрес с тем же методом, который использовал предыдущий запрос.
    status[400] = "Bad Request"; // "Плохой запрос". Этот ответ означает, что сервер не понимает запрос из-за неверного синтаксиса.
    status[403] = "Forbidden"; // "Запрещено". У клиента нет прав доступа к содержимому, поэтому сервер отказывается дать надлежащий ответ.
    status[404] = "Not Found"; // "Не найден". Сервер не может найти запрашиваемый ресурс.
    status[405] = "Method Not Allowed"; // "Метод не разрешён". Метод не может быть использован, потому что не указан в конфиге. Методы GET и HEAD всегда разрешены
    status[408] = "Request Timeout"; // Он означает, что сервер хотел бы отключить это неиспользуемое соединение
    status[413] = "Request Entity Too Large"; // Размер запроса превышает лимит, объявленный сервером. Сервер может закрыть соединение, вернув заголовок Retry-After
    status[500] = "Internal Server Error"; // "Внутренняя ошибка сервера". Сервер столкнулся с ситуацией, которую он не знает как обработать.
    status[501] = "Not Implemented"; // "Не выполнено". Метод запроса не поддерживается сервером и не может быть обработан. Исключение GET и HEAD
    status[505] = "HTTP Version Not Supported"; // "HTTP-версия не поддерживается". HTTP-версия, используемая в запросе, не поддерживается сервером.
    return status[code];
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

size_t  ft_hex_to_dec(std::string& str)
{
    size_t x;   
    std::stringstream ss;
    ss << std::hex << str;
    ss >> x;
    return(x);
}



// set/get

// void ft::Response::setFullBuffer(std::string& str, bool clear)
// {
//     if(clear)
//         this->full_buffer = str;
//     else
//         this->full_buffer += str;
// }
// std::string const ft::Response::getFullBuffer() const
// {
//     return this->full_buffer;
// }
// void ft::Response::setFullLog(std::string& key, std::string& val, bool append)
// {
//     if(append)
//         this->full_log[key] += val;
//     else
//         this->full_log[key] = val;
// }
// std::map<std::string, std::string> const ft::Response::getFullLog() const
// {
//     return this->full_log;
// }
// void ft::Response::setIsContentLength(bool set)
// {
//     this->is_content_length = set;
// }
// bool const ft::Response::getIsContentLength() const
// {
//     return this->is_content_length;
// }
// void ft::Response::setIsChunked(bool set)
// {
//     this->is_chunked = set;
// }
// bool const ft::Response::getIsChunked() const
// {
//     return this->is_chunked;
// }
// void ft::Response::setIsMulty(bool set)
// {
//     this->is_multy = set;
// }
// bool const ft::Response::getIsMulty() const
// {
//     return this->is_multy;
// }
// void ft::Response::setIsBody(bool set)
// {
//     this->is_body = set;
// }
// bool const ft::Response::getIsBody() const
// {
//     return this->is_body;
// }
// void ft::Response::setIsAutoIndex(bool set)
// {
//     this->is_auto_in = set;
// }
// bool const ft::Response::getIsAutoIndex() const 
// {
//     return this->is_auto_in;
// }
// void ft::Response::setBodyLength(size_t length)
// {
//     this->body_length = length;
// }
// size_t const ft::Response::getBodyLength() const
// {
//     return this->body_length;
// }

// end
