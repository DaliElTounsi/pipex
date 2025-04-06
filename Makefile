NAME = pipex

SRC = src/main.c src/exec.c parssing/parssing_1.c parssing/parssing_2.c fonctions_utils/fonction_utils_1.c \
	fonctions_utils/fonction_utils_2.c
OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror
LIBFT = libft/libft.a

## @brief Compile l'exécutable pipex (règle par défaut).
all: $(NAME)

## @brief Compile la bibliothèque libft.a si nécessaire.
$(LIBFT):
	@make -C libft

## @brief Compile les fichiers .c en .o
%.o: %.c
	@cc $(CFLAGS) -c $< -o $@

## @brief Lie les fichiers objets avec libft.a pour créer l'exécutable pipex.
$(NAME): $(LIBFT) $(OBJ)
	@cc $(OBJ) $(LIBFT) -o $(NAME)

## @brief Supprime les fichiers objets et nettoie libft.
clean:
	@rm -f $(OBJ)
	@make -C libft clean

## @brief Supprime l'exécutable, les fichiers objets, et nettoie libft complètement.
fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean

## @brief Nettoie tout et recompile.
re: fclean all

.PHONY: all clean fclean re