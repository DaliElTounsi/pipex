/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:42:16 by mohchams          #+#    #+#             */
/*   Updated: 2025/05/06 20:00:24 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex.h"

void	funct_forks(char **av, char **env, int *fd)
{
	pid_t	pid1;
	pid_t	pid2;

	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork");
		exit(1);
	}
	if (pid1 == 0)
		first_child(av, env, fd);
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("fork");
		exit(1);
	}
	if (pid2 == 0)
		second_child(av, env, fd);
	close(fd[0]);
	close(fd[1]);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
}

int	main(int ac, char **av, char **env)
{
	int	fd[2];

	if (ac != 5)
		return (ft_putendl_fd("usage: ./pipex file1 cmd1 cmd2 file2", 2), 2);
	if (!env)
		return (ft_putendl_fd("env not found", 2), 1);
	if (pipe(fd) < 0)
		return (perror("pipe"), 1);
	funct_forks(av, env, fd);
	return (0);
}
