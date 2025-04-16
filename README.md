# pipex
Projet du cercle 2 Ecole42
Project PIPEX  

Nouvelles Fonctions 

Perror 

Prototype 	void perror(const char *s); 
 

Rôle : Affiche un message d'erreur sur la sortie standard d'erreur (stderr) pour signaler un problème lié à un appel système. 

Utilisation : Prend une chaîne de caractères personnalisée (ex. "Erreur fichier") et ajoute une description de l'erreur basée sur errno. 

Pourquoi stderr ? : Permet de séparer les erreurs des sorties normales (stdout), utile quand on redirige les résultats (ex. > fichier). 

Exemple dans Pipex : Si open échoue sur file1, perror peut afficher "Erreur fichier: No such file or directory". 

strerror 

Prototype : 	char *strerror(int errnum); 
 

Rôle : Retourne une chaîne de caractères décrivant une erreur à partir d’un code d’erreur (ex. valeur de errno). 

Utilisation : Prend un entier (code d’erreur) et renvoie un texte lisible (ex. "No such file or directory" pour ENOENT). 

Différence avec perror : Ne fait qu fournir la description (pas d’affichage direct), te laissant libre de l’utiliser comme tu veux. 

Exemple dans Pipex : Si execve échoue, strerror(errno) peut renvoyer "No such file or directory" pour une commande introuvable. 

 

access 

Prototype : 	int access(const char *pathname, int mode); 
 

Rôle : Vérifie l’existence ou les permissions d’un fichier (lecture, écriture, exécution). 

Utilisation : Prend un chemin de fichier et un mode (F_OK pour existence, R_OK pour lecture, W_OK pour écriture, X_OK pour exécution). 

Retour : 0 si OK, -1 si échec (avec errno mis à jour). 

Exemple dans Pipex : Tester si file1 existe et est lisible avant open, ou si une commande comme "ls" est exécutable avant execve. 

 

dup 

Prototype :  	int  dup(int oldfd); 

Rôle : Duplique un descripteur de fichier pour créer un nouveau descripteur pointant vers la même ressource. 

Utilisation : Prend un descripteur existant (ex. de open ou pipe) et renvoie un nouveau numéro de descripteur. 

Retour : Nouveau descripteur (le plus petit disponible) ou -1 si échec (avec errno). 

Exemple dans Pipex : Dupliquer le descripteur de file1 ou d’un pipe pour rediriger les entrées/sorties de cmd1 ou cmd2. 

Dup2 

Prototype :  	int dup2(int oldfd, int newfd); 
 

Rôle : Duplique un descripteur de fichier en le redirigeant vers un numéro spécifique choisi. 

Utilisation : Prend un descripteur source (oldfd) et un cible (newfd), remplaçant newfd s’il est déjà ouvert. 

Retour : newfd si succès, -1 si échec (avec errno). 

Exemple dans Pipex : Rediriger file1 vers stdin (dup2(fd_file1, 0)) ou une extrémité de pipe vers stdout pour cmd1. 

Execve 

Prototype :  int execve(const char *pathname, char *const argv[], char *const envp[]); 

 
Rôle : Remplace le processus actuel par un nouveau programme exécutable. 

Utilisation : Prend un chemin (ex. "/bin/ls"), un tableau d’arguments (ex. {"ls", "-l", NULL}) et un tableau d’environnements. 

Retour : Ne retourne rien si succès (processus remplacé), -1 si échec (avec errno). 

Exemple dans Pipex : Exécuter cmd1 (ex. "ls -l") dans un processus fils après redirection avec dup2. 

exit 

Prototype : 	void exit(int status); 
 

Rôle : Termine le processus en cours et renvoie un statut au système. 

Utilisation : Prend un entier (0 pour succès, autre pour erreur) et arrête le programme. 

Retour : Aucun (termine le processus). 

Exemple dans Pipex : Quitter un processus fils avec exit(1) si execve échoue, ou arrêter le main en cas d’erreur grave. 

 

 

fork 

Prototype : pid_t 	fork(void); 
 

Rôle : Crée un nouveau processus (fils) en dupliquant le processus actuel (parent). 

Utilisation : Sépare l’exécution en deux processus indépendants. 

Retour : PID du fils (> 0) dans le parent, 0 dans le fils, -1 si échec (avec errno). 

Exemple dans Pipex : Créer un fils pour exécuter cmd1 (sortie vers pipe) et un autre pour cmd2 (entrée depuis pipe). 

Pipe 
	Prototype :  int 	pipe(int pipefd[2]); 

Rôle : Crée un tube pour connecter la sortie d’un processus à l’entrée d’un autre. 

Utilisation : Remplit un tableau pipefd[2] avec pipefd[0] (lecture) et pipefd[1] (écriture). 

Retour : 0 si succès, -1 si échec (avec errno). 

Exemple dans Pipex : Relier cmd1 (écrit dans pipefd[1]) à cmd2 (lit depuis pipefd[0]). 

 

unlink 

  	Prototype :	 int 	unlink(const char *pathname); 
 

Rôle : Supprime un fichier en enlevant son lien dans le répertoire. 

Utilisation : Prend le chemin du fichier à supprimer (ex. "tmp_input"). 

Retour : 0 si succès, -1 si échec (avec errno). 

Exemple dans Pipex : Effacer un fichier temporaire créé pour here_doc après usage. 

 

wait 

 	Prototype :	 pid_t 	wait(int *wstatus); 
 

Rôle : Attend la fin d’un processus fils et récupère son statut de sortie. 

Utilisation : Prend un pointeur pour stocker le statut (ou NULL si inutile). 

