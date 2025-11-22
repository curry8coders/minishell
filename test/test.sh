#!/usr/bin/env bash

# colors
RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET="\033[0m"

OK=$GREEN"OK"$RESET
NG=$RED"NG"$RESET
print_desc(){
	echo -e $YELLOW"$1"$RESET
}

# get specific directory
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# make tmp directoryNG
TMP_DIR="$SCRIPT_DIR/tmp"
rm -rf "$TMP_DIR"
mkdir -p "$TMP_DIR"

# move to tmp directory
cd "$TMP_DIR" || exit 1

# make symbolic link
ln -sf "$PROJECT_ROOT/Makefile" "$TMP_DIR/Makefile"
ln -sf "$PROJECT_ROOT/minishell" "$TMP_DIR/minishell"

# set timeout commnad
TIMEOUT=$(command -v gtimeout || command -v timeout)
TIMEOUT_SEC=1

cat <<EOF | gcc -xc -o a.out -
#include <stdio.h>
int main() { printf("hello from a.out\n"); }
EOF

cat <<EOF | gcc -xc -o print_args -
#include <stdio.h>
int main(int argc, char **argv) {
	for (int i = 0; argv[i]; i++)
		printf("argv[%d] = %s\n", i, argv[i]);
}
EOF

cat <<EOF | gcc -xc -o exit42 -
int main() { return 42; }
EOF



