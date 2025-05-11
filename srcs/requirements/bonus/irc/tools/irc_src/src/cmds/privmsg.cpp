#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"

void        Server::privmsg(int fd, std::vector<std::string> &cmd)
{
	Client  *receiver;
	Client  *sender;
	Channel *chnl;

	if (cmd.size() == 1)
		return ;
	sender = &(getClient(fd));
	if (cmd.at(1)[0] == '#') {
		chnl = getChannel(cmd.at(1));
		if (!chnl)
			return (sendError(fd, ERR_NOSUCHCHANNEL(cmd.at(1))));
		if (!chnl->clientInChannel(sender->getNickname()))
			return (sendError(fd, ERR_CANTSENDINCHNL(sender->getIp(), sender->getNickname(), cmd.at(1))));
		for (size_t i = 0; i < chnl->getUsers().size(); i++)
		{
			receiver = getClient(chnl->getUsers()[i]);
			if (receiver->getFd() != fd)
				sendResponse(receiver->getFd(), RPL_PRIVMSGCHNL(sender->getNickname(), chnl->getChnlName(), cmd[2]));
		}
	}
	else {
		receiver = getClient(cmd.at(1));
		if (!receiver)
			return (sendError(fd, ERR_NOSUCHUSER(cmd.at(1))));
		if (receiver->getFd() != fd)
			sendResponse(receiver->getFd(), RPL_PRIVMSGUSER(sender->getNickname(), receiver->getNickname(), cmd[2]));
	}
}

void    Channel::privmsg(std::vector<Client> clients, std::string msg)
{
	std::vector<Client>::iterator	it;

	for (it = clients.begin(); it != clients.end(); it++) {
		if (clientInChannel(it->getNickname())) {
			sendResponse(it->getFd(), msg);
		}
	}
}
