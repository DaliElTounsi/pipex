NAME = pipex

SRC = src/main.c \
      src/exec.c \
      src/extract_cmd.c \
      parssing/parssing_1.c \
      fonctions_utils/fonction_utils_1.c \
      fonctions_utils/fonction_utils_2.c \

OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -Werror

LIBFT = libft/libft.a

all: $(NAME)

$(LIBFT):
	@make -C libft

%.o: %.c
	@cc $(CFLAGS) -Iinclude -c $< -o $@
	@echo "Compiled $< into $@"

$(NAME): $(LIBFT) $(OBJ)
	@cc $(OBJ) $(LIBFT) -o $(NAME)
	@echo "Linked $(NAME) successfully"

clean:
	@rm -f $(OBJ)
	@make -C libft clean
	@echo "Cleaned object files"

fclean: clean
	@rm -f $(NAME)
	@make -C libft fclean
	@echo "Cleaned executable and libft"

re: fclean all

.PHONY: all clean fclean re