#pragma once

#include "utils.hpp"

class Iexception : public std::exception
{
    std::string msg;

    public:
        Iexception(const std::string &msg);
        const char *what() const throw();
        virtual ~Iexception() throw();
};