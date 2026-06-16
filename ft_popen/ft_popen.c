#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

//fd = ft_popen("ls", (char *const []){"ls", NULL}, 'r');

int ft_popen(const char *file, char *const argv[], char type)
{
	int		pipefd[2];
	pid_t	pid;

    if (type != 'r' && type != 'w')
        return (-1);
    if (pipe(pipefd) == -1)
        return (-1);
    pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (-1);
    }
    if (pid == 0)
    {
        if (type == 'r')
        {
            close(pipefd[0]);
            dup2(pipefd[1], 1);
            close(pipefd[1]);
        }
        else if (type == 'w')
        {
            close(pipefd[1]);
            dup2(pipefd[0], 0);
            close(pipefd[0]);
        }
        execvp(file, argv);
        exit(1);
    }
    else
    {
        if (type == 'r')
        {
            close(pipefd[1]);
            return (pipefd[0]);
        }
        else if (type == 'w')
        {
            close(pipefd[0]);
            return (pipefd[1]);
        }
    }
}
