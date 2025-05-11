#include "../../inc/Server.hpp"
#include "../../inc/Channel.hpp"
#include "../../inc/Client.hpp"

int Channel::modeI(char mode)
{

	if (mode == '+') {
		if (getIsInviteOnly())
			return (0);
		setIsInviteOnly(true);
		return (1);
	}
	else {
		if (!getIsInviteOnly())
			return (0);
		setIsInviteOnly(false);
	}

	return (1);
}

int Channel::modeT(char mode)
{
	if (mode == '+') {
		if (getRestrictTopic())
			return (0);
		setRestrictTopic(true);
		return (1);
	}
	else {
		if (!getRestrictTopic())
			return (0);
		setRestrictTopic(false);
	}

	return (1);
}

int	Channel::modeL(int fd, std::vector<std::string> &cmd, char mode, size_t prmIdx)
{
	if (mode == '+') {
		if (prmIdx >= cmd.size())
			return (sendError(fd, ERR_NEEDMODEPARM(getChnlName(), "(l)")), -1);
		
		int nb = stringToInt(cmd.at(prmIdx));
		if (nb <= 0)
			return (sendError(fd, ERR_INVALIDMODEPARM(getChnlName(), "(l)")), -1);
		setMaxMembers(nb);
		return (0);
	}
	else {
		if (getMaxMembers()) {
			setMaxMembers(0);
			return (0);
		}
		return (-1);
	}
	return (0);
}

int	Channel::modeK(int fd, std::vector<std::string> &cmd, char mode, size_t prmIdx)
{
	if (mode == '+') {
		if (prmIdx >= cmd.size())
			return (sendError(fd, ERR_NEEDMODEPARM(getChnlName(), "k")), -1);
		setPassword(cmd.at(prmIdx));
		setHasPassword(true);
		return (0);
	}
	else {
		if (prmIdx >= cmd.size())
			return (sendError(fd, ERR_NEEDMODEPARM(getChnlName(), "k")), -1);
		else if (cmd.at(prmIdx) != getPassword())
			return (sendError(fd, ERR_KEYSET(getChnlName())), 1);
		setHasPassword(false);
		setPassword("");
	}
	return (0);
}

int	Channel::modeO(int fd, std::vector<std::string> &cmd, char mode, size_t prmIdx)
{
	int	flag;

	if (mode == '+') {
		if (prmIdx >= cmd.size())
			return (sendError(fd, ERR_NEEDMODEPARM(getChnlName(), "(o)")), -1);
		flag = addModerator(cmd.at(prmIdx));
		if (flag == -1)
			return (sendError(fd, ERR_NOSUCHNICK(getChnlName(), cmd.at(prmIdx))), 1);
		return (flag);
	}
	else {
		if (prmIdx >= cmd.size())
			return (sendError(fd, ERR_NEEDMODEPARM(getChnlName(), "(o)")), -1);
		flag = rmvModerator(cmd.at(prmIdx));
		if (flag == -1)
			return (sendError(fd, ERR_NOSUCHNICK(getChnlName(), cmd.at(prmIdx))), 1);
		return (flag);
	}
}

int	Channel::checkModeParms(int fd, Client *user, std::vector<std::string> &cmd)
{
	if (!clientInChannel(user->getNickname()))
		return (sendError(fd, ERR_NOTONCHANNEL(user->getNickname(), cmd.at(1))), -1);
	if (cmd.size() == 2) {
		std::string modes;
		if (getIsInviteOnly() || getRestrictTopic() || getMaxMembers() || getHasPassword()) {
			modes = "+";
			if (getIsInviteOnly())
				modes += "i";
			if (getRestrictTopic())
				modes += "t";
			if (getMaxMembers())
				modes += "l";
			if (getHasPassword())
				modes += "k";
		}
		sendResponse(fd, RPL_CHNLMODE(user->getNickname(), getChnlName(), modes));
		sendResponse(fd, RPL_CHNLDATE(user->getNickname() , getChnlName(), longToStr(getTimeCreated())));
		return (0);
	}
	if (!isModerator(user->getNickname()))
		return (sendError(fd, ERR_CHANOPRIVSNEEDED(cmd.at(1))), -1);
	return (0);
}

