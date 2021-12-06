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

ft::CGI::CGI()
{
    init_env();
}

ft::CGI::~CGI() { }

void            ft::CGI::CGI_read(long length)
{
    _data = (char*)malloc(sizeof(char) * (length + 1));
    read(1, _data, length);
    std::cout << _data << std::endl;
}

std::map<std::string, std::string>    ft::CGI::parseQString(const char *qstring)
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
        ret[key] = value;
    }
    return (ret);
}

void            ft::CGI::init_env()
{
    _env["REQUEST_METHOD"] = std::getenv("REQUEST_METHOD");
    _env["QUERY_STRING"] = std::getenv("QUERY_STRING");

    if (!strcmp((_env.find("REQUEST_METHOD"))->second, "GET"))
    {
        std::cout << "It's GET\n";
    }
    else if (!strcmp((_env.find("REQUEST_METHOD"))->second, "POST"))
    {
        std::cout << "It's POST\n";
        //проанализировать QUERY_STRING
        std::map<std::string, std::string> qstring = parseQString(_env.find("QUERY_STRING")->second);
        //Получить длину данных из CONTENT_LENGTH
        long content_length = atol(std::getenv("CONTENT_LENGTH"));
        //Если CONTENT_LENGTH > 0
        if (content_length > 0)
        {   
            //Считать CONTENT_LENGTH байт из stdin
            CGI_read(content_length);
        }
        for (std::map<std::string, std::string>::iterator it = qstring.begin(); it != qstring.end(); it++)
            std::cout << it->first << " : " << it->second << std::endl;
    }
    else
    {
        std::cerr << "Unknonw method\n";
        return ;
    }
}