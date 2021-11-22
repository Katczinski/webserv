NAME 		= webserv

SRCS		= main.cpp\
			srcs/Sockets/SimpleSocket.cpp\
			srcs/Sockets/BindingSocket.cpp\
			srcs/Sockets/ConnectingSocket.cpp\
			srcs/Sockets/ListeningSocket.cpp\
			srcs/Servers/SimpleServer.cpp\
			srcs/Servers/WebServer.cpp


OBJS		= $(SRCS:.cpp=.o)

HEADER		= includes/websockets.hpp\
			includes/Sockets/SimpleSocket.hpp\
			includes/Sockets/BindingSocket.hpp\
			includes/Sockets/ConnectingSocket.hpp\
			includes/Sockets/ListeningSocket.hpp\
			includes/Servers/SimpleServer.hpp\
			includes/Servers/WebServer.hpp
			
INC			= -Iincludes/Sockets -Iincludes/Servers -Iincludes

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