void    Channel::mode(int fd, Client *user, std::vector<Client> clients, std::vector<std::string> &cmd)
{
	std::string	modes;
	std::string	argK;
	std::string	argL;
	std::string	argO;
	char    	mode = '+';
	char		Omode = '+';
	size_t  	modeIdx = 2;
	bool		setI = false;
	bool		setT = false;
	bool		setL = false;
	bool		setK = false;
	bool		setO = false;

	if (checkModeParms(fd, user, cmd))
		return ;

	while (modeIdx < cmd.size()) {
		int	hasL = 0;
		int	hasK = 0;
		int	hasO = 0;
		for (size_t i = 0; i < cmd.at(modeIdx).length(); i++) {

			while (i < cmd.at(modeIdx).length() && (cmd.at(modeIdx)[i] == '+' || cmd.at(modeIdx)[i] == '-'))
				i++;
			if (cmd.at(modeIdx)[i -1] == '+' || cmd.at(modeIdx)[i -1] == '-')
				mode = cmd.at(modeIdx)[i -1];
			if (i < cmd.at(modeIdx).length()) {
				if (cmd.at(modeIdx)[i] == 'i') {
					if (!setI)
					setI = modeI(mode);
				}
				else if(cmd.at(modeIdx)[i] == 't') {
					if (!setT)
					setT = modeT(mode);
				}
				else if(cmd.at(modeIdx)[i] == 'l') {
					if (!setL) {
						if (!modeL(fd, cmd, mode, modeIdx +1 + hasK + hasO)) {
							setL = true;
							Omode = mode;
							if (mode == '+') {
								argL = cmd.at(modeIdx +1 + hasK + hasO);
								hasL = 1;
							}
						}
					}
				}
				else if (cmd.at(modeIdx)[i] == 'k') {
					if (!setK) {
						if (!(hasK = modeK(fd, cmd, mode, modeIdx +1 + hasL + hasO))) {
							argK = cmd.at(modeIdx +1 + hasL + hasO);
							setK = true;
						}
						if (hasK == -1)
							hasK = 0;
						else
							hasK = 1;
					}
				}
				else if (cmd.at(modeIdx)[i] == 'o') {
					if (!setO) {
						if (!(hasO = modeO(fd, cmd, mode, modeIdx +1 + hasL + hasK)) || hasO == 1) {
							if (hasO == 0)
								setO = true;
							Omode = mode;
							argO = cmd.at(modeIdx +1 + hasL + hasK);
						}
						if (hasO == -1)
							hasO = 0;
						else
							hasO = 1;
					}
				}
				else
					sendError(fd, ERR_NOSUCHMODE(user->getNickname(), getChnlName(), cmd.at(modeIdx)[i]));
			}
		}
			modeIdx += 1 + hasL + hasK + hasO;
			if (modeIdx >= cmd.size())
				break;
	}

	if ((setI && !getIsInviteOnly()) || (setT && !getRestrictTopic()) || (setL && !getMaxMembers())
			|| (setK && !getHasPassword()) || (setO && Omode == '-')) {
		modes += "-";
		if (setI && !getIsInviteOnly())
			modes += "i";
		if (setT && !getRestrictTopic())
			modes += "t";
		if (setL && !getMaxMembers())
			modes += "l";
		if (setK && !getHasPassword())
			modes += "k";
		if (setO && Omode == '-')
			modes += "o";
	}
	if ((setI && getIsInviteOnly()) || (setT && getRestrictTopic()) || (setL && getMaxMembers())
			|| (setK && getHasPassword()) || (setO && Omode == '+')) {
		modes += "+";
		if (setI  && getIsInviteOnly())
			modes += "i";
		if (setT && getRestrictTopic())
			modes += "t";
		if (setL && getMaxMembers())
			modes += "l";
		if (setK && getHasPassword())
			modes += "k";
		if (setO && Omode == '+')
			modes += "o";
	}
	if (setL && getMaxMembers())
		modes += " " + argL;
	if (setK)
		modes += " " + argK;
	if (setO)
		modes += " " + argO;
	if (setI || setT || setL || setK || setO)
		privmsg(clients, RPL_SETINVITEMODE(user->getNickname(), user->getUsername(),
				getChnlName(), modes));
}
