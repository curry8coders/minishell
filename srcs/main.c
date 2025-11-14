/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/20 15:10:51 by hichikaw          #+#    #+#             */
/*   Updated: 2025/11/04 18:53:33 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <sys/wait.h>

#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "minishell.h"

char	*search_path(const char *filename)
{
	char path[PATH_MAX];
	char *value;
	char *end;

	value = getenv("PATH");
	while (*value)
	{
		// /bin:/usr/bin
		//     ^
		//     end
		bzero(path, PATH_MAX);
		end = strchr(value, ':');
		if (end)
		{
			strncpy(path, value, end - value);
			path[end - value] = '\0';
		}
		else
		{
			strncpy(path, value, PATH_MAX - 1);
			path[PATH_MAX - 1] = '\0';
		}
		strncat(path, "/", PATH_MAX - strlen(path) - 1);
		strncat(path, filename, PATH_MAX - strlen(path) - 1);
		if (access(path, X_OK) == 0)
		{
			char	*dup;
			
			dup = strdup(path);
			if (dup == NULL)
				fatal_error("strdup");
			return (dup);
		}
		if (end == NULL)
			return (NULL);
		value = end + 1;
	}
	return (NULL);
}

void	validate_access(const char *path, const char *filename)
{
	if (path == NULL)
		err_exit(filename, "command not found", 127);
	if (access(path, F_OK) < 0)
		err_exit(filename, "command not found", 127);
}
//指定された `path` に実行ファイルが本当に存在するかどうかを検証する
// accessはシステムコール
//      int access(const char *path, int mode);
// mode:
//未実装:追加で 空文字,'..',ファイル無効,ディレクトリかどうか,実行権限:permission(chmodでやるやつ)の確認をする
// man accessより
// X_OK for execute/search permission), or the existence test (F_OK)
// F_OK：そのパスが「存在するか？」だけを調べる chmodとかでは扱わないフラグ



int	exec(char *argv[])
{
	extern char **environ;
	const char *path = argv[0];
	pid_t	pid;
	int 	wstatus;

	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	else if (pid == 0)
	{
		// child process
		if (strchr(path, '/') == NULL)
			path = search_path(argv[0]);
		validate_access(path, argv[0]);
		execve(path, argv, environ);
		fatal_error("execve");
	}
	else
	{
		// parent process
		wait(&wstatus);
		return (WEXITSTATUS(wstatus));
	}
}
//fork
//pid_t fork(void);pidを取得
//pid==0子プロセスのコードを実行
//pid>0親プロセスのコード
//	自分の子プロセスのどれか一つが終了するのを待つ。
// 	今回の場合、先に`fork()` で作成した子プロセスが終了するのを待つことになる。
// wait()はシステムコール
// 親プロセス（シェル）がこの`wait()`を呼び出すと、その場で実行を一時停止する。
// WEXITSTATUS()は<sys/wait.h>に含まれるマクロ

void interpret(char *line, int *stat_loc)
{
	t_token	*tok;
	char 	**argv;
	t_node	*node;

	tok = tokenize(line);
	if (tok->kind == TK_EOF)
		;
	else if (syntax_error)
		*stat_loc = ERROR_TOKENIZE;
	else
	{
		node = parse(tok);
		expand(node);
		argv = token_list_to_argv(node->args);
		*stat_loc = exec(argv);
		free_argv(argv);
		free_node(node);
	}
	free_tok(tok);
}
//*stat_locではメモリアクセス
//status localtion
// なぜinterpretでexecが起動されるか

int	main(void)
{
	int		status;
	char	*line;

	rl_outstream = stderr;
	status = 0;
	
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		interpret(line, &status);
		free(line);
	}
	exit (status);
}
//interpret(line, &status)は&アドレス渡し

