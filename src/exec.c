#include "../include/pipex.h"

extern char **environ;

char **split_cmd(char *cmd)
{
    char    **args;

    args = ft_split(cmd, ' ');
    if (!args)
    {
        ft_putstr_fd("Error: memory allocation failed\n", 2);
        exit(1);
    }
    return (args);
}

void execute_cmd(t_args_file *list_f, int pipefd[2], int is_first)
{
    pid_t   pid;

    pid = fork();
    if (pid == -1)
    {
        ft_putstr_fd("Error: fork failed\n", 2);
        exit(1);
    }
    if (pid == 0)
        execute_cmd2(list_f, pipefd, is_first);
}

void execute_cmd2(t_args_file *list_f, int pipefd[2], int is_first)
{
    if (is_first)
    {
        pip_is_fisrt(list_f->input_file, NULL, pipefd);
        list_f->args = list_f->cmd1_args;
    }
    else
    {
        pip_is_second(list_f->output_file, NULL, pipefd);
        list_f->args = list_f->cmd2_args;
    }
    if (!list_f->args[0])
        exit(127);
    execve(list_f->args[0], list_f->args, environ);
    free_args(list_f->args);
    ft_putstr_fd("Error: execve failed\n", 2);
    exit(1);
}

int execute_pipex(char *file1, char *file2, t_args_file *list_f)
{
    int pipefd[2];
    int status;
    pid_t pid2;

    if (pipe(pipefd) == -1)
    {
        ft_putstr_fd("Error: pipe failed\n", 2);
        return (1);
    }
    list_f->input_file = file1;
    execute_cmd(list_f, pipefd, 1);
    wait(&status);
    list_f->input_file = NULL;
    list_f->output_file = file2;
    execute_cmd(list_f, pipefd, 0);
    pid2 = wait(&status);
    close(pipefd[0]);
    close(pipefd[1]);
    free_args(list_f->cmd1_args);
    free_args(list_f->cmd2_args);
    if (pid2 != -1 && WIFEXITED(status))
        return (WEXITSTATUS(status));
    return (0);
}