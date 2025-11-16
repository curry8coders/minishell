#!/usr/bin/env bash
RED="\033[31m"
GREEN="\033[32m"
RESET="\033[0m"
OK=$GREEN"OK"$RESET
NG=$RED"NG"$RESET

# get script directry 
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"

# make tmp directry
TMP_DIR="$SCRIPT_DIR/tmp"
mkdir -p "$TMP_DIR"

# set timeout commnad
TIMEOUT=$(command -v gtimeout || command -v timeout)
TIMEOUT_SEC=2

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



assert() {
	COMMAND="$1"
	shift
	printf '%-50s:' "[$COMMAND]"
	# exit status
	echo -n -e "$COMMAND" | $TIMEOUT $TIMEOUT_SEC bash >"$TMP_DIR/cmp" 2>&1
	expected=$?
	for arg in "$@"
	do
		cp "$arg" "${arg}.cmp"
		# note:mv->cp is this actually correct?
	done
	echo -n -e "$COMMAND" | $TIMEOUT $TIMEOUT_SEC ./minishell >"$TMP_DIR/out" 2>&1
	actual=$?
	
	for arg in "$@"
	do
		cp "$arg" "${arg}.out"
	done
	
# ハング判定(timeout)
	if [ "$actual" -eq 124 ]; then
		echo -e "  ${RED}HANG${RESET} (timeout after ${TIMEOUT_SEC}s)"
		return
	fi
# 124とは？: タイムアウトで終了した場合（SIGTERM）

# diff判定
	diff "$TMP_DIR/cmp" "$TMP_DIR/out" >/dev/null && echo -n '  diff OK' || echo -n '  diff NG'

# status判定
	if [ "$actual" = "$expected" ]; then
		echo -n '  status OK'
	else
		echo -n "  status NG, expected $expected but got $actual"
	fi
# ファイル判定
	for arg in "$@"
	do
		echo -n "	[$arg] "
		diff "${arg}.cmp" "${arg}.out" >/dev/null && echo -e -n "$OK" || echo -e -n "$NG"
		rm -f "${arg}.cmp" "${arg}.out"
	done
	echo
}

cleanup() {
	rm -f a.out print_args hello.txt pwd.txt
	rm -f test_*
	rm -rf "$TMP_DIR"
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
assert 'cat <Makefile' 'Makefile'
echo hello >f1
echo world >f2
echo 42Tokyo >f3
assert 'cat <f1<f2<f3'
rm -f f1 f2 f3
assert 'cat <hoge'

## Apending Redirected output
assert 'pwd >>pwd.txt' 'pwd.txt'
assert 'pwd >>pwd.txt \n pwd >>pwd.txt' 'pwd.txt'

## Here Document
assert 'cat <<EOF\nhello\nworld\nEOF\nNOPRINT'
assert 'cat <<EOF<<eof\nhello\nworld\nEOF\nNOPRINT'
assert 'cat <<EOF\nhello\nworld'
assert 'cat <<E"O"F\nhello\nworld\nEOF\nNOPRINT'

cleanup
echo 'all OK'

