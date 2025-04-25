#include "../include/pipex.h"

void error_alloc_failed(void)
{
    ft_putstr_fd("Error: memory allocation failed\n", 2);
    exit(1);
}

void error_cmd_not_found(void *list, char **args, char *temp_path)
{
    (void)temp_path;
    (void)list;
    free_args(args);
    ft_putstr_fd("Error: command not found or not executable\n", 2);
    exit(127);
}