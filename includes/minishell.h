/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:58:40 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/05 22:03:46 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <errno.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>

# define ERROR_TOKENIZE 258
# define ERROR_PARSE 258
# define ERROR_EXPAND 258
# define ERROR_OPEN_REDIR 1
# define SINGLE_QUOTE_CHAR '\''
# define DOUBLE_QUOTE_CHAR '"'
# define BUILTIN_EXIT_REQUEST 256
# define STRBUF_INIT_CAP 16

typedef struct s_token			t_token;
typedef struct s_node			t_node;
typedef struct s_map			t_map;
typedef struct s_item			t_item;
typedef struct s_strbuf			t_strbuf;
typedef struct s_shell			t_shell;
typedef volatile sig_atomic_t	t_sa;

extern t_sa						g_sig;

typedef enum e_token_kind
{
	TK_WORD,
	TK_OP,
	TK_EOF,
}	t_token_kind;

typedef enum e_node_kind
{
	ND_PIPELINE,
	ND_SIMPLE_CMD,
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_APPEND,
	ND_REDIR_HEREDOC,
}	t_node_kind;

struct s_token
{
	char			*word;
	t_token_kind	kind;
	t_token			*next;
};

struct s_node
{
	t_node_kind		kind;
	t_node			*next;
	t_token			*args;
	t_node			*redirects;
	int				targetfd;
	t_token			*filename;
	t_token			*delimiter;
	bool			is_delim_unquoted;
	int				filefd;
	int				stashed_targetfd;
	int				inpipe[2];
	int				outpipe[2];
	t_node			*command;
};

struct s_item
{
	char			*name;
	char			*value;
	struct s_item	*next;
};

struct s_map
{
	t_item			item_head;
};

struct s_strbuf
{
	char			*data;
	size_t			len;
	size_t			capacity;
};

struct s_shell
{
	int				last_status;
	int				exit_status;
	bool			syntax_error;
	bool			readline_interrupted;
	t_map			*envmap;
};
 

//expand_utils.c


// tokenize.c


// tokenize_utils.c


// expand.c

// destructor.c


// parse.c

// parse_redirect.c


// parse_utils.c


// parse_append.c

// redirect.c

// redirect_heredoc.c


// redirect_open.c

// pipe.c


// exec.c


// exec_utils.c


// signal.c


// signal_handler.c


// builtin.c


// builtin_exit.c

// builtin_export.c

// builtin_unset.c

// builtin_env.c

// builtin_cd.c

// builtin_cd_utils.c


// builtin_pwd.c


// builtin_echo.c


// map.c


// map_utils.c


// map_item.c


void				perror_prefix(void);
void				fatal_error(const char *msg) __attribute__((noreturn));
void				assert_error(const char *msg) __attribute__((noreturn));
void				print_error(const char *location, const char *msg);
void				err_exit(const char *loc, const char *msg, int status)
					__attribute__((noreturn));
void				tokenize_error(t_shell *sh, const char *loc,
						char **rest, char *line);
void				parse_error(t_shell *shell, t_token **rest, t_token *tok);
void				xperror(const char *location);
void				builtin_error(const char *f, const char *n, const char *e);
void				strbuf_init(t_strbuf *buf);
void				strbuf_grow(t_strbuf *buf, size_t needed);
void				strbuf_append_char(t_strbuf *buf, char c);
void				strbuf_append_str(t_strbuf *buf, const char *s);
char				*strbuf_finish(t_strbuf *buf);
void				append_num(t_strbuf *buf, unsigned int num);
void				expand_special_param(t_shell *sh, t_strbuf *buf,
						char **rest, char *p);
void				expand_variable(t_shell *sh, t_strbuf *buf,
						char **rest, char *p);
void				expand_variable_tok(t_shell *shell, t_token *tok);
void				expand_variable_node(t_shell *shell, t_node *node);
void				remove_single_quote(t_strbuf *buf, char **rest, char *p);
void				remove_double_quote(t_strbuf *buf, char **rest, char *p);
void				remove_quote(t_token *tok);
void				append_single_quote(t_strbuf *buf, char **rest, char *p);
void				append_double_quote(t_shell *sh, t_strbuf *buf,
						char **rest, char *p);
