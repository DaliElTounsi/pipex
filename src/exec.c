#include "../include/pipex.h"
#include <sys/wait.h> // Ajout pour wait

// Déclaration explicite de environ
extern char **environ;

char **split_cmd(char *cmd)
{
    char **args;

    args = ft_split(cmd, ' ');
    if (!args)
    {
        perror("Error: memory allocation failed");
        exit(1);
    }
    return (args);
}

char *find_cmd_path(char *cmd)
{
    t_parse *list;
    char *result;

    list = init_parse_struct();
    if (!list)
        return (NULL);
    list->path_env = getenv("PATH");
    if (!list->path_env)
    {
        free(list);
        perror("Error: PATH not found");
        return (NULL);
    }
    list->path_copy = ft_strdup(list->path_env);
    if (!list->path_copy)
    {
        free(list);
        return (NULL);
    }
    list->path = ft_strtok(list->path_copy, ":");
    init_list_path(list, cmd);
    free(list->path_copy);
    if (!list->found)
    {
        free(list);
        return (NULL);
    }
    result = ft_strdup(list->cmd_path);
    free(list);
    return (result);
}

void execute_cmd(char *cmd, char *input_file, char *output_file, int pipefd[2], int is_first)
{
    t_parse *list;
    pid_t pid;
    char **args;
    char *temp_path;

    list = init_parse_struct();
    if (!list)
    {
        perror("Error: memory allocation failed");
        exit(1);
    }

    pid = fork();
    if (pid == -1)
		free_perror(list, "Error: fork failed");
    if (pid == 0) // Processus fils
    {
        if (is_first) // cmd1
            pip_is_fisrt(input_file, list, pipefd);
        else // cmd2
            pip_is_second(output_file, list, pipefd);
        args = split_cmd(cmd);
        if (!args[0])
			free_perror(list, "Error: invalid command");
        // Trouver le chemin complet de la commande
        temp_path = find_cmd_path(args[0]);
        if (!temp_path)
        {
            free_args(args);
			free_perror(list, "Error: command not found");
        }
        ft_strlcpy(list->cmd_path, temp_path, PATH_MAX);
        free(temp_path);
        // Exécuter la commande avec execve
        execve(list->cmd_path, args, environ);
        // Si execve échoue, afficher une erreur
        free_args(args);
		free_perror(list, "Error: execve failed");
	}
    free(list); // Libère list dans le parent
}

void execute_pipex(char *file1, char *cmd1, char *cmd2, char *file2)
{
    int pipefd[2];

    if (pipe(pipefd) == -1)
    {
        perror("Error: pipe failed");
        exit(1);
    }
    execute_cmd(cmd1, file1, NULL, pipefd, 1); // cmd1
    execute_cmd(cmd2, NULL, file2, pipefd, 0); // cmd2
    close(pipefd[0]);
    close(pipefd[1]);
    wait(NULL); // Attendre cmd1
    wait(NULL); // Attendre cmd2
}