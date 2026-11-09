#include <unistd.h>
#include <stdlib.h>

int ft_popen(const char *file, char *const argv[], char type)
{
	int fd[2];
	int pid;

	if (!file || !argv || (type != 'w' && type != 'r'))
		return (-1);
	if (pipe(fd) < 0)
		return (-1);
	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		return -1;
	}
	if (!pid)
	{
		if (type == 'r')
		{
			if (dup2(fd[1], 1) < 0)
				exit (-1);
		}
		if (type == 'w')
			if (dup2(fd[0], 0) < 0)
				exit (-1);
		close(fd[0]);
		close(fd[1]);
		execvp(file, argv);
		exit(-1);	
	}

}