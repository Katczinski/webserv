NAME 		= webserv

SRCS		= main.cpp\
			srcs/Sockets/Socket.cpp\
			srcs/Servers/Server.cpp\
			srcs/Cluster/Cluster.cpp\
			srcs/Client/Client.cpp\
			srcs/Config/Config.cpp\
			srcs/Config/Location.cpp\
			srcs/Config/parser.cpp\
			srcs/HTTP\ requests/Response.cpp\
			srcs/HTTP\ requests/http_header.cpp\
			srcs/CGI/CGI.cpp


OBJS		= $(SRCS:.cpp=.o)

HEADER		= includes/Sockets/Socket.hpp\
			includes/Servers/Server.hpp\
			includes/Cluster/Cluster.hpp\
			includes/Client/Client.hpp\
			includes/Config/Config.hpp\
			includes/Config/Location.hpp\
			includes/Config/parser.hpp\
			includes/HTTP\ requests/Response.hpp\
			includes/CGI/CGI.hpp


INC			= -Iincludes/Sockets\
			 -Iincludes/Servers\
			 -Iincludes/Config\
			 -Iincludes/Client\
			 -Iincludes\
			 -Iincludes/HTTP\ requests\
			 -Iincludes/Cluster\
			 -Iincludes/CGI
			 
CC			= clang++

FLAGS		= -std=c++98

all:		$(NAME)

$(NAME):	$(SRCS) $(HEADER)
			$(CC) $(FLAGS) $(INC) $(SRCS) -o $(NAME)

%.o:		%.cpp
			$(CC) -g $(FLAGS) $(INC) -c $< -o $@

debug: FLAGS += -fsanitize=address -g
debug: all

clean:
			rm -rf $(OBJS)

fclean:		clean
			rm -rf $(NAME)

re:			fclean all

.PHONY:		all clean fclean re