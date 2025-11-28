#!/usr/bin/env bash

# =================================================================================
# 
# =================================================================================

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

# =================================================================================
# Perse Command Line Arangement
# =================================================================================

# test_(number)
MAX_TEST_NUM=10
## Default
TESTS_TO_RUN=()
RUN_ALL=true

# options
SHOW_DIFF=false

for arg in "$@"; do
	if [ "$arg" = "--all" ]; then
		RUN_ALL=true
	elif [ "$1" = "--diff" ]; then
		SHOW_DIFF=true
	fi
	# future options can be added
done

for arg in "$@"; do
	if [[ "$arg" == --* ]]; then
		continue
	fi
	TESTS_TO_RUN+=("$arg")
	RUN_ALL=false
done

if [ "$RUN_ALL" == true ]; then
	TESTS_TO_RUN=($(seq 1 "$MAX_TEST_NUM"))
fi

# =================================================================================
# 
# =================================================================================

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


# =================================================================================
# 
# =================================================================================

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
	printf "%b" "$COMMAND" | bash >"$TMP_DIR/cmp" 2>"$TMP_DIR/cmp.err"
	expected=$?
	for arg in "$@"
	do
		mv "$arg" "${arg}.cmp"
	done
	
	# minishell
	printf "%b" "$COMMAND" | $TIMEOUT $TIMEOUT_SEC ./minishell >"$TMP_DIR/out" 2>"$TMP_DIR/out.err"
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

	# diff stdout
	if diff "$TMP_DIR/cmp" "$TMP_DIR/out" >/dev/null; then
		echo -n -e " diff $OK"
	else
		echo -n -e " diff $NG"
		if [ "$SHOW_DIFF" = true ]; then
			echo
			echo "    ${RED}--- stdout diff ---${RESET}"
			diff "$TMP_DIR/cmp" "$TMP_DIR/out" | sed 's/^/    /'
		fi
	fi

	# diff stderr
	if diff "$TMP_DIR/cmp.err" "$TMP_DIR/out.err" >/dev/null; then
		echo -n -e " err $OK"
	else
		echo -n -e " err $NG"
		if [ "$SHOW_DIFF" = true ]; then
			echo
			echo "    ${RED}--- stderr diff ---${RESET}"
			diff "$TMP_DIR/cmp.err" "$TMP_DIR/out.err" | sed 's/^/    /'
		fi
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
			echo
			echo "    ${RED}--- file($arg) diff ---${RESET}"
			diff "${arg}.cmp" "${arg}.out" | sed 's/^/    /'
		fi
		echo -n "]"
		rm -f "${arg}.cmp" "${arg}.out"
	done
	echo
}

# =================================================================================
# 
# =================================================================================

# ----------------------------------------1----------------------------------------
test_1(){
	# Empty line (EOF)
	printf "## Empty line (EOF)\n"
	assert ''
	echo
	# Absolute path commands without args
	printf "## Absolute path commands without args\n"
	assert '/bin/pwd'
	assert '/bin/echo'
	assert '/bin/ls'
	echo
	# Search command path without args
	printf "## Search command path without args\n"
	assert 'pwd'
	assert 'echo'
	assert 'ls'
	assert './a.out'
	echo
	# no such command
	printf "## no such command\n"
	assert 'a.out'
	assert 'nosuchfile'
	
	printf '%80' ''  | tr ' ' '1'
	echo
}
# ----------------------------------------1----------------------------------------

# ----------------------------------------2----------------------------------------
test_2(){
	# Tokenize
	printf "# Tokenize\n"
	## unquoted word
	printf "## unquoted word\n"
	assert 'ls /'
	assert 'echo hello	world	'
	assert 'nosuchfile\n\n'
	echo
	
	## single quote
	printf "## single quote\n"
	assert "./print_args 'hello	world' '42Tokyo'"
	assert "echo 'hello	world' '42Tokyo'"
	assert "echo '\"hello	world\"' '42Tokyo'"
	echo
	
	## double quote
	printf "## double quote\n"
	assert './print_args "hello		world" "42Tokyo"'
	assert 'echo "hello		world" "42Tokyo"'
	assert "echo \"'hello	world'\" \"42Tokyo\""
	echo
	
	## combination
	printf "## combination\n"
	assert "echo hello'		world'"
	assert "echo hello' world	'\" 42Tokyo \""
	echo
}
# ----------------------------------------2----------------------------------------

