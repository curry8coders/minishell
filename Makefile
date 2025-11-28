# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/14 16:40:32 by ichikawahik       #+#    #+#              #
#    Updated: 2025/11/29 03:21:58 by hichikaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#############
# Variables #
#############

NAME = minishell
CC = cc
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a
INCLUDES = -I includes -I$(LIBFT_DIR)
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LIBS = -lreadline $(LIBFT)
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
	   srcs/builtin/builtin_cd_utils.c\
	   srcs/builtin/builtin_pwd.c\
	   srcs/builtin/builtin_echo.c\
	   srcs/hashstamp/map.c\
	   srcs/hashstamp/env.c\

OBJS = $(SRCS:%.c=%.o)

#################
# General rules #
#################

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

$(LIBFT):
	make -C $(LIBFT_DIR) all bonus

clean:
	$(RM) $(OBJS)
	make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	make -C $(LIBFT_DIR) fclean

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
    LIBS := -L$(RLDIR)/lib $(LIBS)
endif
