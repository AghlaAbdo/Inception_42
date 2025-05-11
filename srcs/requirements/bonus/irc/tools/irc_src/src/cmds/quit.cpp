#include "../../inc/Server.hpp"
#include "../../inc/Iexception.hpp"
#include "../../inc/utils.hpp"

void    Server::quit(int fd)
{
    Client  *client = &(getClient(fd));
    Channel *channel;
    std::vector<std::string>    *users;
    std::vector<std::string>    *moderators;
    size_t                      i;

    i = 0;
    while (i < channels.size())
    {
        channel = &(getChannelAt(i));
        users = &(channel->getUsers());
        moderators = &(channel->getModerators());
        if (clientInChannel(client->getNickname(), channel->getChnlName()))
        {
            users->erase(std::find(users->begin(), users->end(), client->getNickname()));
            channel->setNbMembers(channel->getNbMembers() - 1);
        }
        if (isModerator(client->getNickname(), channel->getChnlName()))
            moderators->erase(std::find(moderators->begin(), moderators->end(), client->getNickname()));
        if (!users->size())
        {
            channels.erase(getChannelIt(channel->getChnlName()));
            i = 0;
        }
        else
            i++;
    }
    close(fd);
    clients.erase(getClientP(fd, clients));
    poll_fds.erase(getPoll(fd, poll_fds));
    std::cout << "Client " << fd << " disconnected!" << std::endl;
}
