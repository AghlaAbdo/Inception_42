#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

void    Server::topic(int fd, std::vector<std::string> &cmd)
{
    Client      *user = &getClient(fd);
    Channel     *chnl;
    std::string chnlName;

    if (cmd.size() == 1)
        return (sendError(fd, ERR_NEEDMOREPARAMS(cmd.at(0))));
    chnlName = cmd.at(1);
    chnl = getChannel(chnlName);
    if (!chnl)
        return (sendError(fd, ERR_NOSUCHCHANNEL(chnlName)));
    if (cmd.size() == 2) {
        if (chnl->getTopic().empty())
            return (sendError(fd, RPL_NOTOPICSET(user->getNickname(), chnl->getChnlName())));
        sendResponse(fd, RPL_TOPIC(user->getNickname(), chnl->getChnlName(), chnl->getTopic()));
        sendResponse(fd, RPL_TOPICINFO(user->getNickname(), chnl->getChnlName(),
            chnl->getTopicSetter(), longToStr(chnl->getTimeCreated())));
        return ;
    }
    if (chnl->getRestrictTopic() && !chnl->isModerator(user->getNickname()))
        return (sendError(fd, ERR_CHANOPRIVSNEEDED(chnlName)));

    if (cmd.at(2).length() > 80)
        return (sendError(fd, ERR_LINETOOLONG(user->getNickname())));

    chnl->setTopic(cmd.at(2));
    chnl->setTopicSetter(user->getNickname());
    chnl->privmsg(clients, RPL_TOPICSET(user->getNickname(), user->getUsername(), user->getIp(), chnl->getChnlName(), cmd.at(2)));
}