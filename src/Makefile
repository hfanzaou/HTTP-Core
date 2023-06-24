# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hfanzaou <hfanzaou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/09 22:12:40 by ebensalt          #+#    #+#              #
#    Updated: 2023/06/23 01:51:03 by hfanzaou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv
CPP			= Socket/main.cpp \
			  Socket/Socket.cpp \
			  Request/Request.cpp \
			  Response/Get.cpp \
			  Response/Response.cpp \
			  config/ConfigParse/Config.cpp \
			  config/ConfigParse/Location.cpp \
			  config/ConfigParse/ServerConfig.cpp
CC			= c++
CPPFLAGS	= -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3
O			= $(CPP:.cpp=.o)
RM			= rm -rf
%.o			: %.cpp
	@$(CC) $(CPPFLAGS) -c -o $@ $<
all			: $(NAME)
$(NAME)		: $(O)
	@$(CC) $(CPPFLAGS) $(O) -o $(NAME)
clean		:
	@$(RM) $(O)
fclean		: clean
	@$(RM) $(NAME)
re			: fclean all
