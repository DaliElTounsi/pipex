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

 
