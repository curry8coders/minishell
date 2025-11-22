# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/14 16:40:32 by ichikawahik       #+#    #+#              #
#    Updated: 2025/11/22 15:29:43 by ichikawahik      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#############
# Variables #
#############

NAME = minishell
CC = cc
RLDIR = $(shell brew --prefix readline)
INCLUDES = -I includes -I$(RLDIR)/include
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LIBS = -lreadline -L$(RLDIR)/lib
SRCS = srcs/main.c\
       srcs/error_handler/error.c\
       srcs/tokenizer/tokenize.c\
       srcs/destructor.c\
       srcs/tokenizer/expand.c\
       srcs/parser/parse.c\
       srcs/redirection/redirect.c\
	   srcs/pipe/pipe.c\
	   srcs/exec/exec.c\
	   srcs/signal/signal.c\
	   srcs/builtin/builtin.c\
	   srcs/builtin/builtin_exit.c\
	   srcs/builtin/builtin_export.c\
	   srcs/builtin/builtin_unset.c\
	   srcs/builtin/builtin_env.c\
	   srcs/builtin/builtin_cd.c\
	   #srcs/builtin/builtin_echo.c
	   srcs/builtin/builtin_pwd.c\
	   srcs/hashstamp/map.c\
	   srcs/hashstamp/env.c\

OBJS = $(SRCS:%.c=%.o)

#################
# General rules #
#################

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(LIBS) -o $(NAME) $(OBJS)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# We may delete the following code when we push this project to 42's remote repository.
test: all
	./test/test.sh

.PHONY: all clean fclean re test

##########################
# Platform Compatibility #
##########################

#Linux | Darwin
OS := $(shell uname -s)

ifeq ($(OS),Linux)
	# commands for Linux
endif

ifeq ($(OS),Darwin)
    # command for macOS
    RLDIR = $(shell brew --prefix readline)
    INCLUDES += -I$(RLDIR)/include
    LIBS += -L$(RLDIR)/lib
endif
