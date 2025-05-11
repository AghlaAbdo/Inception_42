#include "../inc/Server.hpp"
#include "../inc/Iexception.hpp"
#include "../inc/utils.hpp"



Server::~Server()
{
    close(server_fd);
    for (size_t i = 0; i < clients.size(); i++)
        close(clients[i].getFd());
}

// getters

Client& Server::getClient(int fd)
{
    size_t  i;

    for (i = 0; i < clients.size(); i++)
    {
        if (clients[i].getFd() == fd)
            return (clients[i]);
    }
    return (clients[i]);
}


// constructor

Server::Server(int port, std::string password)
{
    struct sockaddr_in  server_addr;
    int                 opt;
    pollfd              pf;

    opt = 1;
    this->port = port;
    this->password = password;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        throw Iexception("Socket creation error!");
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
        throw Iexception("Setting socket to reuse error!");
    if (fcntl(server_fd, F_SETFL, O_NONBLOCK) < 0)
        throw Iexception("Set socket to non block mode error!");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        throw Iexception("Binding socket error!");
    if (listen(server_fd, SOMAXCONN) < 0)
        throw Iexception("Listening for connections error!");
    pf.fd = server_fd;
    pf.events = POLLIN;
    pf.revents = 0;
    poll_fds.push_back(pf);
    std::cout << "The server is listening on port " << port << "..." << std::endl;
}

// methods

void    Server::acceptClient()
{
    int                 client_fd;
    sockaddr_in         client_addr;
    socklen_t           len;
    pollfd              pf;

    len = sizeof(client_addr);
    if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len)) < 0)
        throw Iexception("Accepting new connection error!");
    if (fcntl(client_fd, F_SETFL, O_NONBLOCK) < 0)
        throw Iexception("Set socket to non block mode error!");
    std::cout << "Client " <<client_fd << " connected!\n";
    Client client = Client(client_fd, inet_ntoa(client_addr.sin_addr));
    pf.fd = client_fd;
    pf.events = POLLIN;
    pf.revents = 0;
    clients.push_back(client);
    poll_fds.push_back(pf);
}

void    Server::readData(int fd)
{
    char        request[1024];
    std::string cmd;
    int         e;
    std::vector<Client>::iterator   client;

    memset(request, 0, 1024);
    e = recv(fd, &request, 1023, 0);
    if (e < 0)
        throw Iexception("Recv system call error!");
    client = getClientP(fd, clients);
    request[e] = '\0';
    cmd = request;
    removeChar(cmd, '\r');
    client->addBuffer(cmd);
    if (cmd.find('\n') != std::string::npos)
    {
        parseCmd(client->getBuffer(), fd);
        exec(fd);
        client->setBuffer("");
    }
}

void    Server::listening()
{
    while (!terminate)
    {
        if ((poll(&poll_fds[0], poll_fds.size(), -1)) < 0)
            throw Iexception("Server closed!");
        for (size_t i = 0; i < poll_fds.size(); i++)
        {
            if (poll_fds[i].revents == 17)
                quit(poll_fds[i].fd);
            if (poll_fds[i].revents == POLLIN)
            {
                if (poll_fds[i].fd == server_fd)
                    acceptClient();
                else
                    readData(poll_fds[i].fd);
            }
        }
    }
}

void    Server::exec(int fd)
{
    std::vector< std::vector<std::string> >     cmds;
    std::vector<std::string>                    cmd;
    Client                                      *client;

    cmds = getClient(fd).getCmd();
    client = &(getClient(fd));
    if (!cmds.size())
        return ;
    for (size_t i = 0; i < cmds.size(); i++)
    {
        cmd = cmds[i];
        if (!cmd.size())
            return ;
        if (cmd[0] == "pass" || cmd[0] == "PASS")
            pass(fd, cmd);
        else if (cmd[0] == "nick" || cmd[0] == "NICK")
            nick(fd, cmd);
        else if (cmd[0] == "user" || cmd[0] == "USER")
            user(fd, cmd);
        else if (cmd[0] == "quit" || cmd[0] == "QUIT")
            quit(fd);
        else if (cmd[0] == "PONG")
            return ;
        else
        {
            if (!client->getIsLogin())
                sendError(fd, ERR_NOTREGISTERED(client->getNickname()));
            else if (cmd[0] == "join" || cmd[0] == "JOIN")
                join(fd, cmd);
            else if (cmd[0] == "mode" || cmd[0] == "MODE") {
                if (cmd.size() < 2)
                    return (sendError(fd, ERR_MODNEEDPARAMS(getClient(fd).getNickname())));
                Channel *chnl = getChannel(cmd[1]);
                if (!chnl)
                return (sendError(fd, ERR_CHANNELNOTFOUND(getClient(fd).getNickname(), cmd.at(1))));
                chnl->mode(fd, &getClient(fd), clients, cmd);
            }
            else if (cmd[0] == "invite" || cmd[0] == "INVITE")
                invite(fd, cmd);
            else if (cmd[0] == "kick" || cmd[0] == "KICK")  
                kick(fd, cmd);
            else if (cmd[0] == "privmsg" || cmd[0] == "PRIVMSG")
                privmsg(fd, cmd);
            else if (cmd[0] == "topic" || cmd[0] == "TOPIC")
                topic(fd, cmd);
            else if (cmd[0] == "part" || cmd[0] == "PART")
                part(fd, clients, cmd);
            else
            {
                std::string tmp = "*";
                sendError(fd, ERR_UNKNOWNCOMMAND(tmp));
            }
        }
    }
}

