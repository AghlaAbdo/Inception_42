#pragma once

# include "utils.hpp"
# include "Channel.hpp"

class Client
{
    int                         client_fd;
    std::string                 ip;
    bool                        isRegistred;
    bool                        isLogin;

    std::string                 nickname;
    std::string                 username;
    std::string                 mode;
    std::string                 unused;
    std::string                 realname;

    std::vector< std::vector<std::string> >     cmd;
    std::string                                 buffer;

    std::vector<std::string>    invites;

    public:
        Client();
        Client(int client_fd, std::string ip);

        // setters
        void    setNickname(std::string &nickname);
        void    setRegistred(bool bl);
        void    setCmd(std::vector< std::vector<std::string> > cmd);
        void    setUsername(std::string &username);
        void    setMode(std::string &mode);
        void    setUnused(std::string &unused);
        void    setRealname(std::string &realname);
        void    setLogin(bool isLogin);
        void    setBuffer(std::string buffer);
        void    addBuffer(std::string &cmd);

        void    addInvite(std::string invite);

        // getters
        int                         getFd() const;
        bool                        getIsRegistered() const;
        bool                        getIsLogin() const;
        std::string                 getNickname() const;
        std::string                 getUsername() const;
        std::string                 getBuffer() const;
        std::vector< std::vector<std::string> >    &getCmd();
        
        std::string                 getIp(void);
        std::vector<std::string>&    getInvites(void);

        void    rmvInvite(std::string invite);
};
