#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Client.hpp"

void    Server::part(int fd, std::vector<Client> &clients, std::vector<std::string> &cmd)
{
    std::vector<std::string>::iterator  it;
    Client          *user = &getClient(fd);
    std::string     msg;

    if (cmd.size() == 1)
        return (sendError(fd, ERR_NEEDMOREPARAMS(cmd.at(0))));

    Channel *chnl = getChannel(cmd.at(1));
    if (!chnl)
        return (sendError(fd, ERR_NOSUCHCHANNEL(cmd.at(1))));

    if (!chnl->clientInChannel(user->getNickname()))
        return (sendError(fd, ERR_NOTONCHANNEL(user->getNickname(), cmd.at(1))));

    if (cmd.size() >= 3)
        msg = cmd.at(2);
    chnl->privmsg(clients, RPL_PART(user->getNickname(), user->getUsername(), user->getIp(), cmd.at(1), msg));
    
    it = std::find(chnl->getUsers().begin(), chnl->getUsers().end(), user->getNickname());
    if (it != chnl->getUsers().end())
        chnl->getUsers().erase(it);

    it = std::find(chnl->getModerators().begin(), chnl->getModerators().end(), user->getNickname());
    if (it != chnl->getModerators().end())
        chnl->getModerators().erase(it);

    chnl->setNbMembers(chnl->getNbMembers() - 1);
    if (!chnl->getNbMembers())
        channels.erase(getChannelIt(chnl->getChnlName()));


}