#ifndef PIPEX_H
# define

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <fcntl.h>

pipex.c Point d’entrée (main), parsing des arguments, coordination générale.
exec.c Gestion de l’exécution des commandes (fork, execve, redirections avec dup2).
pipe_management.c Création et gestion d’un seul pipe
utils.c Fonctions utilitaires (gestion d’erreurs avec perror, fermeture de descripteurs).

pipe_bonus.c Nouveau main pour les bonus, remplace pipex.c.
multi_pipe.c Gestion de plusieurs pipes pour les commandes enchaînées.
here_doc.c  Traitement du here_doc (lecture jusqu’au LIMITER, fichier temporaire).
exec_bonus.c Adaptation de l’exécution pour plusieurs commandes.


# endif