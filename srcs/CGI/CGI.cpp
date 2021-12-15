#include "CGI.hpp"

// Возвращает верхний регистр символа
char upperchar(char ch) {
	if ((ch >= 'a') && (ch <= 'z')) {
		ch = 'A' + (ch - 'a');
		 return ch;
	} else return ch;
};

// Переводит из Hex в Dec
char gethex(char ch) {
	ch = upperchar(ch);
	if ((ch >= '0') && (ch <= '9')) return (ch - '0');
	if ((ch >= 'A') && (ch <= 'F')) return (ch - 'A' + 10);
    else return (ch);
};

ft::CGI::CGI(ft::Response& req)
{
    std::cout << "====================================================================================\n";
    init_env(req);
    _argv = (char**)malloc(sizeof(char*) * 2);
    _argv[0] = strdup((std::string(std::getenv("PWD")) + "/srcs/www" + _env["SCRIPT_NAME"]).c_str());
    std::cout << "\n\n" << _argv[0] << "\n\n";
    _argv[1] = NULL;
}

ft::CGI::~CGI() { 
    int i = 0;
    for ( ; _cenv[i]; i++)
        free(_cenv[i]);
    free(_cenv[i]);
    free(_cenv);
}

void            ft::CGI::CGI_read(long fd)
{
    char buf[3000];
    int ret = 1;
    while (ret)
    {
        ret = read(fd, buf, 3000);
        buf[ret] = '\0';
        _data += std::string(buf);
    }
    std::cout << "Response:\n" << _data << std::endl;
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
    // _env["QUERY_STRING"] = "";
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

std::string             ft::CGI::execute(ft::Response& req)
{
    pid_t   pid;
    FILE	*fIn = tmpfile();
	FILE	*fOut = tmpfile();
	long	fdIn = fileno(fIn);
	long	fdOut = fileno(fOut);
    int     status;

    // CGI_write(req.full_log["Body"]);
    write(fdIn, req.full_log["Body"].c_str(), req.full_log["Body"].size());
    lseek(fdIn, 0, SEEK_SET);
    pid = fork();
    if (pid == 0)
    {
        dup2(fdIn, 0);
        dup2(fdOut, 1);
        status = execve(_argv[0], _argv, _cenv);
        std::cerr << "Execve failed\n" << strerror(errno) << std::endl;
        exit(status);
    }
    else if (pid > 0){
        waitpid(pid, &status, 0);
        lseek(fdOut, 0, SEEK_SET);
        CGI_read(fdOut);
    }
    else
        std::cerr << "Fork failed\n";
    close(fdIn);
    close(fdOut);
    fclose(fIn);
    fclose(fOut);
    return (_data);
}