# include "Bot.hpp"

bool validPort(const std::string &port)
{
    if (port.length() < 1 || port.length() > 5)
        return (false);
    
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!std::isdigit(port[i]))
            return (false);
    }
    return (true);
}

int main(int argc, char **argv)
{
     if (argc != 5)
        return (std::cerr << "Invalid number of arguments: <port> <hostname/ip> <channel> <passowrd>\n", 1);
    try{
        int                 port;
        if (!validPort(argv[1]))
            return (std::cerr << "Not valid port!\n", 1);
        std::stringstream   ss(argv[1]);
        ss >> port;
        if (port > 65535 || port < 1)
            return (std::cerr << "Not valid port!\n", 1);
        signal(SIGINT, Bot::signalHandler);
        signal(SIGQUIT, Bot::signalHandler);
        Bot bot = Bot(port, argv[2], argv[3], argv[4]);
        bot.init();
        bot.sanitize();
    }
    catch(std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}
