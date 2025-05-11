#pragma once

# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <unistd.h>
# include <iostream>
# include <cstring>
# include <exception>
# include <csignal> 
# include <cstdlib>
# include <sstream>
# include <vector>
# include <arpa/inet.h>
# include <poll.h>


class Client;

bool                                    validPort(const std::string &port);
bool                                    validPass(const std::string &password);
void                                    sendError(int fd, std::string msg);
void                                    sendResponse(int fd, std::string msg);
bool                                    validNaming(std::string &nickname);
bool                                    usernameSetted(Client *client);
std::vector< std::vector<std::string> > tokenizer(std::string &cmd);
std::vector<Client>::iterator           getClientP(int fd, std::vector<Client> &clients);
std::vector<pollfd>::iterator           getPoll(int fd, std::vector<pollfd> &poll_fds);
std::string                             longToStr(long num);
int                                     stringToInt(const std::string& str);
bool                                    validParams(std::vector<std::string> &cmd);
void                                    removeChar(std::string &str, char ch);






