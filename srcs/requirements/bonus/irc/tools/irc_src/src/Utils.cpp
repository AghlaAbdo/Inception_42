# include "../inc/utils.hpp"
# include "../inc/Client.hpp"
# include "../inc/Iexception.hpp"

bool validPort(const std::string &port)
{
    if (port.length() < 1 || port.length() > 5)
        return (false);
    
    for (size_t i = 0; i < port.length(); i++)
    {
        if (!std::isdigit(port[i]))
            return (false);
    }
    return (true);
}

bool    validPass(const std::string &password)
{
    for (size_t i = 0; i < password.length(); i++)
    {
        if (std::isspace(password[i]))
            return (false);
    }
    return (password.length() > 2 && password.length() < 15);
}

std::vector<Client>::iterator  getClientP(int fd, std::vector<Client> &clients)
{
    std::vector<Client>::iterator   it;

    for (it = clients.begin(); it != clients.end(); it++)
    {
        if (it->getFd() == fd)
            return (it);
    }
    return (it);
}

std::vector<pollfd>::iterator  getPoll(int fd, std::vector<pollfd> &poll_fds)
{
    std::vector<pollfd>::iterator   it;

    for (it = poll_fds.begin(); it != poll_fds.end(); it++)
    {
        if (it->fd == fd)
            return (it);
    }
    return (it);
}

std::vector<std::vector<std::string> > tokenizer(std::string &line)
{
    std::vector<std::vector<std::string> >  cmds;
    size_t start = 0, count;

    while ((count = line.find('\n', start)) != std::string::npos)
    {
        std::string tmp1 = line.substr(start, count - start);
        start = count + 1; 

        std::vector<std::string> cmd;
        size_t pos = tmp1.find(':');
        
        std::stringstream ss(tmp1.substr(0, pos));
        std::string token;
        while (ss >> token)
            cmd.push_back(token);
        
        if (pos != std::string::npos) 
        {
            cmd.push_back(tmp1.substr(pos + 1));
        }

        cmds.push_back(cmd);
    }
    
    return cmds;
}


void    sendError(int fd, std::string msg)
{
    if (send(fd, msg.c_str(), msg.length(), 0) < 0)
        throw Iexception("Send system call error!");
}

void    sendResponse(int fd, std::string msg)
{
    if (send(fd, msg.c_str(), msg.length(), 0) < 0)
        throw Iexception("Send system call error!");
}

bool    validNaming(std::string &nickname)
{   
    if (!isalpha(nickname[0]))
        return (false);
    if (nickname.length() > 15)
        return (false);
    for (size_t i = 1; i < nickname.length(); i++)
    {
        if (!isalnum(nickname[i]) && nickname[i] != '_')
            return (false);
    }
    return (true);
}

bool    usernameSetted(Client *client)
{
    return (client->getUsername() != "");
}

std::string longToStr(long num)
{
    std::stringstream ss;
    ss << num;
    return ss.str();
}

int stringToInt(const std::string& str) {
    std::stringstream ss(str);
    int number;

    ss >> number;
    if (ss.fail())
        return (-1);
    
    return number;
}

bool    validParams(std::vector<std::string> &cmd)
{
    if (!validNaming(cmd[1]))
        return (false);
    return (cmd[2].length() < 15 && cmd[3].length() < 15 && cmd[4].length() < 15);
}

void removeChar(std::string &str, char ch) 
{
    size_t pos;
    while ((pos = str.find(ch)) != std::string::npos) 
    {
        str.erase(pos, 1);
    }
}
