#pragma once

# include <map>
#include <fcntl.h>
# include "utils.hpp"
# include <algorithm>
# include "../inc/Client.hpp"
# include "../inc/Channel.hpp"

class Server
{
    int                         port;
    static int                  terminate;
    int                         server_fd;
    std::string                 password;
    std::vector<Client>         clients;
    std::vector<pollfd>         poll_fds;

    std::vector<Channel>        channels;


    public:
        Server(int port, std::string password);
        ~Server();
        
        // getters
        Client& getClient(int fd);
        Client* getClient(std::string nickName);
        Channel*    getChannel(std::string chnlName);
        Channel&    getChannelAt(int i);
        std::vector<Channel>::iterator  getChannelIt(std::string channel);

        void        listening();
        static void signalHandler(int signal);
        void        acceptClient();
        void        readData(int fd);
        void        parseCmd(std::string cmd, int fd);
        void        exec(int fd);
        void        pass(int fd, std::vector<std::string> &cmd);
        void        nick(int fd, std::vector<std::string> &cmd);
        void        user(int fd, std::vector<std::string> &cmd);
        bool        validNickname(int fd, std::string &nickname);
        bool        inUseNickname(std::string &nickname);
        void        quit(int fd);
        void        kick(int fd, std::vector<std::string> &cmd);
        void        invite(int fd,  std::vector<std::string> &cmd);
        bool        isChannelExist(std::string &channel);
        bool        clientInChannel(std::string nickname, std::string channel);
        bool        clientIsMod(std::string nickname, std::string channel);
        bool        isModerator(std::string nickname, std::string channel);
        void        privmsg(int fd, std::vector<std::string> &cmd);
        void    changeNickInChannels(std::string old_nick, std::string new_nick);

        // by Aghla
        void        join(int fd, std::vector<std::string> &cmd);
        void	    topic(int fd, std::vector<std::string> &cmd);
        void        part(int fd, std::vector<Client> &clients, std::vector<std::string> &cmd);

        void    sendNickReplay(Client *user, std::string newNick, int fd);
};

#define PREFIX ":IRCServer "
#define POSTFIX "\n"

//Errors :
#define ERR_NICKNAMEINUSE(target)						PREFIX "433 " + target + " :Nickname is already in use" POSTFIX 
#define ERR_NEEDMOREPARAMS(command)						PREFIX "461 " + command + " :Not enough parameters" POSTFIX  
#define ERR_UNKNOWNCOMMAND(command)						PREFIX "421 " + command + " :Unknown command" POSTFIX 
#define ERR_CHANOPRIVSNEEDED(channel)					": 482 " + channel + " :You're not a channel operator" POSTFIX
#define ERR_ERRONEUSNICKNAME(nick)						PREFIX "432 " + nick + " :Erroneus nickname" POSTFIX
#define ERR_USERNAME(USERNAME)						    PREFIX "468 " + USERNAME + " :Erroneus username" POSTFIX
#define ERR_NOTEXTTOSEND()								PREFIX "412 :No text to send" POSTFIX 
#define ERR_NOSUCHCHANNEL(channel)						PREFIX "403 " + channel + " :No such channel" POSTFIX 
#define ERR_NOSUCHNICK(channel, nick)					": 401 " + channel + " " + nick + " :No such nick/channel" + POSTFIX 
#define ERR_NOSUCHUSER(nick)        					": 401 " + nick + " :No such nick" + POSTFIX 
#define ERR_NOTONCHANNEL(target, channel)	   			PREFIX "442 " + target + " " + channel + " :You're not on that channel" POSTFIX 
#define ERR_NOTONSERVER(target, channel)	   			PREFIX "442 " + target + " " + channel + " :Target not on the server" POSTFIX 
#define ERR_USERNOTINCHANNEL(user, channel) 			PREFIX "441 " + user + " " + channel + " :They aren't on that channel" POSTFIX 
#define ERR_NONICKNAMEGIVEN(target)           			PREFIX "431 " + target + " :No nickname given" POSTFIX 
#define ERR_PASSWDMISMATCH(target)						PREFIX "464 " + target + ":Password incorrect" POSTFIX 
#define ERR_ALREADYREGISTRED(target)					PREFIX "462 " + target + " :You may not reregister" POSTFIX 
#define ERR_NOTREGISTERED(nickname)                     PREFIX "451 " + nickname + " :You have not registered!" POSTFIX
#define ERR_CHANNELISFULL(client, channel)				PREFIX " 471 " + client + " " + channel + " :Cannot join channel (+l)" POSTFIX
#define ERR_INVITEONLYCHAN(client, channel)				PREFIX " 473 " + client + " " + channel + " :Cannot join channel (+i)" POSTFIX
#define ERR_USERONCHANNEL(client, channel)				PREFIX "443 " + client + " " + channel + " :is already on channel" POSTFIX
#define ERR_INCORCHNLPASS(client, channel)				PREFIX " 475 " + client + " " + channel + " :Cannot join channel (+K)" POSTFIX
#define ERR_NEEDMODEPARM(channelname, mode)				": 696 " + channelname + " * You must specify a parameter for the key mode. " + mode + POSTFIX
#define ERR_INVALIDMODEPARM(channelname, mode)			": 696 " + channelname + " Invalid mode parameter. " + mode + POSTFIX
#define ERR_UNKNOWNMODE(nickname, channelname, mode)	": 472 " + nickname + " " + channelname + " " + mode + " :is not a recognised channel mode" + POSTFIX
#define ERR_INCORPASS(nickname)							": 464 " + nickname + " :Password incorrect !" + POSTFIX
#define ERR_JOINNOPARAMS(hostname, sender)              ":" + hostname + " 461 " + sender + " :Not enough parameters" + POSTFIX
#define ERR_MODNEEDPARAMS(user)                         ": 461 " + user + " :Not enough parameters." + POSTFIX

