# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kenakamu <kenakamu@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/11 15:25:14 by kenakamu          #+#    #+#              #
#    Updated: 2025/08/14 01:50:32 by kenakamu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	libftdprintf.a
CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror
SRCS		=	ft_dprintf.c		\
				ac_put.c		\
				ac_put_uniques.c


OBJS		=	$(SRCS:.c=.o)


all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)

%.o: %.c ft_dprintf.h
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
