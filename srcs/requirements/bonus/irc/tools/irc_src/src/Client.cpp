# include "../inc/Client.hpp"

Client::Client()
{

}

Client::Client(int client_fd, std::string ip)
{
    this->client_fd = client_fd;
    this->ip = ip;
    isRegistred = false;
    isLogin = false;
    nickname = "_";
}

// getters

int Client::getFd() const
{
    return (client_fd);
}

std::string Client::getUsername() const
{
    return (username);
}

bool Client::getIsRegistered() const
{
    return (isRegistred);
}

bool Client::getIsLogin() const
{
    return (isLogin);
}


std::string    Client::getNickname() const
{
    return (nickname);
}

std::string  Client::getBuffer() const
{
    return (buffer);
}

std::vector< std::vector<std::string> >    &Client::getCmd()
{
    return (cmd);
}

// setters

void    Client::setNickname(std::string &nickname)
{
    this->nickname = nickname;
}

void    Client::setRegistred(bool bl)
{
    isRegistred = bl;
}

void    Client::setCmd(std::vector< std::vector<std::string> > cmd)
{
    this->cmd = cmd;
}

void    Client::setUsername(std::string &username)
{
    this->username = username;
}

void    Client::setMode(std::string &mode)
{
    this->mode = mode;
}

void    Client::setUnused(std::string &unused)
{
    this->unused = unused;
}

void    Client::setRealname(std::string &realname)
{
    this->realname = realname;
}

void    Client::setLogin(bool isLogin)
{
    this->isLogin = isLogin;
}

void    Client::addBuffer(std::string &cmd)
{
    buffer = buffer + cmd;
}

void    Client::setBuffer(std::string buffer)
{
    this->buffer = buffer;
}

// by Aghla
std::vector<std::string>    &Client::getInvites(void)
{
    return (invites);
}

std::string Client::getIp(void)
{
    return (ip);
}

void    Client::addInvite(std::string invite)
{
    invites.push_back(invite);
}

void    Client::rmvInvite(std::string invite)
{
    std::vector<std::string>::iterator  it;

    for (it = invites.begin(); it != invites.end(); it++)
    {
        if (*it == invite)
            invites.erase(it);
    }
}
