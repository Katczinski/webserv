#include "Response.hpp"

//GET, POST, DELETE
// application/x-www-form-urlencoded multipart/form-data

// 413 -- размере requeeset Больше лимита, приходит из конфига сервера
// 505 HTTP version not suported

bool check_url(ft::Response& req, ft::Config& conf)
{
    bool is_file = false;
    std::string real_root;
    std::string real_dir;
    int         qs;
    if ((qs = req.full_log["Dirrectory"].find("?")) != std::string::npos)
    {
        req.full_log["Query_string"] = req.full_log["Dirrectory"].substr(qs + 1, req.full_log["Dirrectory"].length());
        req.full_log["Dirrectory"].erase(qs, req.full_log["Dirrectory"].length());
    }
    else
        req.full_log["Query_string"] = "";
    std::map<std::string, ft::Location>::iterator it = conf.getBeginLocation();
    while(it !=  conf.getEndLocation())
    {
        real_root = (*it).second.getRoot().substr(0, (*it).second.getRoot().size() - 1);

        real_dir = req.full_log["Dirrectory"].substr(req.full_log["Dirrectory"].find_last_of("/"), req.full_log["Dirrectory"].size());
        if((real_root + real_dir) == (*it).second.getRoot())
        {
            req.full_log["for_methods_location"] = req.full_log["Dirrectory"];
            if((real_root + req.full_log["Dirrectory"]).find("cgi") != std::string::npos)
                return true;
            return false;
        }
        else
        {
            int i = 0;
            while (i < (*it).second.getIndex().size())
            {
                if((real_root + real_dir) == (*it).second.getIndex()[i])
                {
                    size_t i = req.full_log["Dirrectory"].find_last_of('/');
                    req.full_log["for_methods_location"] = req.full_log["Dirrectory"].substr(0, i+1);
                    i = req.full_log["for_methods_location"].find_first_of('/');
                    if(i != std::string::npos && i == 0 && req.full_log["for_methods_location"].size() > 1)
                        req.full_log["for_methods_location"].erase(req.full_log["for_methods_location"].begin() +i);
                    return false;
                }
                i++;
            }
        }
        it++;
    }

    return true;
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
            {
                req.full_log["Content-Length"] = buffer.substr((buffer[15] == ' ') ?  16 : 15);
                req.body_length = ft::ft_atoi(req.full_log["Content-Length"]);
            }
        }
        if(!buffer.compare(0, 1, "\r")) // кончились хедеры - тело записывается в CLuster.cpp
            break;   
    }

    if(!req.full_log["Host"].size()) // проверка был ли хост в принципе
        return(req.answer(400, fd, conf));
    else if(check_url(req, conf)) // вот тут происходит чек location
        return(req.answer(404,fd, conf));
    int i =  req.req_methods_settings((conf.getLocation().find(req.full_log["for_methods_location"]))->second.getMethods()); // вот здесь спец-настройка в замисимости от метода и хэдеров, нам сюда
    if(i)
        return(req.answer(i, fd, conf));    
    return true;
}
