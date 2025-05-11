#include "../../inc/Server.hpp"
#include "../../inc/Iexception.hpp"
#include "../../inc/utils.hpp"

void    Server::sendNickReplay(Client *user, std::string newNick, int fd)
{
    std::vector<std::string>::iterator	it;
    (void)fd;
    for (size_t i = 0; i < channels.size(); i++) 
    {
        if (channels[i].clientInChannel(user->getNickname()))
        {
            for (it = channels[i].getUsers().begin(); it != channels[i].getUsers().end(); it++)
            {
                if (fd != getClient(*it)->getFd())
                    sendResponse(getClient(*it)->getFd(), RPL_NEWNICK(user->getNickname(), newNick));
            }
        }
    }
}

void    Server::nick(int fd, std::vector<std::string> &cmd)
{
    Client  *client;

    client = &(getClient(fd));
    if (cmd.size() == 1)
        return(sendError(fd, ERR_NEEDMOREPARAMS(cmd[0])));
    if (!client->getIsRegistered())
        sendError(fd, ERR_NOTREGISTERED(client->getNickname()));
    else
    {
        if (cmd[1] == client->getNickname())
            return ;
        if (validNickname(fd, cmd[1]))
        {
            if (cmd[1].length() > 15)
                cmd[1].resize(15);
            if (usernameSetted(client) && !client->getIsLogin())
            {
                client->setLogin(true);
                client->setNickname(cmd[1]);
                sendResponse(fd, RPL_WELCOME(client->getNickname(), "Welcome to the Internet Relay Network"));
            }
            else
            {
                sendResponse(fd, RPL_NICK(client->getNickname(), cmd[1]));
                sendNickReplay(client, cmd[1], fd);
                changeNickInChannels(client->getNickname(), cmd[1]);
                client->setNickname(cmd[1]);
            }
        }
    }
}
