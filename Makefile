# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/08/24 07:58:27 by mgo               #+#    #+#              #
#    Updated: 2022/08/24 07:58:29 by mgo              ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC			=	gcc
#CFLAGS		=	-Wall -Wextra -Werror
#CFLAGS		+=	-fsanitize=address -g
RM			=	rm -rf

DIR_SERV		=	./ft_ttt_server/
SRC_LIST_SERV	=	main.c \
					client_session.c \
					server.c
SRC_SERV		=	$(addprefix $(DIR_SERV), $(SRC_LIST_SERV))
OBJ_SERV		=	$(SRC_SERV:.c=.o)
NAME_SERV		=	$(addprefix $(DIR_SERV), ft_ttt_server)

DIR_CLNT		=	./ft_ttt_client/
SRC_LIST_CLNT	=	main.c \
					client.c \
					tictactoe.c
SRC_CLNT		=	$(addprefix $(DIR_CLNT), $(SRC_LIST_CLNT))
OBJ_CLNT		=	$(SRC_CLNT:.c=.o)
NAME_CLNT		=	$(addprefix $(DIR_CLNT), ft_ttt_client)

%.o	:	%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME_SERV)	:	$(OBJ_SERV)
	$(CC) $(CFLAGS) $(OBJ_SERV) -o $(NAME_SERV)

$(NAME_CLNT)	:	$(OBJ_SERV)
	$(CC) $(CFLAGS) $(OBJ_SERV) -o $(NAME_CLNT)

.PHONY	:	s
s		:	$(NAME_SERV)

.PHONY	:	c
c		:	$(NAME_CLNT)

all		:	s c

clean	:
	$(RM) $(OBJ_SERV)
	$(RM) $(OBJ_CLNT)

fclean	: clean
	$(RM) $(NAME_SERV)
	$(RM) $(NAME_CLNT)

re	:	fclean all

.PHONY	:	all clean fclean re
