NAME 		= webserv

SRCS		= main.cpp\
			srcs/SimpleSocket.cpp\
			srcs/BindingSocket.cpp\
			srcs/ConnectingSocket.cpp


OBJS		= $(SRCS:.cpp=.o)

HEADER		= includes/SimpleSocket.hpp\
			includes/BindingSocket.hpp\
			includes/ConnectingSocket.hpp


INC			= -Iincludes

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