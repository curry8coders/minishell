#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv, char **envp)
{
	if (argc < 2)
	{
		write(STDERR_FILENO, "Usage: ./execve <program> [args...] \n", 38);
		return (1);
	}
	if (execve(argv[1], &argv[1], envp) == -1)
	{
		perror("execve failed");
		return (1);
	}
	return (0);
}
