#include "../include/pipex.h"

void pip_is_fisrt(char *input_file, void *list, int pipefd[2])
{
    int fd;

    (void)list;
    fd = open(input_file, O_RDONLY);
    if (fd == -1)
    {
        dup2(STDIN_FILENO, pipefd[0]);
        close(pipefd[0]);
        close(pipefd[1]);
        return ;
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
}

void pip_is_second(char *output_file, void *list, int pipefd[2])
{
    int fd;

    (void)list;
    if (ft_strncmp(output_file, "/dev/stdout", ft_strlen("/dev/stdout")) == 0)
    {
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        return ;
    }
    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        ft_putstr_fd("Error: file2: Permission denied\n", 2);
        exit(1);
    }
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    close(pipefd[1]);
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void free_args(char **args)
{
    int i;

    if (!args)
        return ;
    i = 0;
    while (args[i])
    {
        free(args[i]);
        i++;
    }
    free(args);
}

void free_cmd(char *cmd1, char *cmd2, void *list)
{
    if (cmd1)
        free(cmd1);
    if (cmd2)
        free(cmd2);
    (void)list;
}