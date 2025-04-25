#include "../include/pipex.h"

int check_arg_count(int ac)
{
    if (ac < 5)
    {
        ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", 2);
        return (1);
    }
    return (0);
}

void parse_args(int ac, char **av)
{
    t_args_file list_f;
    int status;

    if (check_arg_count(ac))
        exit(1);
    if (check_file1(av[1]))
    {
        ft_putstr_fd("Error: file1: No such file or directory\n", 2);
    }
    if (check_file2_with_path(av[ac - 1]))
        exit(1);
    ft_memset(&list_f, 0, sizeof(t_args_file));
    parse_cmd(av[2], av[3], &list_f);
    status = execute_pipex(av[1], av[4], &list_f);
    if (status != 0)
        exit(status);
}

int check_file1(char *file1)
{
    if (access(file1, F_OK | R_OK) == -1)
        return (1);
    return (0);
}

int check_file2_with_path(const char *path)
{
    char    parent_dir[PATH_MAX];
    char    *last_slash;
    int     i;

    if (access(path, W_OK) == 0)
        return (0);
    if (errno == ENOENT)
    {
        ft_strlcpy(parent_dir, path, PATH_MAX);
        last_slash = ft_strrchr(parent_dir, '/');
        if (last_slash == NULL)
            ft_strlcpy(parent_dir, ".", 2);
        else if (last_slash == parent_dir)
            ft_strlcpy(parent_dir, "/", 2);
        else
        {
            i = last_slash - parent_dir;
            parent_dir[i] = '\0';
        }
        if (access(parent_dir, W_OK) == -1)
        {
            ft_putstr_fd("Error: file2 (parent directory): Permission denied\n", 2);
            return (1);
        }
    }
    else
    {
        ft_putstr_fd("Error: file2: Permission denied\n", 2);
        return (1);
    }
    return (0);
}

void parse_cmd(char *cmd1, char *cmd2, t_args_file *list_f)
{
    list_f->cmd1_args = extract_cmd(cmd1);
    if (list_f->cmd1_args[0])
        check_cmd_executable(list_f->cmd1_args);
    list_f->cmd2_args = extract_cmd(cmd2);
    if (list_f->cmd2_args[0])
        check_cmd_executable(list_f->cmd2_args);
}