assert() {
	COMMAND="$1"
	shift
	# convert tab and /t and /n
	DISPLAY_CMD=$(printf '%s' "$COMMAND" | tr '\t\n' '  ')
	if [ ${#DISPLAY_CMD} -gt 48 ]; then
		DISPLAY_CMD="${DISPLAY_CMD}..."
	fi
	printf '%-50s :' "[$DISPLAY_CMD]"
	
	# bash
	echo -n -e "$COMMAND" | bash >"$TMP_DIR/cmp" 2>&-
	expected=$?
	for arg in "$@"
	do
		mv "$arg" "${arg}.cmp"
	done
	
	# minishell
	echo -n -e "$COMMAND" | $TIMEOUT $TIMEOUT_SEC ./minishell >"$TMP_DIR/out" 2>&-
	actual=$?
	for arg in "$@"
	do
		mv "$arg" "${arg}.out"
	done
	
	# ハング判定(timeout)
	if [ "$actual" -eq 124 ]; then
		echo -e " ${RED}<<HANG>>${RESET} (timeout after ${TIMEOUT_SEC}s)"
		return
	fi
	## 124とは？: タイムアウトで終了した場合（SIGTERM）

	# diff
	if diff "$TMP_DIR/cmp" "$TMP_DIR/out" >/dev/null; then
		echo -n -e " diff $OK"
	else
		echo -n -e " diff $NG"
	fi
	
	# status判定
	if [ "$actual" = "$expected" ]; then
		echo -n -e " status $OK"
	else
		echo -n -e " status $NG, ($expected->$actual)"
	fi
	# ファイル判定
	for arg in "$@"
	do
		echo -n " [$arg:"
		if diff "${arg}.cmp" "${arg}.out" >/dev/null; then
			echo -n -e "$OK"
		else
			echo -n -e "$NG"
		fi
		echo -n "]"
		rm -f "${arg}.cmp" "${arg}.out"
	done
	echo
}

# Empty line (EOF)
assert ''

# Absolute path commands without args
assert '/bin/pwd'
assert '/bin/echo'
assert '/bin/ls'

# Search command path without args
assert 'pwd'
assert 'echo'
assert 'ls'
assert './a.out'

## no such command
assert 'a.out'
assert 'nosuchfile'

# Tokenize
## unquoted word
assert 'ls /'
assert 'echo hello	world	'
assert 'nosuchfile\n\n'

## single quote
assert "./print_args 'hello	world' '42Tokyo'"
assert "echo 'hello	world' '42Tokyo'"
assert "echo '\"hello	world\"' '42Tokyo'"

## double quote
assert './print_args "hello		world" "42Tokyo"'
assert 'echo "hello		world" "42Tokyo"'
assert "echo \"'hello	world'\" \"42Tokyo\""

## combination
assert "echo hello'		world'"
assert "echo hello' world	'\" 42Tokyo \""

# Redirect
## Redirecting output
assert 'echo hello >hello.txt' 'hello.txt'
assert 'echo hello >f1>f2>f3' 'f1' 'f2' 'f3'

## Redirecting input
assert 'cat <Makefile'
echo hello >f1
echo world >f2
echo 42Tokyo >f3
assert 'cat <f1<f2<f3'
assert 'cat <hoge'

## Apending Redirected output
assert 'pwd >>pwd.txt' 'pwd.txt'
assert 'pwd >>pwd.txt \n pwd >>pwd.txt' 'pwd.txt'

## Here Document
assert 'cat <<EOF\nhello\nworld\nEOF\nNOPRINT'
assert 'cat <<EOF<<eof\nhello\nworld\nEOF\nNOPRINT'
assert 'cat <<EOF\nhello\nworld'
assert 'cat <<E"O"F\nhello\nworld\nEOF\nNOPRINT'
assert 'cat <<EOF	\n$USER\n$NO_SUCH_VAR\n$FOO$BAR\nEOF'
assert 'cat <<"EOF"	\n$USER\n$NO_SUCH_VAR\n$FOO$BAR\nEOF'
assert 'cat <<EO"F"	\n$USER\n$NO_SUCH_VAR\n$FOO$BAR\nEOF'
export	EOF='eof'
assert	'cat <<$EOF			\neof\nEOF\nEOF'
assert	'cat <<"$EOF		\neof\nEOF\nEOF'

# Pipe
assert 'cat Makefile | grep minishell'
assert 'cat | cat | ls\n\n'

# Expand Variable
assert 'echo $USER'
assert 'echo $USER$PATH&TERM'
assert 'echo "$USER $PATH $TERM"'

# Special Parameter $?
assert 'echo $?'
assert 'invalid\necho $?\necho $?'
assert 'exit42\necho $?\necho $?'
assert 'exit42\n\necho $?\necho $?'

echo

# Signal handling
echo "int main() {while (1);}" | cc -xc -o infinite_loop -
 # `-xc`: 入力がファイルではないため、ソースの言語をC言語と明示
 # `-o infinite_loop`: 出力される実行可能ファイル名を `infinite_loop` に指定
 # `-`: ソースコードをファイルからではなく、標準入力から読み込み

## Signal to shell processes
print_desc "SIGTERM to SHELL"
(sleep 0.01; pkill -SIGTERM bash;
 sleep 0.01; pkill -SIGTERM minishell) &
assert './infinite_loop' 2>/dev/null
 # 目標:SIGTERMが成功する

print_desc "SIGQUIT to SHELL"
(sleep 0.01; pkill -SIGQUIT bash; # SIGQUITシグナルを正しく【無視】する
 sleep 0.01; pkill -SIGTERM bash;
 sleep 0.01; pkill -SIGQUIT minishell; # SIGQUITシグナルを正しく【無視】する
 sleep 0.01; pkill -SIGTERM minishell) &
assert './infinite_loop' 2>/dev/null
 # SIGQUIT がshellをkillしない

print_desc "SIGINT to SHELL"
(sleep 0.01; pkill -SIGINT bash; # SIGINTシグナルを正しく【無視】する
 sleep 0.01; pkill -SIGTERM bash;
 sleep 0.01; pkill -SIGINT minishell; # SIGINTシグナルを正しく【無視】する
 sleep 0.01; pkill -SIGTERM minishell) &
assert './infinite_loop' 2>/dev/null
 # SIGINT がshellをkillしない


## Signal to child processes すべて正常に終了・中断すること
print_desc "SITERM to child process"
(sleep 0.01; pkill -SIGTERM infinite_loop;
 sleep 0.01; pkill -SIGTERM infinite_loop) &
assert './infinite_loop'
 # 1回目にbash 2回目にminishell の子プロセス がターゲットに

print_desc "SIGQUIT to child process"
(sleep 0.01; pkill -SIGQUIT infinite_loop;
 sleep 0.01; pkill -SIGQUIT infinite_loop) &
assert './infinite_loop'

print_desc "SIGINT to child process"
(sleep 0.01; pkill -SIGINT infinite_loop;
 sleep 0.01; pkill -SIGINT infinite_loop) &
assert './infinite_loop'

print_desc "SIGUSR1 to child process"
(sleep 0.01; pkill -SIGUSR1 infinite_loop;
 sleep 0.01; pkill -SIGUSR1 infinite_loop) &
assert './infinite_loop'
 #infinite_loopにはSIGUSR1受け取りの動作がないので、
 #SIGUSR1のデフォルト動作である「プロセスの終了」が実行されることを確認


# cleanup 
cd "$PROJECT_ROOT" || exit 1
rm -rf "$TMP_DIR"

echo
echo "Fin."
