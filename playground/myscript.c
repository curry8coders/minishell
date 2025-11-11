// gcc myscript.c -o myscript
// ./myscript > results.log 2> error.log
// cat results.log
// hello
// cat error.log
// error

// ↑これが期待している挙動。

#include <unistd.h>

int main(void)
{
	write(1, "hello\n", 6);
	write(2, "error\n", 6);
}