#define ERR_CHANNELNOTFOUND(nickname, channelname)      (": 403 " + nickname + " " + channelname + " :No such channel" + POSTFIX)
#define ERR_KEYSET(channelname)                         ": 467 #" + channelname + " Channel key already set. " + POSTFIX
#define ERR_KICK(client, target, channel)               (":" + client + "!~k@localhost KICK " + channel + " " + target + POSTFIX)
#define ERR_KICK_SP(client, target, channel, msg)       (":" + client + "!~k@localhost KICK " + channel + " " + target + " :" + msg + POSTFIX)
#define ERR_LINETOOLONG(nick)                               ": 417 " + nick + " Input line was too long" POSTFIX
#define ERR_CANTSENDINCHNL(ip, nick, chnl)              ":" + ip + " 404 " + nick + " " + chnl + " :Cannot send to channel" + POSTFIX
#define ERR_NOSUCHMODE(nick, chnl, mode)                ": 472 " + nick + " " + chnl + " " + mode + " :is not a recognised channel mode" + POSTFIX

//Replays :
#define RPL_WELCOME(sender, msg)               			PREFIX "001 " + sender + " : " + msg + POSTFIX 
#define RPL_NAMREPLY(nick, channel, users)    	        ": 353 " + nick + " @ " + channel + " :" + users + POSTFIX
#define RPL_PRIVMSGUSER(sender, target, msg)			":" + sender + " PRIVMSG " + target + " :" + msg + POSTFIX 
#define RPL_PRIVMSGCHNL(sender, target, msg)			":" + sender + " PRIVMSG " + target + " :" + msg + POSTFIX 
#define RPL_NICK(sender, nick)							":" + sender + " NICK " + nick + POSTFIX
#define RPL_NOTOPIC(sender, channel)					PREFIX " 331 " + sender + " " + channel + " :No topic is set" + POSTFIX
#define RPL_INVITING(nickname, targnick, targchan)  	": 341 " + nickname + " " + targnick + " " + targchan + POSTFIX
#define RPL_INVITE(sender, target, channel)				":" + sender + " INVITE " + target + " " + channel + POSTFIX
#define RPL_INVITING(nickname, targnick, targchan) 	 	": 341 " + nickname + " " + targnick + " " + targchan + POSTFIX
#define RPL_JOIN(sender, channel)						":" + sender + " JOIN :" + channel + POSTFIX
#define RPL_CHANGEMODE(hostname, channelname, mode)		(":" + hostname + " MODE " + channelname + " " + mode + POSTFIX)
#define RPL_UMODEIS(hostname, channelname, mode, user)	":" + hostname + " MODE " + channelname + " " + mode + " " + user + POSTFIX

#define RPL_SETINVITEMODE(nick, user, channel, arg)     ":" + nick + "!" + user + " MODE " + channel + " " + arg + POSTFIX
#define RPL_CHNLMODE(user, channel, arg)                ": 324 " + user + " " + channel + " " + arg + POSTFIX
#define RPL_CHNLDATE(user, channel, date)               ": 329 " + user + " " + channel + " " + date + POSTFIX

#define RPL_JOINMSG(nick, user, ipaddress, channelname) (":" + nick + "!" + user + "@" + ipaddress + " JOIN " + channelname + POSTFIX)
#define RPL_ENDOFNAMES(sender, channel)                    ": 366 " + sender + " " + channel + " :End of /NAMES list." POSTFIX
#define RPL_NOTOPICSET(nick, channel)                   ": 331 " + nick + " " + channel + " :No topic is set" + POSTFIX
#define RPL_TOPIC(nick, channel, topic)                 ": 332 " + nick + " " + channel + " :" + topic + POSTFIX
#define RPL_TOPICINFO(nick, channel, setter, date)      ": 333 " + nick + " " + channel + " " + setter + " " + date + POSTFIX
#define RPL_TOPICSET(nick, user, ip, channel, topic)    ":" + nick + "!" + user + "@" + ip + " TOPIC " + channel + " :" + topic + POSTFIX
#define RPL_PART(nick, user, ip, channel, msg)          ":" + nick + "!" + user + "@" + ip + " PART " + channel + " :" + msg + POSTFIX
#define RPL_NEWNICK(nick, newNick)                      ":" + nick +" NICK " + newNick + POSTFIX