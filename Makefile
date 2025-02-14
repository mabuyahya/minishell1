# Shell and Compiler
CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I include -I libft/include

# Utilities
RM = rm -f
AR = ar rcs

# Directories
SRC_DIR = src
OBJ_DIR = obj
LIBFT_DIR = libft

# Libft
LIBFT = $(LIBFT_DIR)/libft.a


# Binary Name
BIN = minishell
NAME = $(BIN)

# Source Files
SRC = main.c builtins.c ft_strtrim_all.c exec.c \
      fill_node.c get_params.c handle_quote.c \
      expand.c heredoc.c error.c env.c custom_cmd.c \
      ft_cmdsubsplit.c signal.c parse_args.c get_cmd.c \
      matrix_things.c utilities.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

# Default Target
all: $(NAME)

# Build Executable
$(NAME): $(LIBFT) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -lreadline -o $@

# Compile Source Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Build Libft
$(LIBFT): | $(LIBFT_DIR)
	make -C $(LIBFT_DIR)

# Cleanup
clean:
	make clean -C $(LIBFT_DIR)
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)
	$(RM) $(LIBFT)

# Create Directories if Needed
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Recompile Everything
re: fclean all

.PHONY: all clean fclean re