Retour : PID du fils terminé, ou -1 si échec (avec errno). 

Exemple dans Pipex : Attendre que les fils exécutant cmd1 et cmd2 terminent. 

waitpid 

Prototype : 	pid_t 	waitpid(pid_t pid, int *wstatus, int options); 
 

Rôle : Attend un processus fils spécifique ou tous, avec options de contrôle. 

Utilisation : Prend un PID (fils à attendre), un pointeur pour le statut, et des options (ex. WNOHANG). 

Retour : PID du fils terminé, 0 si non terminé (avec WNOHANG), ou -1 si échec (avec errno). 

Exemple dans Pipex : Attendre le fils de cmd1 ou tous les fils après fork. 

stdin, stdout, stderr 

Rôle : Flux standards en C pour les entrées/sorties. 

stdin : Entrée standard (n° 0), souvent le clavier, pour lire des données. 

stdout : Sortie standard (n° 1), souvent l’écran, pour les résultats normaux. 

stderr : Sortie d’erreur (n° 2), pour les messages d’erreur, séparée de stdout. 

Exemple dans Pipex : Rediriger file1 vers stdin, file2 vers stdout, et afficher les erreurs sur stderr. 

 

Niveau 0 : Sommet - main 

Rôle : Point d’entrée, parsing des arguments, coordination générale. 

Interactions :  

Vérifie les arguments (argc, argv). 

Appelle une fonction de parsing pour trier file1, commandes, file2, et détecter here_doc. 

Initialise les fichiers et pipes, puis lance l’exécution. 

Niveau 1 : Parsing et initialisation 

parse_args (fonction personnalisée)  

Rôle : Analyse argv pour extraire les éléments. 

Interactions :  

Si argv[1] == "here_doc", appelle une fonction pour gérer here_doc. 

Utilise access pour vérifier l’existence/permissions de file1 et file2. 

Retourne une structure avec les données (fichiers, commandes, nb de pipes). 

Fonctions appelées :  

access : Vérifie file1 (lecture) et file2 (écriture). 

perror / strerror : Si erreur (ex. fichier inexistant), affiche un message et exit. 

init_files (fonction personnalisée)  

Rôle : Ouvre les fichiers d’entrée/sortie. 

Interactions :  

open pour file1 (lecture) et file2 (écriture, avec création si besoin). 

Si here_doc, appelle une fonction pour créer un fichier temporaire. 

Fonctions appelées :  

open : Ouvre les fichiers. 

perror : Si échec d’ouverture, affiche une erreur et exit. 

handle_here_doc (fonction personnalisée, bonus)  

Rôle : Gère l’entrée here_doc jusqu’au LIMITER. 

Interactions :  

Crée un fichier temporaire avec open. 

Lit stdin avec read jusqu’au LIMITER et écrit dans le fichier avec write. 

Fonctions appelées :  

open, read, write : Gestion du fichier temporaire. 

close : Ferme le fichier temporaire après écriture. 

init_pipes (fonction personnalisée)  

Rôle : Crée les pipes nécessaires (1 pour obligatoire, N-1 pour multi-pipes). 

Interactions :  

Alloue un tableau de pipes selon le nombre de commandes. 

Crée chaque pipe. 

Fonctions appelées :  

pipe : Crée un tube [pipefd[0], pipefd[1]]. 

perror : Si échec, affiche une erreur et exit. 

Niveau 2 : Exécution des commandes 

execute_commands (fonction personnalisée)  

Rôle : Coordonne l’exécution des commandes avec fork. 

Interactions :  

Boucle sur chaque commande. 

Crée un fils pour chaque commande et configure les redirections. 

Attend la fin des fils. 

Fonctions appelées :  

fork : Crée un processus fils par commande. 

exec_cmd (voir ci-dessous) : Dans le fils, exécute la commande. 

waitpid : Dans le parent, attend les fils. 

close : Ferme les descripteurs inutilisés dans le parent. 

exec_cmd (fonction personnalisée)  

Rôle : Configure et exécute une commande dans un fils. 

Interactions :  

Redirige stdin et stdout selon la position de la commande (fichier, pipe, ou sortie). 

Lance la commande. 

Fonctions appelées :  

dup2 : Redirige stdin (de file1 ou pipe précédent) et stdout (vers pipe suivant ou file2). 

close : Ferme les descripteurs inutilisés dans le fils. 

execve : Exécute la commande (ex. "/bin/ls", {"ls", "-l", NULL}, environ). 

perror : Si execve échoue, affiche une erreur. 

exit : Quitte le fils après échec (exit(1)). 

Niveau 3 : Nettoyage 

cleanup (fonction personnalisée)  

Rôle : Ferme les ressources et supprime les fichiers temporaires. 

Interactions :  

Ferme tous les descripteurs de fichiers et pipes restants. 

Si here_doc, supprime le fichier temporaire. 

Fonctions appelées :  

close : Ferme les descripteurs. 

unlink : Supprime le fichier temporaire de here_doc. 

exit : Termine le programme avec un statut (ex. exit(0)). 

 

 

pipex/ 

├── include/        # Pour les fichiers header (.h) 

├── src/            # Sources de la partie obligatoire 

├── bonus/          # Sources des bonus (fichiers _bonus.c) 

├── libft/          # Ta libft (si autorisée et utilisée) 

└── Makefile        # À la racine 

main  

├── parse_args  

│ 	├── access  

│ 	├── perror / strerror  

│ 	└── exit  

├── init_files  

│ 	├── open  

│ 	├── handle_here_doc  

│ 	│ 	├── open  

│ 	│ 	├── read  

│ 	│ 	├── write  

