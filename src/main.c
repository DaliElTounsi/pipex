#include "../include/pipex.h"

int main(int ac, char **av)
{
    parse_args(ac, av);
    execute_pipex(av[1], av[2], av[3], av[4]);
    return (0);
}