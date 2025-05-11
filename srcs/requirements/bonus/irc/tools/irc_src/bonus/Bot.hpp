
# include <iostream>
# include <string>
# include <sstream>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <iostream>
# include <cstring>
# include <exception>
# include <csignal> 
# include <cstdlib>
# include <sstream>
# include <vector>
# include <arpa/inet.h>
# include <poll.h>
# include <fcntl.h>

#pragma once

# include "Iexception.hpp"

class Iexception;

class Bot
{
    int bot_fd;
    std::string nickname;
    std::string response;
    std::string channel;
    int port;
    std::string ip;
    sockaddr_in         server_addr;
    socklen_t           len;
    pollfd              pf;
    std::vector<std::string>    responseTokens;
    std::string         password;
    bool    isLogin;
    std::vector<std::string>    forbidden_words;

    public:
        Bot(int port, std::string ip, std::string channel, std::string password);
        ~Bot();
        void    init();
        void    sanitize();
        void    parseResponse();
        static void signalHandler(int signal);
};