│ 	│ 	└── close  

│ 	├── perror  

│ 	└── exit  

├── init_pipes  

│ 	├── pipe  

│ 	├── perror  

│ 	└── exit  

├── execute_commands  

│ 	├── fork  

│ 	├── exec_cmd  

│ 	│ 	├── dup2  

│ 	│ 	├── close  

│ 	│ 	├── execve  

│ 	│ 	├── perror  

│ 	│ 	└── exit  

│ 	├── waitpid  

│ 	└── close  

└── cleanup  

├── close  

├── unlink  

└── exit 

 

Explications des interactions 

Flux principal : main appelle les fonctions d’initialisation (parse_args, init_files, init_pipes), puis execute_commands pour lancer les processus, et enfin cleanup pour terminer. 

Gestion des erreurs : À chaque étape, si une fonction système (open, pipe, fork, etc.) échoue, perror ou strerror affiche l’erreur et exit termine proprement. 

Pipes et redirections : pipe crée les tubes, dup2 redirige les flux dans les fils, et execve exécute les commandes. 

Synchronisation : waitpid assure que le parent attend les fils, avec possibilité de gérer les multi-pipes dans l’ordre. 

Bonus : handle_here_doc ajoute une branche pour here_doc, et execute_commands boucle pour les multi-pipes. 

Questions de parsing pour Pipex 

Partie obligatoire : ./pipex file1 cmd1 cmd2 file2 

Y a-t-il assez d’arguments pour la partie obligatoire ?  

Minimum 5 arguments (./pipex, file1, cmd1, cmd2, file2) ? 

Le fichier d’entrée file1 existe-t-il et est-il lisible ?  

Peut-on utiliser access pour vérifier son existence et les permissions de lecture ? 

Le fichier de sortie file2 est-il accessible ou peut-il être créé ?  

Peut-on vérifier les permissions d’écriture dans le répertoire avec access ? 

Les commandes cmd1 et cmd2 sont-elles valides et exécutables ?  

Peut-on trouver leur chemin dans le PATH avec access et X_OK ? 

Les arguments sont-ils bien formés (pas de chaînes vides ou mal placées) ?  

cmd1 et cmd2 contiennent-ils au moins une commande non vide ? 

Bonus : Multi-pipes ./pipex file1 cmd1 cmd2 cmd3 ... file2 

Combien de commandes sont passées en arguments ?  

Peut-on compter le nombre de commandes entre file1 et file2 pour savoir combien de pipes créer ? 

Le nombre d’arguments est-il cohérent avec un multi-pipe ?  

Minimum 6 arguments (./pipex, file1, au moins 2 commandes, file2) ? 

Chaque commande intermédiaire est-elle valide et exécutable ?  

Peut-on vérifier chaque cmdX dans le PATH comme pour cmd1 et cmd2 ? 

Bonus : here_doc ./pipex here_doc LIMITER cmd cmd1 file 

Le premier argument est-il "here_doc" ?  

Si oui, passe-t-on en mode here_doc au lieu d’un fichier d’entrée classique ? 

Y a-t-il assez d’arguments pour here_doc ?  

Minimum 6 arguments (./pipex, "here_doc", LIMITER, cmd, cmd1, file) ? 

Le LIMITER est-il une chaîne valide (non vide) ?  

Peut-on vérifier que argv[2] n’est pas une chaîne vide ou un espace seul ? 

Les commandes cmd et cmd1 sont-elles valides dans le cas here_doc ?  

Même vérification que pour la partie obligatoire avec access ? 

Le fichier de sortie file est-il accessible ou créable dans le cas here_doc ?  

Identique à file2, peut-on écrire dedans ou le créer ? 

Gestion des erreurs générales 

Que faire si un argument est manquant ou invalide ?  

Doit-on afficher un message d’erreur avec perror ou strerror et quitter avec exit ? 

Comment gérer les permissions insuffisantes sur les fichiers ?  

Si access échoue, doit-on signaler l’erreur et arrêter ? 

Que faire si une commande n’est pas trouvée dans le PATH ?  

Doit-on prévoir un message spécifique avant de quitter ? 

 

Analyse 

Partie obligatoire : Les questions 1 à 5 couvrent le cas de base avec 2 commandes. 

Multi-pipes : Les questions 6 à 8 ajoutent la gestion d’un nombre variable de commandes. 

Here_doc : Les questions 9 à 13 adaptent le parsing pour ce mode spécial. 

Erreurs : Les questions 14 à 16 assurent une gestion robuste des cas d’échec. 

Cours Complet : Création du Programme Pipex

Introduction : Objectif et Contexte
Bienvenue dans ce cours détaillé sur la création du programme Pipex, un projet essentiel de l'école 42 qui vous permettra de maîtriser les concepts de redirection et de communication entre processus.
Qu'est-ce que Pipex ?
Pipex est un programme qui simule le comportement du pipe (	) dans un shell Unix. Il prend en entrée :
Un fichier source (	)
Une première commande (  ) Une deuxième commande (  ) Un fichier de destination (	)
 
Le programme exécute finalement le résultat dans
 
sur le contenu de
.
 
, puis passe la sortie à
 
via un pipe, et écrit
 
Exemple concret

bash


./pipex /etc/passwd "ls -l" "wc -l" file2

Ce que cette commande fait :
1.	Lit le contenu de
2.	Exécute	sur ce contenu
3.	Passe la sortie à	via un pipe
4.	Écrit le résultat dans
C'est équivalent à la commande shell :

bash


< /etc/passwd ls -l | wc -l > file2

Structure du Cours
Pour construire ce programme étape par étape, nous allons diviser notre cours en chapitres clairs et structurés :
 
