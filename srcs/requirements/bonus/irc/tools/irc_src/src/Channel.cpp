#include "../inc/Channel.hpp"

Channel::Channel(std::string chnlName)
	: chnlName(chnlName)
{
	hasPassword = false;
	isInviteOnly = false;
	restrictTopic = false;
	maxMembers = 0;
	nbMembers = 0;
	timeCreated = time(0);
}


void    Channel::addUser(std::string user)
{
	users.push_back(user);
	nbMembers++;
}

void    Channel::addModr(std::string user)
{
	moderators.push_back(user);
}

std::string	Channel::getTopic(void)
{
	return (topic);
}

std::string Channel::getChnlName(void)
{
	return (chnlName);
}

std::vector<std::string>	&Channel::getUsers()
{
	return (users);
}

std::vector<std::string>	&Channel::getModerators()
{
	return (moderators);
}

std::string Channel::getPassword(void)
{
	return (password);
}

bool	Channel::getHasPassword(void)
{
	return (hasPassword);
}

bool	Channel::getIsInviteOnly(void)
{
	return (isInviteOnly);
}

int	Channel::getMaxMembers(void)
{
	return (maxMembers);
}

int	Channel::getNbMembers(void)
{
	return (nbMembers);
}

long	Channel::getTimeCreated(void)
{
	return (timeCreated);
}

bool	Channel::getRestrictTopic(void)
{
	return (restrictTopic);
}

std::string	Channel::getUsersAsString(void)
{
	std::vector<std::string>::iterator	it;
	std::string							res;

	for (it = users.begin(); it != users.end(); it++) {
		if (!isModerator(*it))
			res += *it + " ";
	}
	return (res);
}

std::string	Channel::getModeratorsAsString(void)
{
	std::vector<std::string>::iterator	it;
	std::string							res;

	for (it = moderators.begin(); it != moderators.end(); it++)
		res += "@" + *it + " ";
	return (res);

}

std::string	Channel::getTopicSetter(void)
{
	return (topicSetter);
}

//	Setters
void	Channel::setIsInviteOnly(bool stat)
{
	isInviteOnly = stat;
}

void	Channel::setRestrictTopic(bool stat)
{
	restrictTopic = stat;
}

void	Channel::setTopic(std::string topic)
{
	this->topic = topic;
}

void	Channel::setTopicSetter(std::string setter)
{
	topicSetter = setter;
}

void	Channel::setHasPassword(bool stat)
{
	hasPassword = stat;
}

void	Channel::setPassword(std::string password)
{
	this->password = password;
}

void	Channel::setMaxMembers(int nb)
{
	this->maxMembers = nb;
}
void	Channel::setNbMembers(int nb)
{
	this->nbMembers = nb;
}

bool    Channel::clientInChannel(std::string nickname)
{
    return (std::find(getUsers().begin(), getUsers().end(), nickname) != getUsers().end());
}

bool    Channel::isModerator(std::string nickname)
{
    return (std::find(getModerators().begin(), getModerators().end(), nickname) != getModerators().end());
}

int	Channel::addModerator(std::string nickname)
{
	std::vector<std::string>::iterator	it;

	it = std::find(getUsers().begin(), getUsers().end(), nickname);
	if (it == getUsers().end())
		return (-1);
	it = std::find(getModerators().begin(), getModerators().end(), nickname);
	if (it != getModerators().end())
		return (1);
	moderators.push_back(nickname);
	return (0);
}

int	 Channel::rmvModerator(std::string nickname)
{
	std::vector<std::string>::iterator	it;

	it = std::find(getUsers().begin(), getUsers().end(), nickname);
	if (it == getUsers().end())
		return (-1);
	it = std::find(getModerators().begin(), getModerators().end(), nickname);
	if (it == getModerators().end())
		return (1);
	moderators.erase(it);
	return (0);
}
