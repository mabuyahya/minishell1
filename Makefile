#wanted
#wanted
NAME = minishell

#compile
CC = cc

CFLAGS = -Wall -Werror -Wextra -g

#dir
LIBFT_DIR = libft

LIBFT_H_DIR = libft/include

INC_DIR = include
#file

SRC_FILES = *.c

SRC_FILES_WHIT_PREFIX = $(addprefix msrcs/, $(SRC_FILES))

OBJ_FILES = $(SRC_FILES_WHIT_PREFIX:srcs/%.c=obj/%.o)
#libs

LIBFT = $(LIBFT_DIR)/libft.a

LIBS = -L$(LIBFT_DIR) -lft
#Header

HEADERS =  -I$(INC_DIR) -I$(LIBFT_H_DIR)
#rules

all:LIBFT $(NAME)

$(NAME): $(LIBFT) $(OBJ_FILES)
	$(CC) $(CFLAGS) $(OBJ_FILES) $(HEADERS) $(LIBS) -lreadline -o $(NAME)

obj/%.o: msrcs/%.c $(INC_DIR)/minishell.h
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

LIBFT:
	$(MAKE) -s -C $(LIBFT_DIR)

clean:
	$(MAKE) -s -C $(LIBFT_DIR) clean
	rm -rf obj

fclean: clean
	$(MAKE) -s -C $(LIBFT_DIR) fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