1.	Structure du Projet et Concepts de Base
2.	Parsing des Arguments et des Fichiers
3.	Parsing des Commandes
4.	Fonctions Utilitaires pour le Parsing
5.	Fonctions Utilitaires pour l'Exécution
6.	Exécution des Commandes avec un Pipe
7.	Point d'Entrée du Programme
8.	Gestion des Erreurs et Nettoyage
9.	Conclusion et Prochaines Étapes
Chaque chapitre présente d'abord les concepts théoriques, puis les met en pratique avec du code commenté et explicite.

Chapitre 1 : Structure du Projet et Concepts de Base
Théorie : Organisation et Concepts Fondamentaux
Avant de commencer à coder, il est essentiel de comprendre l'architecture globale de notre programme et les concepts clés qu'il utilise.
Concepts clés de Pipex
1.	Parsing
  Vérification du nombre d'arguments
  Validation des fichiers source et destination   Contrôle de la validité des commandes
2.	Recherche des commandes
  Exploration de la variable d'environnement PATH   Vérification de l'exécutabilité des commandes
3.	Exécution avec pipe
   Création d'un pipe pour connecter les commandes
Utilisation de	pour créer des processus fils
  Redirection des entrées/sorties avec   Exécution des commandes avec
4.	Gestion de la mémoire
  Allocation appropriée
   Libération systématique pour éviter les fuites
 
Organisation des fichiers
Pour maintenir notre code propre et modulaire, nous organiserons le projet en plusieurs fichiers :

Fichier	Rôle
main.c
Point d'entrée, coordination générale
parssing_1.c
Vérification des arguments et fichiers
parssing_2.c
Vérification des commandes
fonction_utils_1.c
Fonctions utilitaires pour le parsing
fonction_utils_2.c
Fonctions utilitaires pour l'exécution
exec.c
Exécution des commandes avec pipe
pipex.h
Définitions des structures et prototypes
C	C
Pratique : Création de pipex.h
Commençons par créer le fichier d'en-tête qui définira notre structure principale et les prototypes de toutes nos fonctions.
 
#ifndef PIPEX_H # define PIPEX_H

# include "../libft/libft.h" # include <unistd.h>
# include <stdio.h> # include <stdlib.h> # include <limits.h> # include <string.h> # include <fcntl.h> # include <errno.h>
# include <linux/limits.h> # include <sys/wait.h>

typedef struct s_parse
{
char *path_env;		// Stocke la variable d'environnement PATH char *path;	// Stocke un répertoire extrait de PATH char *path_copy;	// Copie de path_env pour la modifier
char cmd_path[PATH_MAX]; // Chemin complet d'une commande (ex. "/bin/ls") int	found;	// Drapeau indiquant si une commande a été trouvée char *file2;	// Chemin du fichier de sortie
char *last_slash;	// Dernier '/' dans le chemin de file2
char parent_dir[PATH_MAX]; // Dossier parent de file2
int	i;	// Variable temporaire pour les calculs
} t_parse;

// Prototypes des fonctions de parssing_1.c void	parse_args(int ac, char **av); t_parse *init_parse_struct(void);
int	check_arg_count(int ac, t_parse *list); int	check_file1(char *file1, t_parse *list); int	check_file2(t_parse *list);
void	free_perror(t_parse *list, char *str);
 
// Prototypes des fonctions de parssing_2.c void	parse_cmd(char *cmd1, char *cmd2); int	is_empty_cmd(char *cmd1, char *cmd2);
int	check_cmd_format(char *cmd, const char *error_msg); char	*extract_cmd(char *cmd);
int	check_cmd_in_path(t_parse *list, char *cmd, const char *error_msg);

// Prototypes des fonctions de fonction_utils_1.c
void	init_list(t_parse *list);
void	fonct_list_path(t_parse *list, char *cmd); void	free_list_exit(t_parse *list);
void	free_cmd(char *cmd1, char *cmd2, t_parse *list);

// Prototypes des fonctions de fonction_utils_2.c
void	free_cmd_error(char *cmd1_base, char *cmd2_base, t_parse *list); void	init_list_path(t_parse *list, char *cmd);
void	pip_is_fisrt(char *input_file, t_parse *list, int pipefd[2]); void	pip_is_second(char *output_file, t_parse *list, int pipefd[2]); void	free_args(char **args);

// Prototypes des fonctions de exec.c
void	execute_pipex(char *file1, char *cmd1, char *cmd2, char *file2); #endif
Explication : Ce fichier d'en-tête définit la structure	qui sera utilisée tout au long du
programme pour stocker les informations nécessaires au parsing et à l'exécution. Il déclare également tous les prototypes des fonctions, regroupés par fichier source pour plus de clarté.

Chapitre 2 : Parsing des Arguments et des Fichiers
Théorie : Vérification des Arguments et des Fichiers
Le parsing est la première étape cruciale dans Pipex. Avant de pouvoir exécuter quoi que ce soit, nous devons nous assurer que les entrées du programme sont valides et accessibles.
Les vérifications essentielles
1.	Nombre d'arguments
  Le programme doit recevoir exactement 5 arguments   Format :
2.	Fichier d'entrée (file1)
  Doit exister
 
  Doit être lisible
3.	Fichier de sortie (file2)
  Doit être accessible en écriture
  Si le fichier n'existe pas, son répertoire parent doit être accessible en écriture

Pratique : Création de parssing_1.c
Implémentons ces vérifications dans notre premier fichier de parsing :
 
