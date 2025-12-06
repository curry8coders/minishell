# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/14 16:40:32 by ichikawahik       #+#    #+#              #
#    Updated: 2025/12/06 15:28:30 by ichikawahik      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Linux | Darwin
OS := $(shell uname -s)

#############
# Variables #
#############

NAME = minishell
CC = cc
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

INCLUDES = -I includes -I$(LIBFT_DIR)
CFLAGS = -Wall -Wextra -Werror $(INCLUDES)
LDFLAGS=

########################
# Platform Compatibility 
########################
ifeq ($(OS),Darwin)
    # commands for macOS
    RLDIR = $(shell brew --prefix readline)
    INCLUDES += -I$(RLDIR)/include
    LDFLAGS += -L$(RLDIR)/lib $(LIBS)
endif

LIBS = -lreadline $(LIBFT)

SRCS = srcs/main.c\
       srcs/error_handler/error.c\
       srcs/error_handler/error_syntax.c\
       srcs/error_handler/error_utils.c\
       srcs/tokenizer/tokenize.c\
       srcs/tokenizer/tokenize_utils.c\
       srcs/tokenizer/tokenize_utils2.c\
       srcs/destructor.c\
       srcs/tokenizer/expand.c\
       srcs/tokenizer/expand_strbuf.c\
       srcs/tokenizer/expand_var.c\
       srcs/tokenizer/expand_quote.c\
       srcs/tokenizer/expand_utils.c\
       srcs/parser/parse.c\
       srcs/parser/parse_redirect.c\
       srcs/parser/parse_utils.c\
       srcs/parser/parse_append.c\
       srcs/redirection/redirect.c\
       srcs/redirection/redirect_close.c\
       srcs/redirection/redirect_heredoc.c\
       srcs/redirection/redirect_open.c\
	   srcs/pipe/pipe.c\
	   srcs/exec/exec.c\
	   srcs/exec/exec_utils.c\
	   srcs/exec/exec_resolve.c\
	   srcs/exec/exec_wait.c\
	   srcs/signal/signal.c\
	   srcs/signal/signal_handler.c\
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
	   srcs/hashstamp/map_utils.c\
	   srcs/hashstamp/map_item.c\
	   srcs/hashstamp/env.c\


OBJS = $(SRCS:%.c=%.o)

#################
# General rules #
#################

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) -o $(NAME) $(OBJS) $(LDFLAGS) $(LIBS) 
	
%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	
$(LIBFT):
	@+$(MAKE) -C $(LIBFT_DIR) all bonus

clean:
	@$(RM) $(OBJS)
	@$(RM) $(SRCS:%.c=%.o.tmp) 
	@+$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	+$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

# We may delete the following code when we push this project to 42's remote repository.
test: all
	bash ./test/test.sh

.PHONY: all clean fclean re test
