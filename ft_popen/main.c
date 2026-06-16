#include <stdio.h>
#include <unistd.h>

int ft_popen(const char *file, char *const argv[], char type);

int main()
{
    int  fd;
    char buffer[1024];
    int  bytes_read;

    // Executa o comando "ls -la" em modo de leitura
    fd = ft_popen("ls", (char *const []){"ls", "-la", NULL}, 'r');
    if (fd == -1)
    {
        printf("Erro ao executar ft_popen\n");
        return (1);
    }

    // Lê o que o "ls" jogou no pipe e imprime na tela
    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }

    close(fd);
    return (0);
}