#include "Response.hpp"

ft::Response::Response()
{
    this->full_log["ZAPROS"] = "";
    this->full_log["Dirrectory"] = "";
    this->full_log["Connection"] = "Keep-Alive";
    this->full_log["Host"] = "";
    this->full_log["Date"] = "";
    this->full_log["Content-Type"] = "";
    this->full_log["Content-Lenght"] = "";
    this->full_log["Server"] = "";
    this->full_log["User-Agent"] = "";

}

void ft::Response::clear()
{
    this->full_log["ZAPROS"] = "";
    this->full_log["Dirrectory"] = "";
    this->full_log["Connection"] = "Keep-Alive";
    this->full_log["Host"] = "";
    this->full_log["Date"] = "";
    this->full_log["Content-Type"] = "";
    this->full_log["Content-Lenght"] = "";
    this->full_log["Server"] = "";
    this->full_log["User-Agent"] = "";
}
