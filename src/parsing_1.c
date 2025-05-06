/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:36:00 by mohchams          #+#    #+#             */
/*   Updated: 2025/05/06 19:59:17 by mohchams         ###   ########.fr       */
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
		return (ft_putendl_fd("command not found", 2), free_split(cmd), NULL);
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
		return (ft_putendl_fd("PATH not found", 2), NULL);
	paths = ft_split(env[i] + 5, ':');
	if (!paths)
		return (ft_putendl_fd("malloc fail", 2), NULL);
	i = -1;
	while (paths[++i])
	{
		path = ft_strjoin(ft_strjoin(paths[i], "/"), cmd_name);
		if (access(path, X_OK) == 0)
			return (free_split(paths), path);
		free(path);
	}
	free_split(paths);
	ft_putendl_fd("command not found", 2);
	return (NULL);
}

void	first_child(char **av, char **env, int *fd)
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
		return (ft_putendl_fd("memory alloc failed", 2), free(path), exit(1));
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
		return (ft_putendl_fd("memory alloc failed", 2), free(path), exit(1));
	execve(path, cmd, env);
	perror("execve");
	free(path);
	free_split(cmd);
	exit(1);
}
