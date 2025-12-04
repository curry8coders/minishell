/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 16:58:40 by hichikaw          #+#    #+#             */
/*   Updated: 2025/12/04 22:39:31 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "libft.h"
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>

#define ERROR_TOKENIZE 258
#define ERROR_PARSE 258
#define ERROR_EXPAND 258 //`new` please check
#define ERROR_OPEN_REDIR 1
#define SINGLE_QUOTE_CHAR '\''
#define DOUBLE_QUOTE_CHAR '"'
#define BUILTIN_EXIT_REQUEST 256

typedef struct s_token t_token;
typedef enum e_token_kind t_token_kind;
typedef enum e_node_kind t_node_kind;
typedef struct s_node t_node;
typedef struct s_map t_map;
typedef struct s_item t_item;

extern int g_last_status;
extern int g_exit_status;
extern bool g_syntax_error;
extern bool g_readline_interrupted;
extern volatile sig_atomic_t g_sig;
extern t_map *g_envmap;

// error.c
void	perror_prefix(void);
void	fatal_error(const char *msg) __attribute__((noreturn));
void	assert_error(const char *msg) __attribute__((noreturn));
void	print_error(const char *location, const char *msg);
void	err_exit(const char *location, const char *msg, int status)
	__attribute__((noreturn));

// error_syntax.c
void	tokenize_error(const char *location, char **rest, char *line);
void	parse_error(t_token **rest, t_token *tok);
void	xperror(const char *location);
void	builtin_error(const char *func, const char *name, const char *err);

// tokenize.c
typedef struct s_token t_token;
enum e_token_kind {
  TK_WORD,
  TK_OP,
  TK_EOF,
};
typedef enum e_token_kind t_token_kind;


// `word` is zero terminated string.
struct s_token {
  char *word;
  t_token_kind kind;
  t_token *next;
};

enum e_node_kind {
  ND_PIPELINE,
  ND_SIMPLE_CMD,
  ND_REDIR_OUT,
  ND_REDIR_IN,
  ND_REDIR_APPEND,
  ND_REDIR_HEREDOC,
};
typedef enum e_node_kind t_node_kind;

typedef struct s_node t_node;
struct s_node {
  t_node_kind kind;
  t_node *next;
  // CMD
  t_token *args;
  t_node *redirects;
  // REDIR
  int targetfd;
  t_token *filename;
  t_token *delimiter;
  bool is_delim_unquoted;
  int filefd;
  int stashed_targetfd;
  // PIPELINE
  int inpipe[2];
  int outpipe[2];
  t_node *command;
};

// Redirectiong output example
// command			: "echo hello 1> out"
// targetfd			: 1
// filename			: "out"
// filefd			: open("out")
// stashed_targetfd	: dup(targetfd)

struct s_item {
  char *name;
  char *value;
  struct s_item *next;
};

struct s_map {
  t_item item_head;
};

#define STRBUF_INIT_CAP 16

typedef struct s_strbuf
{
	char	*data;
	size_t	len;
	size_t	capacity;
}	t_strbuf;

// expand_strbuf.c
void	strbuf_init(t_strbuf *buf);
void	strbuf_grow(t_strbuf *buf, size_t needed);
void	strbuf_append_char(t_strbuf *buf, char c);
void	strbuf_append_str(t_strbuf *buf, const char *s);
char	*strbuf_finish(t_strbuf *buf);

// expand_var.c
void	append_num(t_strbuf *buf, unsigned int num);
void	expand_special_parameter(t_strbuf *buf, char **rest, char *p);
void	expand_variable(t_strbuf *buf, char **rest, char *p);
void	expand_variable_tok(t_token *tok);
void	expand_variable_node(t_node *node);

// expand_quote.c
void	remove_single_quote(t_strbuf *buf, char **rest, char *p);
void	remove_double_quote(t_strbuf *buf, char **rest, char *p);
void	remove_quote(t_token *tok);
void	append_single_quote(t_strbuf *buf, char **rest, char *p);
void	append_double_quote(t_strbuf *buf, char **rest, char *p);

// expand.c
void	expand(t_node *node);
char	*expand_heredoc_line(char *line);
void	expand_quote_removal(t_node *node);

//expand_utils.c
bool	is_alpha_under(char c);
bool	is_alpha_num_under(char c);
bool	is_variable(char *s);
bool	is_special_parameter(char *s);

// tokenize.c
t_token *tokenize(char *line);
char **token_list_to_argv(t_token *tok);
t_token *new_token(char *word, t_token_kind kind);
t_token *operator(char **rest, char *line);

// tokenize_utils.c
bool	is_blank(char c);
bool	consume_blank(char **rest, char *line);
bool	startswith(const char *s, const char *keyword);
bool	is_metacharacter(char c);
bool	is_word(const char *s);
t_token *word(char **rest, char *line);

// destructor.c
void free_node(t_node *node);
void free_tok(t_token *tok);
void free_argv(char **argv);

// parse.c
t_node *parse(t_token *tok);
t_node *simple_command(t_token **rest, t_token *tok);
bool is_control_operator(t_token *tok);
void append_command_element(t_node *command, t_token **rest, t_token *tok);

// parse_redirect.c
t_node *redirect_out(t_token **rest, t_token *tok);
t_node *redirect_input(t_token **rest, t_token *tok);
t_node *redirect_append(t_token **rest, t_token *tok);
t_node *redirect_heredoc(t_token **rest, t_token *tok);

// parse_utils.c
bool at_eof(t_token *tok);
bool equal_op(t_token *tok, char *op);
t_node *new_node(t_node_kind kind);
t_token *tokdup(t_token *tok);

// parse_append.c
void append_tok(t_token **tokens, t_token *tok);
void append_node(t_node **node, t_node *elm);

// redirect.c
int stashfd(int fd);
bool is_redirect(t_node *node);
void do_redirect(t_node *redirects);
void reset_redirect(t_node *redirects);
void close_redirect_fds(t_node *redir);
void close_all_redirect_fds(t_node *node);
void close_pipeline_fds_except_current(t_node *head, t_node *current);

// redirect_heredoc.c
int read_heredoc(const char *delimiter, bool is_delim_unquoted);

// redirect_open.c
int open_redir_file(t_node *node);

// pipe.c
void prepare_pipe(t_node *node);
void prepare_pipe_child(t_node *node);
void prepare_pipe_parent(t_node *node);

// exec.c
int exec(t_node *node);

// exec_utils.c
char *search_path(const char *filename);

int get_exit_status(int wstatus);

// signal.c
int check_state(void);
void setup_signal(void);
void reset_signal(void);

// signal_handler.c
void handler(int signum);
void reset_sig(int signum);
void ignore_sig(int signum);
void setup_sigint(void);

// builtin.c
bool is_builtin(t_node *node);
int exec_builtin(t_node *node);

// builtin_exit.c
bool is_numeric(char *s);
int builtin_exit(char **argv);

// builtin_export.c
int builtin_export(char **argv);

// builtin_unset.c
int builtin_unset(char **argv);

// builtin_env.c
int builtin_env(char **argv);

// builtin_cd.c
int builtin_cd(char **argv);

// builtin_cd_utils.c
void delete_last_elm(char *path);
void append_path_elm(char *dst, char **rest, char *src);
void process_path_element(char *newpwd, char **path);
char *resolve_pwd(char *oldpwd, char *path);

// builtin_pwd.c
int builtin_pwd(char **argv);

// builtin_echo.c
bool check_only_n_options(char *arg);
int builtin_echo(char **argv);

// map.c
t_map *map_new(void);
char *map_get(t_map *map, const char *name);
int map_set(t_map *map, const char *name, const char *value);
int map_unset(t_map *map, const char *name);

// map_utils.c
bool is_identifier(const char *s);
t_item *item_new(char *name, char *value);
char *item_get_string(t_item *item);
int map_put(t_map *map, const char *string, bool allow_empty_value);
size_t map_len(t_map *map, bool count_null_value);
void map_printall(t_map *map);

// map_item.c
void update_existing_item(t_item *cur, const char *value);
t_item *create_new_item(const char *name, const char *value);
void parse_name_value(const char *string, char **name, char **value);

// env.c
char *xgetenv(const char *name);
void initenv(void);
char **get_environ(t_map *map);

#endif