# ----------------------------------------3----------------------------------------
test_3(){
	# redirect
	ptrintf "# redirect\n"
	## Redirecting output
	printf "## Redirecting output\n"
	assert 'echo hello >hello.txt' 'hello.txt'
	assert 'echo hello >f1>f2>f3' 'f1' 'f2' 'f3'
	echo
	
	## Redirecting input
	printf "## Redirecting input\n"
	assert 'cat <Makefile'
	echo hello >f1
	echo world >f2
	echo 42Tokyo >f3
	assert 'cat <f1<f2<f3'
	assert 'cat <hoge'
	echo
	
	## Appending Redirected output
	printf "## Appending Redirected output\n"
	assert 'pwd >>pwd.txt' 'pwd.txt'
	assert 'pwd >>pwd.txt \n pwd >>pwd.txt' 'pwd.txt'
	echo
	
	## Here Document
	printf "## Here Document\n"
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
	echo
}
# ----------------------------------------3----------------------------------------

# ----------------------------------------4----------------------------------------
test_4(){
	# Pipe
	printf "# Pipe\n"
	assert 'cat Makefile | grep minishell'
	assert 'cat | cat | ls\n\n'
	echo
	
	# Expand Variable
	printf "# Expand Variable\n"
	assert 'echo $USER'
	assert 'echo $USER$PATH&TERM'
	assert 'echo "$USER $PATH $TERM"'
	echo
	
	# Special Parameter $?
	printf "# Special Parameter $?\n"
	assert 'echo $?'
	assert 'invalid\necho $?\necho $?'
	assert 'exit42\necho $?\necho $?'
	assert 'exit42\n\necho $?\necho $?'
	echo
}
	
# ----------------------------------------4----------------------------------------

# ----------------------------------------5----------------------------------------
test_5(){
	# Signal
	printf "# Signal\n"
	echo "int main() {while (1);}" | cc -xc -o infinite_loop -
	 # `-xc`: 入力がファイルではないため、ソースの言語をC言語と明示
	 # `-o infinite_loop`: 出力される実行可能ファイル名を `infinite_loop` に指定
	 # `-`: ソースコードをファイルからではなく、標準入力から読み込み
	
	## Signal to shell processes
	print_desc "SIGTERM to SHELL"
	(sleep 0.1; pkill -SIGTERM bash;
	 sleep 0.1; pkill -SIGTERM minishell) &
	assert './infinite_loop' 2>/dev/null
	 ###目標:SIGTERMが成功する
	
	print_desc "SIGQUIT to SHELL"
	(sleep 0.1; pkill -SIGQUIT bash; # SIGQUITシグナルを正しく【無視】する
	 sleep 0.1; pkill -SIGTERM bash;
	 sleep 0.1; pkill -SIGQUIT minishell; # SIGQUITシグナルを正しく【無視】する
	 sleep 0.1; pkill -SIGTERM minishell) &
	assert './infinite_loop' 2>/dev/null
	 ### SIGQUIT がshellをkillしない
	
	print_desc "SIGINT to SHELL"
	(sleep 0.1; pkill -SIGINT bash; # SIGINTシグナルを正しく【無視】する
	 sleep 0.1; pkill -SIGTERM bash;
	 sleep 0.1; pkill -SIGINT minishell; # SIGINTシグナルを正しく【無視】する
	 sleep 0.1; pkill -SIGTERM minishell) &
	assert './infinite_loop' 2>/dev/null
	 ### SIGINT がshellをkillしない
	
	
	## Signal to child processes すべて正常に終了・中断すること
	print_desc "SITERM to child process"
	(sleep 0.1; pkill -SIGTERM infinite_loop;
	 sleep 0.1; pkill -SIGTERM infinite_loop) &
	assert './infinite_loop'
	 # 1回目にbash 2回目にminishell の子プロセス がターゲットに
	
	print_desc "SIGQUIT to child process"
	(sleep 0.1; pkill -SIGQUIT infinite_loop;
	 sleep 0.1; pkill -SIGQUIT infinite_loop) &
	assert './infinite_loop'
	
	print_desc "SIGINT to child process"
	(sleep 0.1; pkill -SIGINT infinite_loop;
	 sleep 0.1; pkill -SIGINT infinite_loop) &
	assert './infinite_loop'
	
	print_desc "SIGUSR1 to child process"
	(sleep 0.1; pkill -SIGUSR1 infinite_loop;
	 sleep 0.1; pkill -SIGUSR1 infinite_loop) &
	assert './infinite_loop'
	 #infinite_loopにはSIGUSR1受け取りの動作がないので、
	 #SIGUSR1のデフォルト動作である「プロセスの終了」が実行されることを確認
	echo
}
# ----------------------------------------5----------------------------------------
 
# Manual Debug
# $ ./minishell
# $
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D
#
# $ ./minishell
# $ hogehoge
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D
#
# $ ./minishell
# $ cat <<EOF
# >
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D
#
# $ ./minishell
# $ cat <<EOF
# > hoge
# > fuga
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D

