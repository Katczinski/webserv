#include "CGI.hpp"

// Возвращает верхний регистр символа
char upperchar(char ch) {
	if ((ch >= 'a') && (ch <= 'z')) {
		ch = 'A' + (ch - 'a');
		 return ch;
	} else return ch;
};

std::string     toHex(int number)
{
    std::stringstream sstream;
    sstream << std::hex << number;
    std::string result = sstream.str();
    return result;
}

// Переводит из Hex в Dec
char gethex(char ch) {
	ch = upperchar(ch);
	if ((ch >= '0') && (ch <= '9')) return (ch - '0');
	if ((ch >= 'A') && (ch <= 'F')) return (ch - 'A' + 10);
    else return (ch);
};

ft::CGI::CGI(ft::Response& req, ft::Config& conf)
{
    std::cout << "====================================================================================\n";
    init_env(req);
    _argv = (char**)malloc(sizeof(char*) * 3);
    _argv[0] = strdup((*conf.getLocation().find("cgi-bin/")).second.getCgiPath().c_str());
    _argv[1] = strdup((std::string(std::getenv("PWD")) + "/srcs/www" + _env["SCRIPT_NAME"]).c_str());
    _argv[2] = NULL;
}

ft::CGI::~CGI() { 
    int i = 0;
    for ( ; _cenv[i]; i++)
        free(_cenv[i]);
    free(_cenv[i]);
    free(_cenv);
    free(_argv[0]);
    free(_argv[1]);
    free(_argv[2]);
    free(_argv);
    _env.clear();
}

void    ft::CGI::formHeader(std::string& header)
{
    header.insert(0, "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nTransfer-Encoding: chunked\r\nConnection: keep-alive\r\n\r\n");
}

void    ft::CGI::parseQString(const char *qstring)
{
    int                                 i = 0;
    std::map<std::string, std::string>  ret;
    while (qstring[i] != '?' && qstring[i] != '\0')
        i++;
    if (qstring[i] != '\0')
        i++;
    for (; qstring[i] != '\0'; i++)
    {
        std::string key;
        std::string value;
        while (qstring[i] != '=' && qstring[i] != '\0')
            key += qstring[i++];
        i++;
        while (qstring[i] != '&' && qstring[i] != '\0')
            value += qstring[i++];
        _env[key] = value;
    }
}

std::string     ft::CGI::getExt(const std::string& path, char delim)
{
    size_t  res;
    res = path.find(delim);
    if (res == std::string::npos)
        return ("");
    else
        return (path.substr(res + 1, path.size()));
}

std::string            ft::CGI::getHost(const std::string& path)
{
    size_t  res;
    res = path.find(':');
    if (res == std::string::npos)
        return ("");
    else
        return (path.substr(0, res));
}

void            ft::CGI::init_env(ft::Response& req)
{
    _env["AUTH_TYPE"] = "";
    _env["CONTENT_LENGTH"] = req.full_log["Content-Length"];
    _env["CONTENT_TYPE"] = req.full_log["Content-type"];
    _env["GATEWAY_INTERFACE"] = "CGI/1.1";
    _env["PATH_INFO"] = getExt(req.full_log["Location"], '?');
    _env["PATH_TRANSLATED"] = getExt(req.full_log["Location"], '?');
    
    _env["QUERY_STRING"] = req.full_log["ZAPROS"] == "POST" ? req.full_log["Body"] : req.full_log["Query_string"];
    _env["REMOTE_ADDR"] = req.full_log["Host"];
    // _env["REMOTE_HOST"] = "";
    // _env["REMOTE_IDENT"] = "";
    // _env["REMOTE_USER"] = "";
    _env["REQUEST_METHOD"] = req.full_log["ZAPROS"];
    _env["SCRIPT_NAME"] = req.full_log["Dirrectory"];
    _env["SERVER_NAME"] = getHost(req.full_log["Host"]);
    _env["SERVER_PORT"] = getExt(req.full_log["Host"], ':');
    _env["SERVER_PROTOCOL"] = "HTTP/1.1";
    // _env["SERVER_SOFTWARE"] = "";
    _cenv = (char**)malloc(sizeof(char*) * (_env.size() + 1));
    int i = 0;
    for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
    {
        std::string concat;
        concat = it->first + "=" + it->second;
        _cenv[i] = strdup(concat.c_str());
        i++;
    }
    _cenv[i] = NULL;
    // _env.clear();
}

std::string             ft::CGI::execute(ft::Response& req, int fd)
{
    pid_t   pid;
    int     pipe_in[2], pipe_out[2];
    int     status;

    if (pipe(pipe_in) < 0)
    {
        std::cerr << "pipe failed\n";
        //send something? i dunno
        return "";
    }
    if (pipe(pipe_out) < 0)
    {
        std::cerr << "pipe failed\n";
        close(pipe_in[0]);
        close(pipe_in[1]);
        return "";
    }
    pid = fork();
    if (pid == 0)
    {
        dup2(pipe_in[0], STDIN_FILENO);
        dup2(pipe_out[1], STDOUT_FILENO);
        close(pipe_in[0]);
        close(pipe_in[1]);
        close(pipe_out[0]);
        close(pipe_out[1]);
        status = execve(_argv[0], _argv, _cenv);
        // вернуть 500 ошибку
        std::cerr << "Execve failed\n" << strerror(errno) << std::endl;
        exit(status);
    }
    else if (pid > 0){
        waitpid(pid, &status, 0);
        close(pipe_out[1]);
        char        buf[201];
        int         res = read(pipe_out[0], buf, 100);
        buf[res] = '\0';
        std::string header(buf);
        std::string body;
        size_t      dcrlf = header.find("\r\n\r\n");

        if (dcrlf != std::string::npos)
        {
            body = header.substr(dcrlf + 4, res);
            header.erase(dcrlf + 4, header.length());
            header.insert(dcrlf, "\r\nTransfer-Encoding: chunked\r\nConnection: keep-alive");
        }
        else
        {
            formHeader(header);
            dcrlf = header.find("\r\n\r\n");
            body = header.substr(dcrlf + 4, res);
            header.erase(dcrlf + 4, header.length());
        }
        send(fd, header.c_str(), header.length(), 0);
        do 
        {
            std::string chunk;
            chunk = to_string(toHex(body.length()));
            chunk += "\r\n";
            chunk += body;
            chunk += "\r\n";
            std::cout << chunk << std::endl;
            send(fd, chunk.c_str(), chunk.length(), 0);
            std::cout << "Chunk sent\n";
            if (res <= 0)
                break;
            res = read(pipe_out[0], buf, 200);
            buf[res] = '\0';
            body = to_string(buf);
        }while (res > 0);
        send(fd, "0\r\n\r\n", 5, 0);
        close(pipe_out[0]);
        close(pipe_in[1]);
        close(pipe_in[0]);
    }
    else
        std::cerr << "Fork failed\n";

    return (_data);
}