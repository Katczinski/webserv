#include "Response.hpp"

//GET, POST, DELETE
// application/x-www-form-urlencoded multipart/form-data

std::string server_name = "localhost:8080";
std::string pemennaya_ot_Dimbl = "/";
std::string methods = "GET, DELETE, POST";
// 413 -- размере requeeset Больше лимита, приходит из конфига сервера
// 505 HTTP version not suported

bool check_url(ft::Response& req,  ft::Config& conf) // добавить чек нескольких серверов
{
    std::string server_name_compare = req.full_log["Host"] + (req.full_log["Dirrectory"]).c_str();

    // if((conf.getLocation()[req.full_log["Dirrectory"]]).getIndex().size())
        // return true;    
    std::string server_name = conf.getHost() + conf.getPort();
    std::cout << "Server name " << server_name << std::endl;
    std::string servers_name = server_name + pemennaya_ot_Dimbl;
    if(servers_name.compare(server_name_compare))
        return true;
    return false;
}

bool http_header(ft::Response& req, std::string buf1, int fd, ft::Config& conf)
{
    std::string buffer;
    std::istringstream is(buf1);

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
                req.full_log["Content-Type"] = buffer.substr((buffer[13] == ' ') ?  14 : 13);
        }
        else if(!buffer.compare(0, 15, "Content-length:") || !buffer.compare(0, 15, "Content-Length:"))
        {
            req.is_content_length = true;
            if(req.full_log["Content-Length"] == "")
                req.full_log["Content-Length"] = buffer.substr((buffer[15] == ' ') ?  16 : 15);
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
    }
    // conf.getHost() conf.getPort()
    if(!req.full_log["Host"].size())
        return(req.answer(400, fd, conf));    
    // else if(check_url(req, conf))
        // return(req.answer(404,fd, conf));    
    if(!req.full_log["ZAPROS"].compare(0, 3, "GET"))
        req.is_content_length = false;
    if(!req.full_log["ZAPROS"].compare(0,4,"POST"))
    {
        // std::map<std::string, ft::Location> it =  conf.getLocation(); в строке пути, в локейшене все его описание
        if(methods.find("POST") == std::string::npos) // заменить переменню Allowed из парсера Димы
            return(req.answer(405,fd, conf));
        if(!req.full_log["Content-Type"].size() || !req.full_log["Content-Length"].size())
            return(req.answer(400,fd, conf));
        std::stringstream ss;
        ss << req.full_log["Content-Length"];
        ss >> req.body_length;
        if(!req.body_length)
            req.is_content_length = false;
        if(req.is_chunked)
            req.is_content_length = false;
    }
    return true;
}
