#include "../../inc/Server.hpp"

void    Server::kick(int fd, std::vector<std::string> &cmd)
{
    Client  *client;
    Channel *channel = getChannel(cmd[1]);
    Client  *target;
    std::vector<std::string>::iterator  it;

    client = &(getClient(fd));
    if (cmd.size() < 3)
        return (sendError(fd, ERR_NEEDMOREPARAMS(cmd[0])));
    if (!isChannelExist(cmd[1]))
        return (sendError(fd, ERR_NOSUCHCHANNEL(cmd[1])));
    if (!clientInChannel(client->getNickname(), cmd[1]))
        return (sendError(fd, ERR_NOTONCHANNEL(client->getNickname(), cmd[1])));
    if (!isModerator(client->getNickname(), cmd[1]))
        return (sendError(fd, ERR_CHANOPRIVSNEEDED(client->getNickname())));
    if (!clientInChannel(cmd[2], cmd[1]))
        return (sendError(fd, ERR_USERNOTINCHANNEL(client->getNickname(), cmd[1])));
    if (client->getNickname() == cmd[2])
        return ;
    target = getClient(cmd[2]);
    if (cmd.size() > 3)
    {
        for (size_t i = 0; i < channel->getUsers().size(); i ++)
            sendResponse(getClient(channel->getUsers()[i])->getFd(), ERR_KICK_SP(client->getNickname(), target->getNickname(), cmd[1], cmd[3]));
    }
    else
    {
        for (size_t i = 0; i < channel->getUsers().size(); i ++)
            sendResponse(getClient(channel->getUsers()[i])->getFd(), ERR_KICK(client->getNickname(), target->getNickname(), cmd[1]));
    }
    channel->setNbMembers(channel->getNbMembers() - 1);
    it = std::find(channel->getModerators().begin(), channel->getModerators().end(), cmd[2]);
    if (it != channel->getModerators().end())
        channel->getModerators().erase(std::find(channel->getModerators().begin(), channel->getModerators().end(), cmd[2]));
    it = std::find(channel->getUsers().begin(), channel->getUsers().end(), cmd[2]);
    if (it != channel->getUsers().end())
        channel->getUsers().erase(std::find(channel->getUsers().begin(), channel->getUsers().end(), cmd[2]));
}