void    Server::signalHandler(int signal)
{
    (void)signal;
}

void    Server::parseCmd(std::string cmd, int fd)
{
    Client *client;

    client = &(getClient(fd));
    client->getCmd().clear();
    client->setCmd(tokenizer(cmd));
}

// pass command

void    Server::pass(int fd, std::vector<std::string> &cmd)
{
    Client  *client;

    client = &(getClient(fd));
    if (cmd.size() == 1)
        return(sendError(fd, ERR_NEEDMOREPARAMS(cmd[0])));
    if (!getClient(fd).getIsRegistered())
    {
        if (password != cmd[1] || cmd.size() > 2)
            sendError(fd, ERR_INCORPASS(client->getNickname()));
        else
            client->setRegistred(true);
    }
    else
        sendError(fd, ERR_ALREADYREGISTRED(client->getNickname()));
}



void    Server::changeNickInChannels(std::string old_nick, std::string new_nick)
{
    std::vector<std::string>::iterator    it;
    for (size_t i = 0; i < channels.size(); i++)
    {   
        it = std::find(channels[i].getUsers().begin(), channels[i].getUsers().end(), old_nick);
        if (it != channels[i].getUsers().end())
            *it = new_nick;
        it = std::find(channels[i].getModerators().begin(), channels[i].getModerators().end(), old_nick);
        if (it != channels[i].getModerators().end())
            *it = new_nick;
    }
}

bool    Server::inUseNickname(std::string &nickname)
{
    for (size_t i = 0; i < clients.size(); i++)
    {
        if (clients[i].getNickname() == nickname)
            return (true);
    }
    return (false);
}

bool    Server::validNickname(int fd, std::string &nickname)
{
    std::string tmp;

    tmp = "_";
    if (!validNaming(nickname))
        return (sendError(fd, ERR_ERRONEUSNICKNAME(tmp)), false);
    if (inUseNickname(nickname))
        return (sendError(fd, ERR_NICKNAMEINUSE(nickname)), false);
    return (true);
}

// utils

Channel    *Server::getChannel(std::string chnlName)
{
    for (size_t i = 0; i < channels.size(); i++)
    {
        if (channels.at(i).getChnlName() == chnlName)
            return (&(channels.at(i)));
    }
    return (NULL);
}

bool    Server::isChannelExist(std::string &channel)
{
    std::vector<Channel>::iterator  it;

    for (it = channels.begin(); it < channels.end(); it++)
    {
        if (it->getChnlName() == channel)
            return (true);
    }
    return (false);
}

bool    Server::clientInChannel(std::string nickname, std::string channel)
{
    Channel *chnl = getChannel(channel);
    return (std::find(chnl->getUsers().begin(), chnl->getUsers().end(), nickname) != chnl->getUsers().end());
}

bool    Server::isModerator(std::string nickname, std::string channel)
{
    Channel *chnl = getChannel(channel);
    return (std::find(chnl->getModerators().begin(), chnl->getModerators().end(), nickname) != chnl->getModerators().end());
}

Channel&    Server::getChannelAt(int i)
{
    return (channels[i]);
}

std::vector<Channel>::iterator  Server::getChannelIt(std::string channel)
{
    std::vector<Channel>::iterator  it;

    for (it = channels.begin(); it < channels.end(); it++)
    {
        if (it->getChnlName() == channel)
            return (it);
    }
    return (it);
}