# ----------------------------------------6---------------------------------------- 
test_6(){
	# Builtin
	printf "# Builtin\n"
	## exit
	printf "## exit"
	assert 'exit'
	assert 'exit 42'
	assert 'exit ""'
	assert 'exit hello'
	assert 'exit 42Tokyo'
	assert 'exit 1 2'
	echo
	
	## export
	printf "## export\n"
	print_desc "Output of 'export' differs, but it's ok."
	assert 'export'
	assert 'export | grep nosuch | sort'
	assert 'export nosuch\n export | grep nosuch | sort'
	assert 'export nosuch=fuga\n export | grep nosuch | sort'
	assert 'export nosuch=fuga hoge=nosuch\n export | grep nosuch | sort'
	assert 'export [invalid]'
	assert 'export [invalid_nosuch]\n export | grep nosuch | sort'
	assert 'export [invalid]=nosuch\n export | grep nosuch | sort'
	assert 'export [invalid] nosuch hoge=nosuch\n export | grep nosuch | sort'
	assert 'export nosuch [invalid] hoge=nosuch\n export | grep nosuch | sort'
	assert 'export nosuch hoge=nosuch [invalid]\n export | grep nosuch | sort'
	assert 'export nosuch="nosuch2=hoge"\nexport $nosuch\n export | grep nosuch | sort'
	echo
}
# ----------------------------------------6---------------------------------------- 

# ----------------------------------------7---------------------------------------- 
test_7(){
	## unset
	printf "# unset\n"
	export hoge fuga=fuga
	assert 'unset'
	assert 'unset hoge'
	assert 'unset fuga'
	assert 'unset nosuch'
	assert 'unset [invalid]'
	assert 'unset hoge fuga'
	assert 'unset hoge nosuch fuga'
	assert 'unset fuga \n export | echo $fuga'
	assert 'unset [invalid] fuga \n echo $fuga'
	echo
}
# ----------------------------------------7---------------------------------------- 

# ----------------------------------------8---------------------------------------- 
test_8(){
	## env
	printf "## env\n"
	print_desc "Output of 'env' differs, but it's ok."
	assert 'env'
	assert 'env | grep hoge | sort'
	echo
}

# ----------------------------------------8---------------------------------------- 

# ----------------------------------------9---------------------------------------- 
test_9(){
	## cd
	printf "## cd\n"
	assert 'cd'
	assert 'cd .'
	assert 'cd ..'
	assert 'cd ///'
	assert 'cd /tmp'
	assert 'cd /tmp/'
	assert 'cd /tmp///'
	assert 'cd /../../../././.././'
	assert 'cd src'
	assert 'unset HOME\ncd'
	echo
	
	assert 'cd \n echo $PWD'
	assert 'cd .\n echo $PWD'
	assert 'cd ..\n echo $PWD'
	assert 'cd ///\n echo $PWD'
	assert 'cd /tmp\n echo $PWD'
	assert 'cd /tmp/\n echo $PWD'
	assert 'cd /tmp///\n echo $PWD'
	assert 'cd /../../../././.././\n echo $PWD'
	assert 'cd src\n echo $PWD'
	assert 'unset HOME\ncd \n echo $PWD'
	echo
}
# ----------------------------------------9---------------------------------------- 

# ----------------------------------------10---------------------------------------- 
test_10(){
	## echo
	printf "## echo\n"
	assert 'echo'
	assert 'echo hello'
	assert 'echo hello "	" world'
	assert 'echo -n'
	assert 'echo -n hello'
	assert 'echo -n hello world'
	assert 'echo hello -n'
	echo
	
	## pwd
	printf "## pwd\n"
	assert 'pwd'
	assert 'cd\npwd'
	assert 'cd src\npwd'
	assert 'cd /etc\npwd'
	assert 'cd . \n pwd \n echo $PWD $OLDPWD'
	assert 'cd .. \n pwd \n echo $PWD $OLDPWD'
	assert 'cd /// \n pwd \n echo $PWD $OLDPWD'
	assert 'cd /tmp/// \n pwd \n echo $PWD $OLDPWD'
	assert 'unset PWD\npwd\ncd /etc\npwd'
	echo
}
# ----------------------------------------10---------------------------------------- 
 
# =================================================================================
# 
# =================================================================================
 
for test_num in "${TESTS_TO_RUN[@]}"; do
	if type "test_$test_num" >/dev/null 2>&1; then
		test_$test_num
	else
		echo -e "${RED}ERROR: $test_num doesn't exist"
	fi
done


# cleanup 
cd "$PROJECT_ROOT" || exit 1
rm -rf "$TMP_DIR"

echo "Fin."
