/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:36:00 by mohchams          #+#    #+#             */
/*   Updated: 2025/05/06 13:12:34 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/pipex.h"

void	free_split(char **tab)
{
	int	i;

	if (!tab)
		return ;
	i = 0;
	while (tab[i])
		free(tab[i++]);
	free(tab);
}

char	*get_cmd(char *cmd_str, char **env)
{
	char	**cmd;
	char	*path;

	cmd = ft_split(cmd_str, ' ');
	if (!cmd || !cmd[0])
		return (write(2, "command not found\n", 18), free_split(cmd), NULL);
	if (access(cmd[0], X_OK) == 0)
	{
		path = ft_strdup(cmd[0]);
		return (free_split(cmd), path);
	}
	path = search_in_paths(cmd[0], env);
	free_split(cmd);
	return (path);
}

char	*search_in_paths(char *cmd_name, char **env)
{
	char	**paths;
	char	*path;
	int		i;

	i = 0;
	while (env[i] && !ft_strnstr(env[i], "PATH=", 5))
		i++;
	if (!env[i])
		return (write(2, "PATH not found\n", 15), NULL);
	paths = ft_split(env[i] + 5, ':');
	if (!paths)
		return (write(2, "malloc fail\n", 12), NULL);
	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd_name);
		if (access(path, X_OK) == 0)
			return (free_split(paths), path);
		free(path);
	}
	free_split(paths);
	write(2, "command not found\n", 18);
	return (NULL);
}

void	child_proc(char **av, char **env, int *fd)
{
	int		fd_in;
	char	*path;
	char	**cmd;

	fd_in = open(av[1], O_RDONLY);
	if (fd_in < 0)
		return (perror("infile"), close(fd[0]), close(fd[1]), exit(1));
	if (dup2(fd_in, STDIN_FILENO) < 0 || dup2(fd[1], STDOUT_FILENO) < 0)
		return (perror("dup2"), close(fd_in),
			close(fd[0]), close(fd[1]), exit(1));
	close(fd_in);
	close(fd[0]);
	close(fd[1]);
	path = get_cmd(av[2], env);
	if (!path)
		exit(1);
	cmd = ft_split(av[2], ' ');
	if (!cmd)
		return (write(2, "memory allocation failed\n", 25), free(path), exit(1));
	execve(path, cmd, env);
	perror("execve");
	free(path);
	free_split(cmd);
	exit(1);
}

void	second_child(char **av, char **env, int *fd)
{
	int		fd_out;
	char	*path;
	char	**cmd;

	fd_out = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out < 0)
		return (perror("outfile"), close(fd[0]), close(fd[1]), exit(1));
	if (dup2(fd[0], STDIN_FILENO) < 0 || dup2(fd_out, STDOUT_FILENO) < 0)
		return (perror("dup2"), close(fd_out),
			close(fd[0]), close(fd[1]), exit(1));
	close(fd_out);
	close(fd[0]);
	close(fd[1]);
	path = get_cmd(av[3], env);
	if (!path)
		exit(1);
	cmd = ft_split(av[3], ' ');
	if (!cmd)
		return (write(2, "memory allocation failed\n", 25), free(path), exit(1));
	execve(path, cmd, env);
	perror("execve");
	free(path);
	free_split(cmd);
	exit(1);
}
