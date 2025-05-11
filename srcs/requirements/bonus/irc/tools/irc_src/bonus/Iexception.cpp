#include "Iexception.hpp"

Iexception::Iexception(const std::string &msg)
{
    this->msg = msg;
}

const char* Iexception::what() const throw()
{
    return msg.c_str();
}

Iexception::~Iexception() throw()
{

}