#include "../include/pipex.h" t_parse *init_parse_struct(void)
{
t_parse *list;

list = (t_parse *)malloc(sizeof(t_parse)); if (!list)
{
write(2, "Error: memory allocation failed\n", 31); exit(1);
}
init_list(list); return (list);
}

int check_arg_count(int ac, t_parse *list)
{
if (ac < 5)
{
free(list);
return (write(2, "Usage: ./pipex file1 cmd1 cmd2 file2\n", 37), 1);
}
return (0);
}

int check_file1(char *file1, t_parse *list)
{
if (access(file1, F_OK | R_OK) == -1)
return (free_perror(list, "Error: file1"), 1); return (0);
}

int check_file2(t_parse *list)
 
{
if (access(list->file2, W_OK) == 0) return (0);
if (errno == ENOENT)
{
ft_strlcpy(list->parent_dir, list->file2, PATH_MAX); list->last_slash = ft_strrchr(list->parent_dir, '/'); if (list->last_slash == NULL)
ft_strlcpy(list->parent_dir, ".", 2);
else if (list->last_slash == list->parent_dir) ft_strlcpy(list->parent_dir, "/", 2);
else
{
list->i = list->last_slash - list->parent_dir; list->parent_dir[list->i] = '\0';
}
if (access(list->parent_dir, W_OK) == -1)
return (free_perror(list, "Error: file2 (parent directory)"), 1);
}
else
return (free_perror(list, "Error: file2"), 1); return (0);
}

void parse_args(int ac, char **av)
{
t_parse *list;

list = init_parse_struct(); if (check_arg_count(ac, list))
{
free(list); exit(1);
}
if (check_file1(av[1], list))
{
free(list); exit(1);
}
list->file2 = av[ac - 1]; if (check_file2(list))
{
free(list); exit(1);
}
parse_cmd(av[2], av[3]);
 
free(list);
}

 
Analyse détaillée :
1.	init_parse_struct : Alloue et initialise une structure
 

pour stocker les données de parsing.
 
Cette structure est essentielle pour centraliser les informations.
2.	check_arg_count : Vérifie que le nombre d'arguments est suffisant (au moins 5). En cas d'erreur, affiche un message d'usage.
3.	check_file1 : S'assure que le fichier d'entrée existe et est lisible en utilisant	avec les
 
flags
 
(existence) et
 
(permission de lecture).
 
4.	check_file2 : Gère le cas plus complexe du fichier de sortie :   Si le fichier existe, vérifie qu'il est accessible en écriture
  Si le fichier n'existe pas, vérifie que son répertoire parent est accessible   Gère les cas particuliers comme les chemins absolus ou relatifs
5.	parse_args : Fonction principale qui coordonne toutes les vérifications et gère proprement les libérations de mémoire.

Chapitre 3 : Parsing des Commandes
Théorie : Vérification des Commandes
Après avoir vérifié les arguments et les fichiers, nous devons nous assurer que les commandes à exécuter sont valides.
Critères de validité d'une commande
1.	Non vide
  La commande ne doit pas être une chaîne vide
  Elle ne doit pas être composée uniquement d'espaces
2.	Format valide
  Ne doit contenir que des caractères autorisés
   Les caractères spéciaux du shell comme	,	,	sont interdits
3.	Exécutable
  La commande doit exister dans l'un des répertoires du PATH   Le fichier doit avoir les permissions d'exécution
Pratique : Création de parssing_2.c
Implémentons ces vérifications pour les commandes :
 
#include "../include/pipex.h"

int is_empty_cmd(char *cmd1, char *cmd2)
{
if (ft_strlen(cmd1) == 0 || ft_strspn(cmd1, " \t\n") == ft_strlen(cmd1)) return (write(2, "Error: cmd1 is empty\n", 21), 1);
if (ft_strlen(cmd2) == 0 || ft_strspn(cmd2, " \t\n") == ft_strlen(cmd2)) return (write(2, "Error: cmd2 is empty\n", 21), 1);
return (0);
}

int check_cmd_format(char *cmd, const char *error_msg)
{
int i;

i = 0;
while (cmd[i])
{
if (!ft_isalnum(cmd[i]) && cmd[i] != ' '
&& cmd[i] != '-' && cmd[i] != '/' && cmd[i] != '.') return (write(2, error_msg, ft_strlen(error_msg)), 1);
i++;
}
return (0);
}

char *extract_cmd(char *cmd)
{
char *cmd_copy; char *token; char *result;

cmd_copy = ft_strdup(cmd); if (!cmd_copy)
 
return (write(2, "Error: memory allocation failed\n", 31), NULL); token = ft_strtok(cmd_copy, " \t\n");
if (!token)
{
free(cmd_copy);
return (write(2, "Error: invalid command\n", 23), NULL);
}
result = ft_strdup(token); free(cmd_copy);
if (!result)
return (write(2, "Error: memory allocation failed\n", 31), NULL); return (result);
}

int check_cmd_in_path(t_parse *list, char *cmd, const char *error_msg)
{
list->path_copy = ft_strdup(list->path_env); if (!list->path_copy)
{
free(list);
write(2, "Error: memory allocation failed\n", 31); return (1);
}
list->found = 0;
list->path = ft_strtok(list->path_copy, ":"); fonct_list_path(list, cmd);
free(list->path_copy); if (!list->found)
{
write(2, error_msg, ft_strlen(error_msg)); return (1);
}
return (0);
}

