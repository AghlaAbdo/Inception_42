#include "../../inc/Server.hpp"
#include "../../inc/Iexception.hpp"
#include "../../inc/utils.hpp"

void    Server::user(int fd, std::vector<std::string> &cmd)
{
    Client  *client;

    client = &(getClient(fd));
    if (cmd.size() < 5)
        return(sendError(fd, ERR_NEEDMOREPARAMS(cmd[0])));
    if (!client->getIsRegistered())
        sendError(fd, ERR_NOTREGISTERED(client->getNickname()));
    else
    {
        if (client->getIsLogin())
            sendError(fd, ERR_ALREADYREGISTRED(client->getNickname()));
        else
        {
            if (!validParams(cmd))
                return(sendError(fd, ERR_USERNAME(cmd[1])));
            client->setUsername(cmd[1]);
            client->setMode(cmd[2]);
            client->setUnused(cmd[3]);
            client->setRealname(cmd[4]);
            if (client->getNickname() != "_")
            {
                client->setLogin(true);
                sendResponse(fd, RPL_WELCOME(client->getNickname(), "Welcome to the Internet Relay Network"));
            }
        }
    }
}