void				expand(t_shell *shell, t_node *node);
char				*expand_heredoc_line(t_shell *shell, char *line);
void				expand_quote_removal(t_node *node);
bool				is_alpha_under(char c);
bool				is_alpha_num_under(char c);
bool				is_variable(char *s);
bool				is_special_parameter(char *s);
t_token				*tokenize(t_shell *shell, char *line);
char				**token_list_to_argv(t_token *tok);
t_token				*new_token(char *word, t_token_kind kind);
t_token				*operator(char **rest, char *line);
bool				is_blank(char c);
bool				consume_blank(char **rest, char *line);
bool				startswith(const char *s, const char *keyword);
bool				is_metacharacter(char c);
bool				is_word(const char *s);
t_token				*word(t_shell *shell, char **rest, char *line);
void				free_node(t_node *node);
void				free_tok(t_token *tok);
void				free_argv(char **argv);
t_node				*parse(t_shell *shell, t_token *tok);
t_node				*simple_command(t_shell *sh, t_token **rest, t_token *tok);
bool				is_control_operator(t_token *tok);
void				append_cmd_elm(t_shell *sh, t_node *cmd,
						t_token **rest, t_token *tok);
t_node				*redirect_out(t_token **rest, t_token *tok);
t_node				*redirect_input(t_token **rest, t_token *tok);
t_node				*redirect_append(t_token **rest, t_token *tok);
t_node				*redirect_heredoc(t_token **rest, t_token *tok);
bool				at_eof(t_token *tok);
bool				equal_op(t_token *tok, char *op);
t_node				*new_node(t_node_kind kind);
t_token				*tokdup(t_token *tok);
void				append_tok(t_token **tokens, t_token *tok);
void				append_node(t_node **node, t_node *elm);
int					stashfd(int fd);
bool				is_redirect(t_node *node);
void				do_redirect(t_node *redirects);
void				reset_redirect(t_node *redirects);
void				close_redirect_fds(t_node *redir);
void				close_all_redirect_fds(t_node *node);
void				close_pipeline_fds_except_current(t_node *h, t_node *cur);
int					read_heredoc(t_shell *sh, const char *delim,
						bool is_delim_unquoted);
int					open_redir_file(t_shell *shell, t_node *node);
int					prepare_pipe(t_node *node);
void				prepare_pipe_child(t_node *node);
void				prepare_pipe_parent(t_node *node);
int					exec(t_shell *shell, t_node *node);
char				*search_path(t_shell *shell, const char *filename);
char				*resolve_path(t_shell *shell, char **argv);
int					get_exit_status(int wstatus);
int					check_state(void);
void				setup_signal(t_shell *shell);
void				reset_signal(void);
void				handler(int signum);
void				reset_sig(int signum);
void				ignore_sig(int signum);
void				setup_sigint(void);
bool				is_builtin(t_node *node);
int					exec_builtin(t_shell *shell, t_node *node);
bool				is_numeric(char *s);
int					builtin_exit(t_shell *shell, char **argv);
int					builtin_export(t_shell *shell, char **argv);
int					builtin_unset(t_shell *shell, char **argv);
int					builtin_env(t_shell *shell, char **argv);
int					builtin_cd(t_shell *shell, char **argv);
void				delete_last_elm(char *path);
void				append_path_elm(char *dst, char **rest, char *src);
void				process_path_element(char *newpwd, char **path);
char				*resolve_pwd(char *oldpwd, char *path);
int					builtin_pwd(t_shell *shell, char **argv);
bool				check_only_n_options(char *arg);
int					builtin_echo(char **argv);
t_map				*map_new(void);
char				*map_get(t_map *map, const char *name);
int					map_set(t_map *map, const char *name, const char *value);
int					map_unset(t_map *map, const char *name);
bool				is_identifier(const char *s);
t_item				*item_new(char *name, char *value);
char				*item_get_string(t_item *item);
int					map_put(t_map *map, const char *str, bool allow_empty);
size_t				map_len(t_map *map, bool count_null_value);
void				map_printall(t_map *map);
void				update_existing_item(t_item *cur, const char *value);
t_item				*create_new_item(const char *name, const char *value);
void				parse_name_value(const char *str, char **name, char **val);
char				*xgetenv(t_shell *shell, const char *name);
void				initenv(t_shell *shell);
char				**get_environ(t_map *map);

#endif
