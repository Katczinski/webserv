NAME 		= webserv

SRCS		= main.cpp\
			srcs/Sockets/Socket.cpp\
			srcs/Servers/Server.cpp\
			srcs/Client/Client.cpp\
			srcs/Config/parserConfig.cpp


OBJS		= $(SRCS:.cpp=.o)

HEADER		= includes/Sockets/Socket.hpp\
			includes/Servers/Server.hpp\
			includes/Client/Client.hpp\
			includes/Config/Config.hpp
			
INC			= -Iincludes/Sockets -Iincludes/Servers -Iincludes/Config -Iincludes/Client -Iincludes

CC			= clang++

FLAGS		= -std=c++98 -Wall -Wextra -Werror

all:		$(NAME)

$(NAME):	$(SRCS) $(HEADER)
			$(CC) $(FLAGS) $(INC) $(SRCS) -o $(NAME)

%.o:		%.cpp
			$(CC) -g $(FLAGS) $(INC) -c $< -o $@

clean:
			rm -rf $(OBJS)

fclean:		clean
			rm -rf $(NAME)

re:			fclean all

.PHONY:		all clean fclean re