void parse_cmd(char *cmd1, char *cmd2)
{
t_parse *list; char *cmd1_base; char *cmd2_base;

list = init_parse_struct(); if (is_empty_cmd(cmd1, cmd2))
free_list_exit(list);
if (check_cmd_format(cmd1, "Error: invalid characters in cmd1\n")) free_list_exit(list);
if (check_cmd_format(cmd2, "Error: invalid characters in cmd2\n"))
 
free_list_exit(list); cmd1_base = extract_cmd(cmd1); if (!cmd1_base)
free_list_exit(list); cmd2_base = extract_cmd(cmd2); if (!cmd2_base)
{
free(cmd1_base); free_list_exit(list);
}
list->path_env = getenv("PATH"); free_cmd_error(cmd1_base, cmd2_base, list); free_cmd(cmd1_base, cmd2_base, list);
}

Points importants :
1.	is_empty_cmd : Détecte les commandes vides ou constituées uniquement d'espaces, de tabulations ou de retours à la ligne.
2.	check_cmd_format : Parcourt chaque caractère de la commande pour s'assurer qu'il est valide (alphanumérique, espace, tiret, barre oblique ou point).
3.	extract_cmd : Isole la commande principale sans ses arguments (ex: "ls -l" → "ls"). Cette extraction est cruciale pour la recherche dans le PATH.
4.	check_cmd_in_path : Vérifie si la commande existe dans l'un des répertoires du PATH et si elle est exécutable.
5.	parse_cmd : Orchestre toutes ces vérifications et assure une libération adéquate des ressources à chaque étape.

Chapitre 4 : Fonctions Utilitaires pour le Parsing
Théorie : Fonctions Utilitaires
Pour maintenir notre code propre et modulaire, nous allons créer un ensemble de fonctions utilitaires qui seront utilisées par les différentes parties du programme.
Types de fonctions utilitaires
1.	Initialisation
  Préparation des structures de données   Attribution de valeurs par défaut
2.	Recherche dans le PATH
   Découpage de la variable PATH en répertoires
  Test d'existence et d'exécutabilité des commandes
 
3.	Gestion des erreurs
  Libération de mémoire
  Affichage de messages d'erreur   Arrêt propre du programme
Pratique : Création de fonction_utils_1.c
 
#include "../include/pipex.h" void init_list(t_parse *list)
{
list->path_env = NULL; list->path = NULL; list->path_copy = NULL;
ft_memset(list->cmd_path, 0, PATH_MAX);
list->found = 0; list->file2 = NULL;
list->last_slash = NULL;
ft_memset(list->parent_dir, 0, PATH_MAX);
list->i = 0;
}

void fonct_list_path(t_parse *list, char *cmd)
{
while (list->path)
{
ft_strlcpy(list->cmd_path, list->path, PATH_MAX); ft_strlcat(list->cmd_path, "/", PATH_MAX); ft_strlcat(list->cmd_path, cmd, PATH_MAX);
if (access(list->cmd_path, X_OK) == 0)
{
list->found = 1; break;
}
list->path = ft_strtok(NULL, ":");
}
}

void free_perror(t_parse *list, char *str)
{
free(list);
 
perror(str); exit(1);
}

void free_cmd(char *cmd1, char *cmd2, t_parse *list)
{
free(cmd1); free(cmd2); free(list);
}

void free_list_exit(t_parse *list)
{
free(list); exit(1);
}

Fonctions clés :
1.	init_list : Initialise tous les champs de la structure à des valeurs sûres (NULL ou 0), évitant ainsi des comportements indéfinis.
2.	fonct_list_path : Méthode cruciale qui parcourt les répertoires du PATH pour trouver une commande exécutable. Pour chaque répertoire, elle construit un chemin complet et teste son exécutabilité.
3.	free_perror : Combine la libération de mémoire avec l'affichage d'un message d'erreur système
via	.
4.	free_cmd et free_list_exit : Fonctions spécialisées pour libérer différentes combinaisons de ressources avant de quitter.

Chapitre 5 : Fonctions Utilitaires pour l'Exécution
Théorie : Utilitaires pour l'Exécution
La partie exécution de Pipex nécessite également des fonctions utilitaires spécifiques, principalement pour :
1.	Gestion des erreurs d'exécution
  Vérification des commandes avant exécution   Libération des ressources en cas d'échec
2.	Configuration des redirections
  Redirection pour la première commande (entrée depuis file1, sortie vers le pipe)   Redirection pour la seconde commande (entrée depuis le pipe, sortie vers file2)
3.	Libération des structures complexes
 
  Nettoyage des tableaux d'arguments   Fermeture des descripteurs de fichiers
Pratique : Création de fonction_utils_2.c
 
#include "../include/pipex.h"

void free_cmd_error(char *cmd1_base, char *cmd2_base, t_parse *list)
{
if (!list->path_env)
{
free_cmd(cmd1_base, cmd2_base, list); write(2, "Error: PATH not found\n", 22); exit(1);
}
if (check_cmd_in_path(list, cmd1_base, "Error: cmd1 not found or not executable\n"))
{
free_cmd(cmd1_base, cmd2_base, list); exit(1);
}
if (check_cmd_in_path(list, cmd2_base, "Error: cmd2 not found or not executable\n"))
{
free_cmd(cmd1_base, cmd2_base, list); exit(1);
}
}

void init_list_path(t_parse *list, char *cmd)
{
while (list->path)
{
ft_strlcpy(list->cmd_path, list->path, PATH_MAX); ft_strlcat(list->cmd_path, "/", PATH_MAX); ft_strlcat(list->cmd_path, cmd, PATH_MAX);
if (access(list->cmd_path, X_OK) == 0)
{
list->found = 1; break;
}
 
list->path = ft_strtok(NULL, ":");
}
}

