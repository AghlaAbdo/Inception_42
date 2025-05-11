NAME = ircserv
FLAGS = -Wall -Wextra -Werror -std=c++98
O_DR	= objs/

SRC	= src/main.cpp			\
		src/Server.cpp		\
		src/Client.cpp		\
		src/Channel.cpp		\
		src/Iexception.cpp	\
		src/Utils.cpp		\
		src/cmds/mode.cpp	\
		src/cmds/join.cpp   \
		src/cmds/invite.cpp \
		src/cmds/kick.cpp	\
		src/cmds/privmsg.cpp	\
		src/cmds/topic.cpp		\
		src/cmds/part.cpp		\
		src/cmds/nick.cpp		\
		src/cmds/user.cpp		\
		src/cmds/quit.cpp

HEADERS = inc/Server.hpp		\
			inc/Client.hpp		\
			inc/utils.hpp		\
			inc/Iexception.hpp	\
			inc/Channel.hpp

SRCB = bonus/Bot.cpp \
		bonus/main.cpp \
		bonus/Iexception.cpp \

HEADERSB = bonus/Bot.hpp \
			bonus/Iexception.hpp

OBJB = $(SRCB:.cpp=.o)

OBJ = $(addprefix $(O_DR),$(SRC:.cpp=.o))


all : $(NAME)

bonus : $(OBJB)
	c++ $(OBJB) -o bot

bonus/%.o : bonus/%.cpp $(HEADERSB)
	c++ $(FLAGS) -c $< -o $@

$(NAME) : $(OBJ)
	c++ $(OBJ) -o $@

$(O_DR)%.o : %.cpp $(HEADERS)
	@mkdir -p $(@D)
	c++ $(FLAGS) -c $< -o $@

clean :
	rm -rf $(O_DR)
	rm -rf $(OBJB)

fclean : clean
	rm -rf $(NAME)
	rm -rf bot

re : fclean all