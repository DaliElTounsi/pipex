#include "pipex.h"

int main(int ac, int *av)
{
    if (ac < 5)
    {
        ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");
        write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37);
        exit(1);
    }

    ft_putchar('\n');
    return (0);
}
