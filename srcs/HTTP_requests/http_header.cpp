#include "Response.hpp"

//GET, POST, DELETE
// application/x-www-form-urlencoded multipart/form-data

// 413 -- размере requeeset Больше лимита, приходит из конфига сервера
// 505 HTTP version not suported

int check_url(ft::Response& req, ft::Config& conf)
{
    bool is_file = false;
    std::string     real_root;
    std::string     real_dir;
    size_t          auto_index_check_length;
    int             qs;

    if ((qs = req.full_log["Dirrectory"].find("?")) != std::string::npos) // если есть query string
    {
        req.full_log["Query_string"] = req.full_log["Dirrectory"].substr(qs + 1, req.full_log["Dirrectory"].length());
        req.full_log["Dirrectory"].erase(qs, req.full_log["Dirrectory"].length());
    }
    else
        req.full_log["Query_string"] = "";
    req.full_log["Path_info"] = "";
    std::map<std::string, ft::Location>::iterator it = conf.getBeginLocation();
    while(it != conf.getEndLocation())
    {
        if (it->second.getCgiExtension() != "")
        {
            qs = req.full_log["Dirrectory"].find(it->second.getCgiExtension());
            if (qs != std::string::npos)
            {
                req.full_log["Path_info"] = req.full_log["Dirrectory"].substr(qs + it->second.getCgiExtension().length(), req.full_log["Dirrectory"].length());
                req.full_log["Dirrectory"].erase(qs + it->second.getCgiExtension().length(), req.full_log["Dirrectory"].length());
                break ;
            }
        }
        it++;
    }
    std::cout << req.full_log["Path_info"] << std::endl;
    it = conf.getBeginLocation();

    while(it != conf.getEndLocation()) // проверяем все Location'ы из конфиг файла
    {        
        auto_index_check_length = req.full_log["Dirrectory"].find_first_of("/", 1); // находим первое вхождение / после 1 символа, нужно для автоиндекса
        // првоерка на то, если ли такая дирректория и что не пришло только  /
        if(req.full_log["Dirrectory"].length() > 1 &&  auto_index_check_length == std::string::npos &&  !(*it).first.compare(0, (*it).first.size()-1, req.full_log["Dirrectory"].substr(1, req.full_log["Dirrectory"].size()))) // если обращение идет по Location, но в конце нет /  = редирект
        {
            req.is_redir = true;
            req.full_log["Dirrectory"] += "/";
            auto_index_check_length = req.full_log["Dirrectory"].find_first_of("/", 1);
        }
        if(it->first == ((req.full_log["Dirrectory"].length() > 1) ? req.full_log["Dirrectory"].substr(1, req.full_log["Dirrectory"].size()) : req.full_log["Dirrectory"]) || ((*it).second.getAutoindex() &&\
         (*it).first == req.full_log["Dirrectory"].substr(1, (auto_index_check_length == std::string::npos) ? 1 : auto_index_check_length))) // если обращение пришло по Location или автоиндекс
        {
            req.current_location = &(*it).second;
            if((req.full_log["Dirrectory"]).find("cgi") != std::string::npos && !((*it).second.getAutoindex()))
                return 404;
            return 0;
        }
        else // обратились на индексовый файл в Location'е который прописан, например localhost:8080/index.html
        {
            struct dirent *ent;
            struct stat dir_check;
            real_root = (*it).second.getRoot().substr(0, (*it).second.getRoot().size() - 1);
            int i = 0;
            int n = req.full_log["Dirrectory"].find_last_of("/");
            if(!req.full_log["Dirrectory"].substr(1, n).compare((*it).first))
            {
                while (i < (*it).second.getIndex().size())
                {
                    if((real_root + req.full_log["Dirrectory"].substr(n, req.full_log["Dirrectory"].size())) == (*it).second.getIndex()[i])
                    {
                        req.current_location = &(*it).second;
                        return 0;
                    }
                    i++;
                }
            }
        }
        it++;
    }

    return 404;
}

