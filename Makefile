# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/14 16:40:32 by ichikawahik       #+#    #+#              #
#    Updated: 2025/10/31 21:17:05 by ichikawahik      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#############
# Variables #
#############

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LIBS = -lreadline
SRCS = srcs/main.c\
	   srcs/error.c\
	   srcs/tokenize.c\
	   srcs/destructor.c\
	   srcs/expand.c\
	   srcs/parse.c\

OBJS = $(SRCS:%.c=%.o)

#################
# General rules #
#################

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

test: all
	./test.sh

.PHONY: all clean fclean re test

##########################
# Platform Compatibility #
##########################

#Linux | Darwin
OS := $(shell uname -s)

ifeq ($(OS),Darwin)
	# command for macOS
	RLDIR = $(shell brew --prefix readline)
	INCLUDES = -I$(RLDIR)/includes
	LDFLAGS = -L$(RLDIR)/lib
endif