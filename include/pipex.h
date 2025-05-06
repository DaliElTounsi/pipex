/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:34:34 by mohchams          #+#    #+#             */
/*   Updated: 2025/05/06 18:44:27 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>

void	first_child(char **av, char **env, int *fd);
void	second_child(char **av, char **env, int *fd);
char	*search_in_paths(char *cmd_name, char **env);
char	*get_cmd(char *cmd_str, char **env);
void	free_split(char **tab);

#endif