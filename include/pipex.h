/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:34:34 by mohchams          #+#    #+#             */
/*   Updated: 2025/04/08 15:09:30 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <limits.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>
# include <linux/limits.h>
# include <sys/wait.h>

typedef struct s_parse
{
	char	*path_env;
	char	*path;
	char	*path_copy;
	char	cmd_path[PATH_MAX];
	int		found;
	char	*file2;
	char	*last_slash;
	char	parent_dir[PATH_MAX];
	int		i;
}	t_parse;

/** @brief Vérifie les arguments de Pipex et s'assure que
 *  file1 et file2 sont accessibles.
 *  @param ac Nombre d'arguments passés au programme.
 *  @param av Tableau des arguments (av[1] = file1, av[ac-1] = file2).
 *  @return Ne retourne rien, mais quitte avec exit(1) en cas d'erreur.
 */
void	parse_args(int ac, char **av);

/** @brief Vérifie que cmd1 et cmd2 ne sont pas vides et sont exécutables.
 *  @param cmd1 Première commande (av[2]).
 *  @param cmd2 Deuxième commande (av[3]).
 *  @return Ne retourne rien, mais quitte avec exit(1) en cas d'erreur.
 */
void	parse_cmd(char *cmd1, char *cmd2);

// parssing_1.c
/** @brief Alloue et initialise une structure t_parse.
 *  @return Pointeur vers la structure t_parse allouée, ou NULL en cas d'erreur.
 */
t_parse	*init_parse_struct(void);

/** @brief Vérifie le nombre d'arguments.
 *  @param ac Nombre d'arguments.
 *  @param list Structure t_parse pour libérer la mémoire en cas d'erreur.
 *  @return 0 si succès, 1 si erreur (ac < 5).
 */
int		check_arg_count(int ac, t_parse *list);

/** @brief Vérifie si file1 est accessible en lecture.
 *  @param file1 Chemin du fichier d'entrée.
 *  @param list Structure t_parse pour libérer la mémoire en cas d'erreur.
 *  @return 0 si succès, 1 si erreur (file1 non accessible).
 */
int		check_file1(char *file1, t_parse *list);

/** @brief Vérifie si file2 est accessible ou peut être créé.
 *  @param list Structure contenant les informations de parsing.
 *  @return 0 si succès, 1 si erreur (file2 non accessible).
 */
int		check_file2(t_parse *list);

/** @brief Libère la structure t_parse et
 *  affiche une erreur avec perror avant de quitter.
 *  @param list Structure t_parse à libérer.
 *  @param str Message d'erreur à afficher avec perror.
 *  @return Ne retourne rien, quitte avec exit(1).
 */
void	free_perror(t_parse *list, char *str);

// parssing_2.c
/** @brief Vérifie si cmd1 et cmd2 sont vides ou composées uniquement d'espaces.
 *  @param cmd1 Première commande (av[2]).
 *  @param cmd2 Deuxième commande (av[3]).
 *  @return 0 si succès, 1 si erreur (cmd1 ou cmd2 vide).
 */
int		is_empty_cmd(char *cmd1, char *cmd2);

/** @brief Vérifie que la commande ne contient pas de caractères invalides.
 *  @param cmd Commande à vérifier.
 *  @param error_msg Message d'erreur à afficher en cas d'échec.
 *  @return 0 si succès, 1 si erreur (caractères invalides).
 */
int		check_cmd_format(char *cmd, const char *error_msg);

/** @brief Extrait la commande de base d'une chaîne (ex. "ls -l" -> "ls").
 *  @param cmd Chaîne contenant la commande et ses arguments.
 *  @return Pointeur vers la commande de base allouée, ou NULL en cas d'erreur.
 */
char	*extract_cmd(char *cmd);

/** @brief Cherche une commande dans le PATH et 
 *  vérifie si elle est exécutable.
 *  @param list Structure contenant les informations de parsing.
 *  @param cmd Commande à chercher.
 *  @param error_msg Message d'erreur à afficher si la 
 * commande n'est pas trouvée.
 *  @return 0 si succès, 1 si erreur (commande non trouvée ou non exécutable).
 */
int		check_cmd_in_path(t_parse *list, char *cmd, const char *error_msg);

// fonction_utils_1.c
/** @brief Initialise les champs d'une structure t_parse.
 *  @param list Structure t_parse à initialiser.
 *  @return Ne retourne rien.
 */
