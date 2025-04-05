#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>


/** @brief Vérifie les arguments de Pipex et s'assure que file1 et file2 sont accessibles.
 *  @param ac Nombre d'arguments passés au programme.
 *  @param av Tableau des arguments (av[1] = file1, av[ac-1] = file2).
 *  @return Ne retourne rien, mais quitte avec exit(1) en cas d'erreur.
 */
void    parse_args(int ac, char **av);
/* pipex.c Point d’entrée (main), parsing des arguments, coordination générale.
exec.c Gestion de l’exécution des commandes (fork, execve, redirections avec dup2).
pipe_management.c Création et gestion d’un seul pipe
utils.c Fonctions utilitaires (gestion d’erreurs avec perror, fermeture de descripteurs).

pipe_bonus.c Nouveau main pour les bonus, remplace pipex.c.
multi_pipe.c Gestion de plusieurs pipes pour les commandes enchaînées.
here_doc.c  Traitement du here_doc (lecture jusqu’au LIMITER, fichier temporaire).
exec_bonus.c Adaptation de l’exécution pour plusieurs commandes. */



# endif