#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>



static char *search_path_mode(const char *filename, int mode);

char	*search_path(const char *filename)
{
	char *path;
	
	path = search_path_mode(filename, X_OK);
	if (path)
		return (path);
	path = search_path_mode(filename, X_OK);
	return (path);
}

static char *search_path_mode(const char *filename, int mode)
{
	
}

int	exec_nonbuiltin(t_node *node)
{
	char	*path;
	
	//リダイレクトを判定+実行();
	// 構造体node->commandで複数のコマンドに対応
	// argv＝トークンからargvに変換(); *execveが引数に(char **)を求めるため
	// #include <unistd.h> 
	// int execve(const char *path:フルパス, char *const argv[]:それに続く引数たち, char *const envp[]:環境変数の配列);
	// buitlin関数：
	
	if (strchr(path, '/') == NULL) //つまりフルパスではない場合の例外処理 pathには最終的にはフルパスになっていて欲しいということ
		path = search_path(path);
	//validate_引数に無効な因子が存在しないか();
	// コマンドそれ自体が、コマンドの引数チェックを行うため、躊躇わずに渡してもよい ただしこの人は	
	// validate_access(path, argv[0]);というコマンドで
	// path=NULL,空文字,'..',ファイル無効,ディレクトリかどうか,実行権限:permission(chmodでやるやつ)の確認をこの時点でしている
	
	//execve(const char *file, char *const *argv, char *const *envp);
	
	//free(argv); //動的確保、関数内で使い切りタイプ
	
	// リダイレクトをリセット();
	//execveのエラー処理();
}

int	main(void)
{
	char *line;
	
	rl_outstream = stderr;
	
	while (1)
	{
		line = readline("minishell$ ");
		if (line == NULL)
			break ;
		if (*line)
			add_history(line);
		free(line);
	}
	exit (0);
}