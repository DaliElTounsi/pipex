NAME = pipex

SRC = src/main.c
OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a

## @brief Compile l'exécutable pipex (règle par défaut).
all: $(NAME)

## @brief Compile la bibliothèque libft.a si nécessaire.
$(LIBFT):
	make -C libft

## @brief Lie les fichiers objets avec libft.a pour créer l'exécutable pipex.
$(NAME): $(LIBFT) $(OBJ)
	cc $(OBJ) $(LIBFT) -o $(NAME)

## @brief Supprime les fichiers objets et nettoie libft.
clean:
	rm -f $(OBJ)
	make -C libft clean

## @brief Supprime l'exécutable, les fichiers objets, et nettoie libft complètement.
fclean: clean
	rm -f $(NAME)
	make -C libft fclean

## @brief Nettoie tout et recompile.
re: fclean all