bool http_header(ft::Response& req, std::string buf1, int fd, ft::Config& conf)
{
    std::string buffer;
    std::istringstream is(buf1);

    while(std::getline(is, buffer, '\n')) // парс хэдеров, ничего тут не предстоит делать, если не понадобится какой-то особенный хэдер ( особенные лежат в дефолт конструкторе Responsee - добавь если понадобится + продублируй в clear())
    {
        if(!buffer.compare(0, 5, "Host:"))
        {
            if(req.full_log["Host"] == "")
                req.full_log["Host"]= buffer.substr((buffer[5] == ' ') ? 6 : 5);
            req.full_log["Host"].erase(req.full_log["Host"].begin() + (req.full_log["Host"].find('\r')));
        }
        else if(!buffer.compare(0, 11, "Connection:"))
        {
            req.full_log["Connection"] = buffer.substr((buffer[11] == ' ') ?  12 : 11);
            if(req.full_log["Connection"].compare(0, 11, "Keep-Alive\r") && req.full_log["Connection"].compare(0, 6, "close\r"))
                req.full_log["Connection"] = "Keep-Alive";
        }
        else if(!buffer.compare(0, 18, "Transfer-Encoding:"))
        {
            if(req.full_log["Transfer-Encoding:"] == "")
                req.full_log["Transfer-Encoding:"] = buffer.substr((buffer[18] == ' ') ?  19 : 18);
            if(req.full_log["Transfer-Encoding"].compare(0, 7,"chunked"))
                req.is_chunked = true;
        }
        else if(!buffer.compare(0, 13, "Content-Type:") || !buffer.compare(0, 13, "Content-type:"))
        {
            if(req.full_log["Content-Type"] == "")
            {
                size_t i;
                req.full_log["Content-Type"] = buffer.substr((buffer[13] == ' ') ?  14 : 13);
                if(req.full_log["Content-Type"].find("multipart/form-data") != std::string::npos)
                {
                    i = req.full_log["Content-Type"].find("boundary=", 0);
                    if(i != std::string::npos)
                    {
                        req.full_log["boundary"] = req.full_log["Content-Type"].substr(i+9, req.full_log["Content-Type"].size());
                        req.full_log["boundary"].erase(req.full_log["boundary"].begin() + req.full_log["boundary"].find('\r'));
                    }
                    else
                        return req.answer(400,fd,conf);
                    req.is_multy = true;
                }
            }
        }
        else if(!buffer.compare(0, 15, "Content-length:") || !buffer.compare(0, 15, "Content-Length:"))
        {
            req.is_content_length = true;
            if(req.full_log["Content-Length"] == "")
                req.full_log["Content-Length"] = buffer.substr((buffer[15] == ' ') ?  16 : 15);
        }
        else if(!buffer.compare(0, 6, "Range:"))
        {
            req.full_log["Range"] = buffer.substr(buffer.find_first_of('=')+1, buffer.size() - 1);
            if( req.full_log["Range"][0] == '-')
                req.range_begin = 0;
            else
                req.range_begin = ft::ft_atoi(req.full_log["Range"]);
            std::string temp = req.full_log["Range"].substr(req.full_log["Range"].find_first_of('-'), req.full_log["Range"].size());
        }
        if(!buffer.compare(0, 1, "\r")) // кончились хедеры - тело записывается в CLuster.cpp
            break;   
    }
    if(!req.full_log["Host"].size()) // проверка был ли хост в принципе №№ ПРОВЕРКА ХОСТА СЕРВЕР_ИМЯ
        return(req.answer(400, fd, conf));
    int i = check_url(req, conf);
    if(i) // вот тут происходит чек location
        return(req.answer(i,fd, conf));
    i =  req.req_methods_settings((req.current_location->getMethods())); // вот здесь спец-настройка в замисимости от метода и хэдеров
    if(i)
        return(req.answer(i, fd, conf));
    return true;
}