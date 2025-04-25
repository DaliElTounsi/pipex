#include "../include/pipex.h"

char **extract_cmd(char *cmd)
{
    char    **args;
    char    *temp;
    int     i, j, len, in_quotes;

    if (!cmd || !cmd[0])
    {
        args = malloc(sizeof(char *) * 2);
        if (!args)
        {
            ft_putstr_fd("Error: memory allocation failed\n", 2);
            exit(1);
        }
        args[0] = NULL;
        args[1] = NULL;
        return (args);
    }

    len = strlen(cmd);
    args = malloc(sizeof(char *) * (len + 1));
    if (!args)
    {
        ft_putstr_fd("Error: memory allocation failed\n", 2);
        exit(1);
    }
    i = 0;
    j = 0;
    in_quotes = 0;
    temp = malloc(len + 1);
    if (!temp)
    {
        free_args(args);
        ft_putstr_fd("Error: memory allocation failed\n", 2);
        exit(1);
    }
    temp[0] = '\0';

    while (i < len)
    {
        while (i < len && (cmd[i] == ' ' || cmd[i] == '\t') && !in_quotes)
            i++;
        if (i >= len)
            break ;
        j = 0;
        while (i < len)
        {
            if (cmd[i] == '"')
            {
                in_quotes = !in_quotes;
                i++;
                continue;
            }
            if (cmd[i] == ' ' && !in_quotes)
                break ;
            temp[j++] = cmd[i++];
        }
        temp[j] = '\0';
        args[j] = ft_strdup(temp);
        if (!args[j])
        {
            free_args(args);
            free(temp);
            ft_putstr_fd("Error: memory allocation failed\n", 2);
            exit(1);
        }
        j++;
    }
    args[j] = NULL;
    free(temp);
    return (args);
}

static int check_absolute_paths(char **args)
{
    int i;

    i = 0;
    while (args[i])
    {
        if (args[i][0] == '/' && access(args[i], X_OK) != 0)
        {
            ft_putstr_fd("Error: command not found or not executable\n", 2);
            return (127);
        }
        i++;
    }
    return (0);
}

static char *build_and_test_path(char *dir, char *cmd, char **args)
{
    char    *temp_path;

    temp_path = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
    if (!temp_path)
    {
        free_args(args);
        ft_putstr_fd("Error: memory allocation failed\n", 2);
        exit(1);
    }
    ft_strlcpy(temp_path, dir, ft_strlen(dir) + 1);
    ft_strlcat(temp_path, "/", ft_strlen(dir) + 2);
    ft_strlcat(temp_path, cmd, ft_strlen(dir) + ft_strlen(cmd) + 2);
    if (access(temp_path, X_OK) == 0)
        return (temp_path);
    free(temp_path);
    return (NULL);
}

static int search_path(t_path_info *info, char **args)
{
    info->path_env = getenv("PATH");
    if (!info->path_env)
    {
        ft_putstr_fd("Error: PATH not found\n", 2);
        return (127);
    }
    info->path_copy = ft_strdup(info->path_env);
    if (!info->path_copy)
    {
        free_args(args);
        ft_putstr_fd("Error: memory allocation failed\n", 2);
        exit(1);
    }
    info->dir = strtok(info->path_copy, ":");
    info->found = 0;
    while (info->dir)
    {
        info->temp_path = build_and_test_path(info->dir, args[0], args);
        if (info->temp_path)
        {
            info->found = 1;
            free(args[0]);
            args[0] = info->temp_path;
            break ;
        }
        info->dir = strtok(NULL, ":");
    }
    free(info->path_copy);
    if (!info->found)
    {
        ft_putstr_fd("Error: command not found or not executable\n", 2);
        return (127);
    }
    return (0);
}

int check_cmd_executable(char **args)
{
    t_path_info info;

    int status = check_absolute_paths(args);
    if (status != 0)
        return (status);
    if (args[0][0] != '/')
        return (search_path(&info, args));
    return (0);
}