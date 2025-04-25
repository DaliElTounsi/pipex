#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/wait.h>

# ifndef PATH_MAX
#  define PATH_MAX 4096
# endif

typedef struct s_args_file
{
    char    *input_file;
    char    *output_file;
    char    **args;
    char    *temp_path;
    char    **cmd1_args;
    char    **cmd2_args;
} t_args_file;

typedef struct s_path_info
{
    char    *path_env;
    char    *path_copy;
    char    *dir;
    char    *temp_path;
    int     found;
} t_path_info;

void    parse_args(int ac, char **av);
int     check_arg_count(int ac);
int     check_file1(char *file1);
int     check_file2_with_path(const char *path);
char    **extract_cmd(char *cmd);
int     check_cmd_executable(char **args);
void    parse_cmd(char *cmd1, char *cmd2, t_args_file *list_f);
void    execute_cmd(t_args_file *list_f, int pipefd[2], int is_first);
void    execute_cmd2(t_args_file *list_f, int pipefd[2], int is_first);
void    pip_is_fisrt(char *input_file, void *list, int pipefd[2]);
void    pip_is_second(char *output_file, void *list, int pipefd[2]);
int     execute_pipex(char *file1, char *file2, t_args_file *list_f);
char    **split_cmd(char *cmd);
void    free_args(char **args);
void    free_cmd(char *cmd1, char *cmd2, void *list);
void    error_alloc_failed(void);
void    error_cmd_not_found(void *list, char **args, char *temp_path);

#endif