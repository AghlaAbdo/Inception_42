#include "../../inc/Server.hpp"

Client* Server::getClient(std::string nickName)
{
	size_t  i;

	for (i = 0; i < clients.size(); i++)
	{
		if (clients[i].getNickname() == nickName)
			return (&clients[i]);
	}
	return (NULL);
}

//  join command
void Server::join(int fd, std::vector<std::string> &cmd)
{
	Client  		*user = &getClient(fd);
	std::vector<Channel>::iterator		it;
	std::vector<std::string>::iterator  strIt;
	bool					hasInvite = false;


	if (cmd.size() < 2) {
		return (sendError(fd, ERR_JOINNOPARAMS(user->getIp(), user->getNickname())));
	}
	if (cmd.at(1)[0] != '#')
		return (sendError(fd, ERR_NOSUCHCHANNEL(cmd.at(1))));
	if (cmd.at(1).length() > 80)
		return (sendError(fd, ERR_LINETOOLONG(user->getNickname())));
	for (it = channels.begin(); it != channels.end(); it++)
	{
		if (cmd.at(1) == it->getChnlName())
		{
			if (clientInChannel(user->getNickname(), it->getChnlName()))
				return ;
			if (user->getNickname() != "bot")
			{
				if (it->getHasPassword() && (cmd.size() <= 2 || cmd.at(2) != it->getPassword())) {
					sendError(fd, ERR_INCORCHNLPASS(user->getNickname(), cmd.at(1)));
					return ; 
				}
				else if (it->getIsInviteOnly()) {
					for (strIt = user->getInvites().begin(); strIt != user->getInvites().end(); strIt++) {
						if (*strIt == it->getChnlName()) {
							hasInvite = true;
							break ;
						}
					}
					if (!hasInvite) {
						sendError(fd, ERR_INVITEONLYCHAN(user->getNickname(), cmd.at(1)));
						return ;
					}
					else
						user->getInvites().erase(strIt);
				}
				else if (it->getMaxMembers() && it->getNbMembers() >= it->getMaxMembers()) {
					sendError(fd, ERR_CHANNELISFULL(user->getNickname(), cmd.at(1)));
					return ;
				}
			}
			it->addUser(user->getNickname());
			if (user->getNickname() == "bot")
				channels.back().addModr(user->getNickname());
			if (user->getNickname() != "bot")
			{
				std::string	operators;
				std::string	users;

				it->privmsg(clients, RPL_JOINMSG(user->getNickname(), user->getUsername(), user->getIp(), cmd.at(1)));
				sendResponse(fd, RPL_NAMREPLY(user->getNickname(), cmd.at(1), it->getModeratorsAsString() + it->getUsersAsString()));
				sendResponse(fd, RPL_ENDOFNAMES(user->getNickname(), cmd.at(1)));
			}
			return ;
		}
	}
	if (user->getNickname() != "bot")
	{
		channels.push_back(Channel(cmd.at(1)));
		channels.back().addUser(user->getNickname());
		channels.back().addModr(user->getNickname());
		sendResponse(fd, RPL_JOINMSG(user->getNickname(), user->getUsername(), user->getIp(), cmd.at(1)));
		sendResponse(fd, RPL_NAMREPLY(user->getNickname(), cmd.at(1),
			channels.back().getModeratorsAsString() + channels.back().getUsersAsString()));
		sendResponse(fd, RPL_ENDOFNAMES(user->getNickname(), cmd.at(1)));
	}
	else
	{
		std::string no_channel = "You are not a bot, you can't create this channel\n";
		send(fd, no_channel.c_str(), no_channel.length(), 0);
	}
}