void	init_list(t_parse *list);

/** @brief Cherche une commande dans les dossiers du PATH.
 *  @param list Structure contenant les informations de parsing.
 *  @param cmd Commande à chercher.
 *  @return Ne retourne rien, met à jour list->found si la commande est trouvée.
 */
void	fonct_list_path(t_parse *list, char *cmd);

/** @brief Libère la structure t_parse et
 *  quitte le programme avec un code d'erreur.
 *  @param list Structure t_parse à libérer.
 *  @return Ne retourne rien, quitte avec exit(1).
 */
void	free_list_exit(t_parse *list);

/** @brief Libère deux commandes et la structure t_parse.
 *  @param cmd1 Première commande à libérer.
 *  @param cmd2 Deuxième commande à libérer.
 *  @param list Structure t_parse à libérer.
 *  @return Ne retourne rien.
 */
void	free_cmd(char *cmd1, char *cmd2, t_parse *list);

// fonction_utils_2.c
/** @brief Vérifie path_env et les commandes,
 *  libère les ressources et quitte en cas d'erreur.
 *  @param cmd1_base Commande de base extraite de cmd1.
 *  @param cmd2_base Commande de base extraite de cmd2.
 *  @param list Structure t_parse contenant les informations de parsing.
 *  @return Ne retourne rien, quitte avec exit(1) en cas d'erreur.
 */
void	free_cmd_error(char *cmd1_base, char *cmd2_base, t_parse *list);

/** @brief Cherche une commande dans les dossiers du PATH.
 *  @param list Structure contenant les informations de parsing.
 *  @param cmd Commande à chercher.
 *  @return Ne retourne rien, met à jour list->found
 *  et list->cmd_path si la commande est trouvée.
 */
void	init_list_path(t_parse *list, char *cmd);

/** @brief Configure les redirections pour la première commande (cmd1).
 *  @param input_file Fichier d'entrée à ouvrir.
 *  @param list Structure t_parse pour libérer la mémoire en cas d'erreur.
 *  @param pipefd Tableau contenant les descripteurs du pipe.
 *  @return Ne retourne rien, quitte avec exit(1) en cas d'erreur.
 */
void	pip_is_fisrt(char *input_file, t_parse *list, int pipefd[2]);

/** @brief Exécute les commandes cmd1 et cmd2 en utilisant un pipe.
 *  @param file1 Fichier d'entrée.
 *  @param cmd1 Première commande.
 *  @param cmd2 Deuxième commande.
 *  @param file2 Fichier de sortie.
 *  @return Ne retourne rien, mais quitte avec exit(1) en cas d'erreur.
 */
/** @brief Configure les redirections pour la deuxième commande (cmd2).
 *  @param output_file Fichier de sortie à ouvrir.
 *  @param list Structure t_parse pour libérer la mémoire en cas d'erreur.
 *  @param pipefd Tableau contenant les descripteurs du pipe.
 *  @return Ne retourne rien, quitte avec exit(1) en cas d'erreur.
 */
void	pip_is_second(char *output_file, t_parse *list, int pipefd[2]);

/** @brief Libère un tableau de chaînes (comme args).
 *  @param args Tableau de chaînes à libérer.
 *  @return Ne retourne rien.
 */
void	free_args(char **args);

/** @brief Exécute les commandes cmd1 et cmd2 en utilisant un pipe.
 *  @param file1 Fichier d'entrée.
 *  @param cmd1 Première commande.
 *  @param cmd2 Deuxième commande.
 *  @param file2 Fichier de sortie.
 *  @return Ne retourne rien, mais quitte avec exit(1) en cas d'erreur.
 */
void	execute_pipex(char *file1, char *cmd1, char *cmd2, char *file2);

/* pipex.c Point d’entrée (main), parsing des arguments, coordination générale.
exec.c Gestion de l’exécution des commandes (fork, execve,
 redirections avec dup2).
pipe_management.c Création et gestion d’un seul pipe
utils.c Fonctions utilitaires (gestion d’erreurs avec perror,
 fermeture de descripteurs).

pipe_bonus.c Nouveau main pour les bonus, remplace pipex.c.
multi_pipe.c Gestion de plusieurs pipes pour les commandes enchaînées.
here_doc.c  Traitement du here_doc (lecture jusqu’au LIMITER,
 fichier temporaire).
exec_bonus.c Adaptation de l’exécution pour plusieurs commandes. */

#endif