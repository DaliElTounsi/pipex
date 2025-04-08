/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:36:00 by mohchams          #+#    #+#             */
/*   Updated: 2025/04/08 15:10:59 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

t_parse	*init_parse_struct(void)
{
	t_parse	*list;

	list = (t_parse *)malloc(sizeof(t_parse));
	if (!list)
	{
		write(2, "Error: memory allocation failed\n", 31);
		exit(1);
	}
	init_list(list);
	return (list);
}

int	check_arg_count(int ac, t_parse *list)
{
	if (ac < 5)
	{
		free(list);
		return (write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37), 1);
	}
	return (0);
}

int	check_file1(char *file1, t_parse *list)
{
	if (access(file1, F_OK | R_OK) == -1)
		return (free_perror(list, "Error: file1"), 1);
	return (0);
}

int	check_file2(t_parse *list)
{
	if (access(list->file2, W_OK) == 0)
		return (0);
	if (errno == ENOENT)
	{
		ft_strlcpy(list->parent_dir, list->file2, PATH_MAX);
		list->last_slash = ft_strrchr(list->parent_dir, '/');
		if (list->last_slash == NULL)
			ft_strlcpy(list->parent_dir, ".", 2);
		else if (list->last_slash == list->parent_dir)
			ft_strlcpy(list->parent_dir, "/", 2);
		else
		{
			list->i = list->last_slash - list->parent_dir;
			list->parent_dir[list->i] = '\0';
		}
		if (access(list->parent_dir, W_OK) == -1)
			return (free_perror(list, "Error: file2 (parent directory)"), 1);
	}
	else
		return (free_perror(list, "Error: file2"), 1);
	return (0);
}

void	parse_args(int ac, char **av)
{
	t_parse	*list;

	list = init_parse_struct();
	if (check_arg_count(ac, list))
		exit(1);
	if (check_file1(av[1], list))
		exit(1);
	list->file2 = av[ac - 1];
	if (check_file2(list))
		exit(1);
	parse_cmd(av[2], av[3]);
	free(list);
}
