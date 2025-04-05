#include "../include/pipex.h"

void    parse_args(int ac, char **av)
{
    (void)av;

    if (ac < 5)
    {
        write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37);
        exit(1);
    }
    if (access(av[1], F_OK | R_OK) == -1)
    {
        perror("Error: file1");
        // ft_printf("Error: file1: %s\n", strerror(errno));
        exit(1);
    }
    if (access(av[ac - 1], W_OK) == -1)
    {
        perror("Error: file2");
        exit(1);
    }
}

int main(int ac, char **av)
{
    parse_args(ac, av);
    ft_putchar_fd('\n', 1);
    return (0);
}
