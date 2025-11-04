# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/14 16:40:32 by ichikawahik       #+#    #+#              #
#    Updated: 2025/11/04 17:50:39 by ichikawahik      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#############
# Variables #
#############

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -Iincludes
LIBS = -lreadline
SRCS = srcs/main.c\
       srcs/error_handler/error.c\
       srcs/tokenizer/tokenize.c\
       srcs/destructor.c\
       srcs/tokenizer/expand.c\
       srcs/parser/parse.c

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
    CFLAGS += -I$(RLDIR)/include
    LDFLAGS = -L$(RLDIR)/lib
endif
