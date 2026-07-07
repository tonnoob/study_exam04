#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int picoshell(char **cmds[])
{
    int     i;
    int     prev_fd;
    int     fd[2];
    pid_t   pid;

    if (!cmds || !cmds[0])
        return (0);

    i = 0;
    prev_fd = -1; // Começa em -1 pois o 1º comando não tem pipe anterior

    while (cmds[i])
    {
        // Se não for o último comando, precisamos criar um novo pipe
        if (cmds[i + 1] != NULL)
        {
            if (pipe(fd) == -1)
            {
                if (prev_fd != -1)
                    close(prev_fd); // Proteção de erro: fecha o que ficou aberto
                return (1);
            }
        }

        pid = fork();
        
        if (pid == -1) // Falha no fork
        {
            if (prev_fd != -1)
                close(prev_fd);
            if (cmds[i + 1] != NULL)
            {
                close(fd[0]);
                close(fd[1]);
            }
            return (1);
        }

        if (pid == 0) // --- PROCESSO FILHO ---
        {
            // Se existe um pipe anterior, redireciona o STDIN (0) para ler dele
            if (prev_fd != -1)
            {
                dup2(prev_fd, 0);
                close(prev_fd);
            }
            
            // Se NÃO for o último comando, redireciona o STDOUT (1) para escrever no pipe atual
            if (cmds[i + 1] != NULL)
            {
                dup2(fd[1], 1);
                close(fd[1]);
                close(fd[0]); // O filho não lê do próprio pipe
            }
            
            // Executa o comando
            execvp(cmds[i][0], cmds[i]);
            
            // Se o execvp falhar (ex: comando não existe), o filho deve morrer com erro
            exit(1); 
        }
        else // --- PROCESSO PAI ---
        {
            // O pai já pode fechar o read do pipe anterior, pois o filho já deu dup2 nele
            if (prev_fd != -1)
                close(prev_fd);
            
            // Se criamos um pipe nesta rodada, o pai salva o read para a próxima rodada
            if (cmds[i + 1] != NULL)
            {
                close(fd[1]); // O pai não escreve nada
                prev_fd = fd[0]; // Guarda a porta de leitura para o próximo comando
            }
        }
        i++;
    }

    // Após lançar todos os processos, o pai espera todos eles terminarem.
    // O wait(NULL) retorna -1 quando não há mais nenhum filho vivo.
    while (wait(NULL) != -1)
        ;

    return (0);
}