# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/09 22:12:40 by ebensalt          #+#    #+#              #
#    Updated: 2023/07/20 00:48:20 by hfanzaou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
CPP			= main.cpp \
			  server/server.cpp   \
			  server/Socket.cpp    \
			  request/request.cpp   \
			  request/Post.cpp		\
			  Response/Response.cpp  \
			  ConfigParse/Config.cpp  \
			  ConfigParse/Location.cpp \
			  ConfigParse/ServerConfig.cpp \
			  cgi/cgi.cpp Response/utils.cpp 
CC			= c++
CPPFLAGS	= -Wall -Wextra -Werror -fsanitize=address -g3
O			= $(CPP:.cpp=.o)
RM			= rm -rf
%.o			: %.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<
all			: $(NAME)
$(NAME)		: $(O)
	$(CC) $(CPPFLAGS) $(O) -o $(NAME)
clean		:
	$(RM) $(O)
fclean		: clean
	$(RM) $(NAME)
re			: fclean all
