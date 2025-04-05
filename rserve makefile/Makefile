# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/26 16:21:28 by mohchams          #+#    #+#              #
#    Updated: 2025/02/11 10:16:29 by mohchams         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #



NAME = libftprintf.a

SRCS = ft_putchar_printf.c ft_putstr_printf.c\
	ft_putnbr_hexa_printf.c ft_strlen.c ft_printf.c ft_putnbr_unsigned.c\
	ft_putnbr_printf.c ft_percent_printf.c ft_putptr_printf.c\
	
HEADER = "ft_printf.h"
OBJS = $(SRCS:.c=.o)

CC = cc
CFLAGS = -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all
