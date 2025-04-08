/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fonction_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:25:18 by mohchams          #+#    #+#             */
/*   Updated: 2025/04/08 15:09:10 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	free_cmd_error(char *cmd1_base, char *cmd2_base, t_parse *list)
{
	if (!list->path_env)
	{
		free_cmd(cmd1_base, cmd2_base, list);
		write(2, "Error: PATH not found\n", 22);
		exit(1);
	}
	if (check_cmd_in_path(list, cmd1_base,
			"Error: cmd1 not found or not executable\n"))
	{
		free_cmd(cmd1_base, cmd2_base, list);
		exit(1);
	}
	if (check_cmd_in_path(list, cmd2_base,
			"Error: cmd2 not found or not executable\n"))
	{
		free_cmd(cmd1_base, cmd2_base, list);
		exit(1);
	}
}

void	init_list_path(t_parse *list, char *cmd)
{
	while (list->path)
	{
		ft_strlcpy(list->cmd_path, list->path, PATH_MAX);
		ft_strlcat(list->cmd_path, "/", PATH_MAX);
		ft_strlcat(list->cmd_path, cmd, PATH_MAX);
		if (access(list->cmd_path, X_OK) == 0)
		{
			list->found = 1;
			break ;
		}
		list->path = ft_strtok(NULL, ":");
	}
}

void	pip_is_fisrt(char *input_file, t_parse *list, int pipefd[2])
{
	int	fd;

	fd = open(input_file, O_RDONLY);
	if (fd == -1)
	{
		free(list);
		perror("Error: cannot open input file");
		exit(1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	dup2(pipefd[1], STDOUT_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
}

void	pip_is_second(char *output_file, t_parse *list, int pipefd[2])
{
	int	fd;

	fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(list);
		perror("Error: cannot open output file");
		exit(1);
	}
	dup2(pipefd[0], STDIN_FILENO);
	close(pipefd[0]);
	close(pipefd[1]);
	dup2(fd, STDOUT_FILENO);
	close(fd);
}

void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}
