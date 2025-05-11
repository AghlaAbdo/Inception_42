#pragma once

// #include "../inc/Server.hpp"
// #include "Client.hpp"

# include <iostream>
# include <algorithm>
# include <vector>

class	Client;

class	Channel {
private:
	std::vector<std::string>	moderators;
	std::vector<std::string>	users;
	std::string					chnlName;
	std::string					topic;
	std::string					password;
	std::string				topicSetter;
	
	bool	hasPassword;
	bool	isInviteOnly;
	bool	restrictTopic;
	int		maxMembers;
	int		nbMembers;
	long	timeCreated;

	Channel(void);

	//	modes
	// bool	clientInChannel(std::string nickname);
	int		addModerator(std::string nickname);
	int		rmvModerator(std::string nickname);
	int		checkModeParms(int fd, Client *user, std::vector<std::string> &cmd);
	int		modeI(char mode);
	int		modeT(char mode);
	int		modeL(int fd, std::vector<std::string> &cmd, char mode, size_t prmIdx);
	int		modeK(int fd, std::vector<std::string> &cmd, char mode, size_t prmIdx);
	int		modeO(int fd, std::vector<std::string> &cmd, char mode, size_t prmIdx);


public:
	Channel(std::string chnlName);
	void	addUser(std::string user);
	void	addModr(std::string user);

	bool	isModerator(std::string nickname);
	bool	clientInChannel(std::string nickname);
	// getters
	std::string	getTopic(void);
	std::string	getChnlName(void);
	std::string	getPassword(void);
	bool		getHasPassword(void);
	bool		getIsInviteOnly(void);
	bool		getRestrictTopic(void);
	int			getMaxMembers(void);
	int			getNbMembers(void);
	long		getTimeCreated(void);
	std::vector<std::string>	&getUsers(void);
	std::vector<std::string>	&getModerators(void);
	std::string	getUsersAsString();
	std::string	getModeratorsAsString();
	std::string	getTopicSetter();

	//	setters
	void	setIsInviteOnly(bool stat);
	void	setRestrictTopic(bool stat);
	void	setTopic(std::string topic);
	void	setHasPassword(bool stat);
	void	setPassword(std::string password);
	void	setNbMembers(int nb);
	void	setMaxMembers(int nb);
	void	setTopicSetter(std::string setter);

	//	cmds
	void	mode(int fd, Client *user, std::vector<Client> clients, std::vector<std::string> &cmd);
	void	privmsg(std::vector<Client> clients, std::string msg);
};