void pip_is_fisrt(char *input_file, t_parse *list, int pipefd[2])
{
int fd;

fd = open(input_file, O_RDONLY); if (fd == -1)
{
free(list);
perror("Error: cannot open input file"); exit(1);
}
dup2(fd, STDIN_FILENO);
close(fd);
dup2(pipefd[1], STDOUT_FILENO); close(pipefd[0]); close(pipefd[1]);
}

void pip_is_second(char *output_file, t_parse *list, int pipefd[2])
{
int fd;

fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); if (fd == -1)
{
free(list);
perror("Error: cannot open output file"); exit(1);
}
dup2(pipefd[0], STDIN_FILENO); close(pipefd[0]); close(pipefd[1]);
dup2(fd, STDOUT_FILENO);
close(fd);
}

void free_args(char **args)
{
int i;

if (!args) return;
i = 0;
 
while (args[i])
{
free(args[i]); i++;
}
free(args);
}

Fonctionnalités essentielles :
1.	free_cmd_error : Vérifie la variable PATH et l'exécutabilité des commandes, libérant les ressources et quittant en cas d'erreur.
2.	init_list_path : Similaire à	, cherche une commande dans le PATH. Ce doublon
est utilisé spécifiquement dans le processus d'exécution.
3.	pip_is_fisrt : Configure les redirections pour la première commande:   Redirige l'entrée standard depuis le fichier d'entrée
  Redirige la sortie standard vers l'extrémité d'écriture du pipe   Ferme les descripteurs inutiles
4.	pip_is_second : Configure les redirections pour la deuxième commande:   Redirige l'entrée standard depuis l'extrémité de lecture du pipe
  Redirige la sortie standard vers le fichier de sortie   Ferme les descripteurs inutiles
5.	free_args : Libère proprement un tableau d'arguments, en parcourant chaque élément puis en libérant le tableau lui-même.

Chapitre 6 : Exécution des Commandes avec un Pipe
Théorie : Exécution des Commandes
L'exécution est le cœur fonctionnel de Pipex. Nous devons :
1.	Créer un pipe pour connecter les deux commandes
2.	Créer des processus fils pour exécuter chaque commande
3.	Configurer les redirections d'entrée/sortie appropriées
4.	Exécuter les commandes avec execve
5.	Attendre la fin des processus fils

Pratique : Création de exec.c
 
#include "../include/pipex.h"

// Déclaration explicite de environ
extern char **environ;

char **split_cmd(char *cmd)
{
char **args;

args = ft_split(cmd, ' '); if (!args)
{
perror("Error: memory allocation failed"); exit(1);
}
return (args);
}

char *find_cmd_path(char *cmd)
{
t_parse *list; char *result;

list = init_parse_struct(); if (!list)
return (NULL);
list->path_env = getenv("PATH"); if (!list->path_env)
{
free(list);
perror("Error: PATH not found"); return (NULL);
}
list->path_copy = ft_strdup(list->path_env);
 
if (!list->path_copy)
{
free(list); return (NULL);
}
list->path = ft_strtok(list->path_copy, ":"); init_list_path(list, cmd);
free(list->path_copy);
if (!list->found)
{
free(list); return (NULL);
}
result = ft_strdup(list->cmd_path); free(list);
return (result);
}

void execute_cmd(char *cmd, char *input_file, char *output_file, int pipefd[2], int is_first)
{
t_parse *list; pid_t pid; char **args;
char *temp_path;

list = init_parse_struct(); if (!list)
{
perror("Error: memory allocation failed"); exit(1);
}
pid = fork(); if (pid == -1)
free_perror(list, "Error: fork failed"); if (pid == 0)
{
if (is_first)
pip_is_fisrt(input_file, list, pipefd); else
pip_is_second(output_file, list, pipefd);
args = split_cmd(cmd); if (!args[0])
free_perror(list, "Error: invalid command"); temp_path = find_cmd_path(args[0]);
if (!temp_path)
{
free_args(args);
 
free_perror(list, "Error: command not found");
}
ft_strlcpy(list->cmd_path, temp_path, PATH_MAX); free(temp_path);
execve(list->cmd_path, args, environ); free_args(args);
free_perror(list, "Error: execve failed");
}
free(list);
}

void execute_pipex(char *file1, char *cmd1, char *cmd2, char *file2)
{
int pipefd[2];

if (pipe(pipefd) == -1)
{
perror("Error: pipe failed"); exit(1);
}
execute_cmd(cmd1, file1, NULL, pipefd, 1); execute_cmd(cmd2, NULL, file2, pipefd, 0); close(pipefd[0]);
close(pipefd[1]); wait(NULL); wait(NULL);
}


Analyse du processus d'exécution :
1.	split_cmd : Divise une commande avec ses arguments en un tableau de chaînes adapté pour
.
2.	find_cmd_path : Cherche le chemin complet d'une commande dans le PATH. Retourne une copie du chemin pour éviter les problèmes de portée et de mémoire.
3.	execute_cmd : Fonction complexe qui gère l'exécution d'une commande :   Crée un processus fils avec
  Configure les redirections appropriées selon si c'est la première ou la seconde commande   Transforme la commande en tableau d'arguments
  Trouve le chemin complet de la commande   Exécute la commande avec
   Gère les erreurs potentielles à chaque étape
4.	execute_pipex : Fonction principale d'exécution :
 
Crée un pipe pour connecter les deux commandes
Exécute la première commande (lisant depuis file1, écrivant dans le pipe) Exécute la seconde commande (lisant depuis le pipe, écrivant dans file2) Ferme les descripteurs du pipe dans le processus parent
Attend la fin des deux processus fils

