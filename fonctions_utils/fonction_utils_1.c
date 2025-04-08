/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fonction_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:28:05 by mohchams          #+#    #+#             */
/*   Updated: 2025/04/08 14:29:15 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	init_list(t_parse *list)
{
	list->path_env = NULL;
	list->path = NULL;
	list->path_copy = NULL;
	ft_memset(list->cmd_path, 0, PATH_MAX);
	list->found = 0;
	list->file2 = NULL;
	list->last_slash = NULL;
	ft_memset(list->parent_dir, 0, PATH_MAX);
	list->i = 0;
}

void	fonct_list_path(t_parse *list, char *cmd)
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

void	free_perror(t_parse *list, char *str)
{
	{
		free(list);
		perror(str);
		exit(1);
	}
}

void	free_cmd(char *cmd1, char *cmd2, t_parse *list)
{
	free(cmd1);
	free(cmd2);
	free(list);
}

void	free_list_exit(t_parse *list)
{
	free(list);
	exit(1);
}
