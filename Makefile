# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/14 16:40:32 by ichikawahik       #+#    #+#              #
#    Updated: 2025/10/25 17:18:45 by hichikaw         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Linux | Darwin
OS := $(shell uname -s)

ifeq ($(OS),Darwin)
	# command for macOS
	RLDIR = $(shell brew --prefix readline)
	INCLUDES = -I$(RLDIR)/includes
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