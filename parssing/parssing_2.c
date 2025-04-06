
#include "../include/pipex.h"

int is_empty_cmd(char *cmd1, char *cmd2)
{
    if (ft_strlen(cmd1) == 0 || ft_strspn(cmd1, " \t\n") == ft_strlen(cmd1))
       return (write(2, "Error: cmd1 is empty\n", 21), 1);
    if (ft_strlen(cmd2) == 0 || ft_strspn(cmd2, " \t\n") == ft_strlen(cmd2))
        return (write(2, "Error: cmd2 is empty\n", 21), 1);
    return (0);
}

int check_cmd_format(char *cmd, const char *error_msg)
{
    int i = 0;

    while (cmd[i])
    {
        if (!ft_isalnum(cmd[i]) && cmd[i] != ' ' && cmd[i] != '-' && cmd[i] != '/' && cmd[i] != '.')
           return (write(2, error_msg, ft_strlen(error_msg)), 1);
        i++;
    }
    return (0);
}

char *extract_cmd(char *cmd)
{
    char *cmd_copy;
    char *token;
    char *result;

    cmd_copy = ft_strdup(cmd);
    if (!cmd_copy)
       return (write(2, "Error: memory allocation failed\n", 31), NULL);
    token = ft_strtok(cmd_copy, " \t\n");
    if (!token)
    {
        free(cmd_copy);
        return (write(2, "Error: invalid command\n", 23), NULL);
    }
    result = ft_strdup(token);
    free(cmd_copy);
    if (!result)
       return (write(2, "Error: memory allocation failed\n", 31), NULL);
    return (result);
}

int check_cmd_in_path(t_parse *list, char *cmd, const char *error_msg)
{
    list->path_copy = ft_strdup(list->path_env);
    if (!list->path_copy)
    {
        free(list);
        write(2, "Error: memory allocation failed\n", 31);
        return (1);
    }
    list->found = 0;
    list->path = ft_strtok(list->path_copy, ":");
    fonct_list_path(list, cmd);
    free(list->path_copy);
    if (!list->found)
    {
        write(2, error_msg, ft_strlen(error_msg));
        return (1);
    }
    return (0);
}

void parse_cmd(char *cmd1, char *cmd2)
{
    t_parse *list;
    char *cmd1_base;
    char *cmd2_base;

    list = init_parse_struct();
    if (is_empty_cmd(cmd1, cmd2))
        free_list_exit(list);
    if (check_cmd_format(cmd1, "Error: invalid characters in cmd1\n"))
        free_list_exit(list);
    if (check_cmd_format(cmd2, "Error: invalid characters in cmd2\n"))
        free_list_exit(list);
    cmd1_base = extract_cmd(cmd1);
    if (!cmd1_base)
        free_list_exit(list);
    cmd2_base = extract_cmd(cmd2);
    if (!cmd2_base)
    {
        free(cmd1_base);
        free_list_exit(list);
    }
    list->path_env = getenv("PATH");
    free_cmd_error(cmd1_base, cmd2_base, list);
    free_cmd(cmd1_base, cmd2_base, list);
}