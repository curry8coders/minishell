# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/14 16:40:32 by ichikawahik       #+#    #+#              #
#    Updated: 2025/10/15 12:19:22 by ichikawahik      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Linux | Darwin
OS := $(shell uname -s)

ifeq ($(OS),Darwin)
	# command for macOS
	RLDIR = $(shell brew --prefix readline)
	INCLUDES = -I$(RLDIR)/include
	LDFLAGS = -L$(RLDIR)/lib
endif

NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LIBS = -lreadline
SRCS = srcs/main.c
OBJS = $(SRCS:%.c=%.o)

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