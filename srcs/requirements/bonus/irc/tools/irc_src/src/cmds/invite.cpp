#include "../../inc/Server.hpp"

void    Server::invite(int fd, std::vector<std::string> &cmd)
{
    Client  *client = &(getClient(fd));
    Client  *invited = getClient(cmd[1]);
    std::vector<std::string>    invites;

    std::string ch = "*";
    if (cmd.size() < 3)
        return (sendError(fd, ERR_NEEDMOREPARAMS(cmd[0]))); 
    if (!invited)
        return (sendError(fd, ERR_NOTONSERVER(client->getNickname(), cmd[2])));
    else
        invites = invited->getInvites();
    if (!isChannelExist(cmd[2]))
        return (sendError(fd, ERR_NOSUCHCHANNEL(ch)));
    if (!clientInChannel(client->getNickname(), cmd[2]))
        return (sendError(fd, ERR_NOTONCHANNEL(client->getNickname(), cmd[2])));
    if (!isModerator(client->getNickname(), cmd[2]))
        return (sendError(fd, ERR_CHANOPRIVSNEEDED(client->getNickname())));
    if (clientInChannel(cmd[1], cmd[2]))
        return (sendError(fd, ERR_USERONCHANNEL(client->getNickname(), cmd[2])));
    if (std::find(invites.begin(), invites.end(), cmd[2]) == invites.end())
        invited->addInvite(cmd[2]);
    sendResponse(fd, RPL_INVITING(client->getNickname(), invited->getNickname(), cmd[2]));
    sendResponse(invited->getFd(), RPL_INVITE(client->getNickname(), invited->getNickname(), cmd[2]));
}