Chapitre 7 : Point d'Entrée du Programme
Théorie : Point d'Entrée
Maintenant que nous avons implémenté toutes les fonctionnalités nécessaires, nous pouvons créer le
point d'entrée de notre programme, la fonction	. C'est elle qui coordonne toutes les étapes :

1.	Parsing des arguments et commandes
2.	Exécution des commandes via le pipe
3.	Retour du code de sortie

Pratique : Création de main.c

#include "../include/pipex.h" int main(int ac, char **av)
{
parse_args(ac, av);
execute_pipex(av[1], av[2], av[3], av[4]); return (0);
}

Simplicité élégante :
 
Notre fonction	est remarquablement concise car elle délègue tout le travail aux fonctions
spécialisées que nous avons créées :
1.	vérifie les arguments et les commandes
2.	exécute les commandes
3.	indique une exécution réussie
Cette approche modulaire facilite la compréhension et la maintenance du code.

Chapitre 8 : Gestion des Erreurs et Nettoyage
Théorie : Gestion des Erreurs
Une partie essentielle de notre programme est la gestion rigoureuse des erreurs et la libération correcte des ressources. Tout au long de notre code, nous avons :
1.	Vérifié les retours de toutes les fonctions sensibles
2.	Libéré la mémoire allouée en cas d'erreur
3.	Affiché des messages d'erreur explicites
4.	Fermé les descripteurs de fichiers

Pratique : Vérification avec Valgrind
Une fois le programme compilé, il est crucial de vérifier qu'il ne contient pas de fuites de mémoire. Valgrind est l'outil idéal pour cela :

bash


valgrind --leak-check=full --show-leak-kinds=all ./pipex /etc/passwd "ls -l" "wc -l" file2

Un résultat correct devrait afficher :


==XXXX== HEAP SUMMARY:
==XXXX==	in use at exit: 0 bytes in 0 blocks
==XXXX==	total heap usage: 10 allocs, 10 frees, 34,264 bytes allocated
==XXXX==
==XXXX== All heap blocks were freed -- no leaks are possible

Cette sortie confirme que toutes les allocations mémoire ont été correctement libérées, démontrant la robustesse de notre gestion des ressources.
Résultat de l'exécution
 
Nous pouvons également vérifier que notre programme fonctionne correctement en examinant le contenu du fichier de sortie :

bash


cat file2

 
Ce fichier devrait contenir le nombre de lignes produites par la commande est exactement ce que nous attendions.
 
, ce qui
 

 
Chapitre 9 : Conclusion et Prochaines Étapes
Récapitulatif des Accomplissements
Félicitations ! Vous avez créé avec succès un programme Pipex qui :
1.	Vérifie rigoureusement les arguments et fichiers d'entrée/sortie
2.	Localise les commandes dans le PATH
3.	Exécute les commandes en utilisant un pipe pour les connecter
4.	Gère correctement les erreurs et libère la mémoire
5.	Maintient un code modulaire et bien structuré

Prochaines Étapes
Si vous souhaitez approfondir ce projet, voici quelques pistes d'amélioration :

Partie Bonus
Le projet Pipex peut être étendu pour prendre en charge :
1.	Multiples pipes
bash


./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2

Équivalent à :
2.	Here document (here_doc)
bash


./pipex here_doc LIMITER cmd1 cmd2 file2

Équivalent à :

Tests Approfondis
 
Pour vous assurer de la robustesse de votre programme, testez-le avec :
  Fichiers inexistants ou sans permissions   Commandes invalides ou introuvables
  Combinaisons complexes de commandes   Entrées très volumineuses
Optimisations
Quelques améliorations possibles :
   Réduire la duplication de code (ex: fonct_list_path et init_list_path)
  Améliorer la gestion des erreurs avec des codes de retour plus précis   Ajouter des commentaires détaillés pour chaque fonction

Récapitulatif du Code Source
Voici la structure complète de notre projet Pipex :


pipex/
├── include/
│	└── pipex.h	# Définitions et prototypes
├── src/
│	├── main.c	# Point d'entrée
│	└── exec.c	# Exécution des commandes
├── parssing/
│	├── parssing_1.c	# Vérification des arguments
│	└── parssing_2.c	# Vérification des commandes
├── fonctions_utils/
│	├── fonction_utils_1.c # Utilitaires pour le parsing
│	└── fonction_utils_2.c # Utilitaires pour l'exécution
├── libft/	# Bibliothèque de fonctions utilitaires
└── Makefile	# Règles de compilation

Cette organisation modulaire facilite la compréhension et la maintenance du code, tout en séparant clairement les différentes responsabilités.
Flux de l'Exécution
1.	Démarrage
  Le programme commence dans
2.	Parsing
 

 


3.	Exécution


 
vérifie les arguments et fichiers vérifie les commandes

crée un pipe
exécute les commandes dans des processus fils
 
   Les redirections sont gérées par	et
4.	Finalisation
  Attente de la fin des processus fils   Libération des ressources
  Retour au shell

Ce flux clair et logique garantit que notre programme est à la fois robuste et maintenable.

Mot de la Fin
Ce cours vous a guidé pas à pas dans la création d'un programme qui simule le comportement des pipes Unix. Au-delà du code lui-même, j'espère que vous avez acquis une compréhension approfondie des concepts clés :
  Processus et communication inter-processus   Redirections d'entrée/sortie
  Gestion des ressources   Conception modulaire
Ces compétences sont fondamentales pour tout développeur système et vous serviront dans de nombreux projets futurs.
La programmation système peut sembler intimidante au premier abord, mais en décomposant les problèmes en modules gérables comme nous l'avons fait ici, vous pouvez créer des programmes robustes et élégants.
Bon code !


 
