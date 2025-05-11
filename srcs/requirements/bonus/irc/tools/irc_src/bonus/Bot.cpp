# include "Bot.hpp"

void    Bot::signalHandler(int signal)
{
    (void)signal;
}

Bot::Bot(int port, std::string ip, std::string channel, std::string password)
{
    nickname = "bot";
    this->ip = ip;
    this->port = port;
    this->channel = channel;
    this->password = password;
    isLogin = false;
    forbidden_words.push_back("drugs");
    forbidden_words.push_back("hack");
    forbidden_words.push_back("illegal");
    forbidden_words.push_back("spam");
    forbidden_words.push_back("scam");
    forbidden_words.push_back("fraud");
    forbidden_words.push_back("threat");
    forbidden_words.push_back("suicide");
    forbidden_words.push_back("crime");
    forbidden_words.push_back("genocide");
}

void    Bot::init()
{
    len = sizeof(server_addr);
    if ((bot_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        throw Iexception("Socket creation error");
    pf.fd = bot_fd;
    pf.events = POLLIN;
    pf.revents = 0;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
    if (connect(bot_fd, (struct sockaddr*)&server_addr, len) < 0)
        throw Iexception("Connection to server failed");
    if (fcntl(bot_fd, F_SETFL, O_NONBLOCK) < 0)
        throw Iexception("Set socket to non block mode error!");
}

void    Bot::sanitize()
{
    char    buffer[1024];
    int     e;
    int k;
    std::string login = "pass " + password + "\nnick bot\nuser k k k k\n";
    std::string join = "join " + channel + "\n";
    std::string cmd = login + join;
    if (send(bot_fd, cmd.c_str(), cmd.length(), 0) < 0)
        throw Iexception("Send data to server error");
    while (true)
    {
        if ((k = poll(&pf, 1, -1)) < 0)
            throw Iexception("Bot Desactivated!");
        if (pf.revents == 17)
            throw Iexception("Bot Desactivated!");
        if (pf.revents == POLLIN)
        {
            memset(buffer, 0, 1024);
            if ((e = recv(bot_fd, &buffer, 1023, 0)) < 0)
                throw Iexception("Recv error");
            buffer[e] = '\0';
            response = buffer;
            parseResponse();
        }
    }
}

// utils

void    Bot::parseResponse()
{
    std::string         token;
    std::vector<std::string>::iterator  it;
    
    responseTokens.clear();
    std::stringstream   ss(response);
    while (ss >> token)
        responseTokens.push_back(token);
    if (!isLogin && std::find(responseTokens.begin(), responseTokens.end(), "Welcome") == responseTokens.end())
        throw Iexception("Cannot login to server!");
    if (!isLogin && std::find(responseTokens.begin(), responseTokens.end(), "channel") != responseTokens.end())
        throw Iexception("No such channel");
    if (!isLogin)
    {
        std::cout << "Bot activated!\n";
        isLogin = true;
        return ;
    }
    if (responseTokens.size() > 1 && (responseTokens[1] == "PRIVMSG" || responseTokens[1] == "privmsg"))
    {
        it = std::find(responseTokens.begin(), responseTokens.end(), responseTokens[1]);
        it += 2;
        if ((*it).find(':') != std::string::npos)
            *it = (*it).substr(1);
        for (size_t i = 0; i < forbidden_words.size(); i++)
        {
            if (std::find(it, responseTokens.end(), forbidden_words[i]) != responseTokens.end())
            {
                std::string user = responseTokens[0].substr(1);
                std::string kick = "kick " + channel + " " + user + "\n";
                send(bot_fd, kick.c_str(), kick.length(), 0);
            }
        }
    }
    if (responseTokens.size() > 1 && (responseTokens[1] == "KICK" || responseTokens[1] == "kick"))
    {
        it = std::find(responseTokens.begin(), responseTokens.end(), "KICK");
        it += 2;
        if (*it == "bot")
            throw Iexception("Bot Desactivated");
    }
}

Bot::~Bot()
{
    close(bot_fd);
}
