# Shell and Compiler
CC = cc
CFLAGS = -Wall -Wextra -Werror -I include -I libft/include

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
SRC = builtins.c \
	env.c \
	handle_errors.c \
	execute.c \
	expand_path.c \
	expand_var.c \
	creat_node_3.c \
	split_separator.c \
	delete_quotes.c \
	get_cmd.c \
	get_params.c \
	handle_quote.c \
	heredoc.c \
	main.c \
	matrix_things.c \
	parse_args.c \
	signal.c \
	utilities.c \
	creat_node.c \
	creat_node_2.c \
	handle_echo_pwd.c \
	utils_2.c \
	handle_unset.c \
	handle_export.c \
	utils_3.c \
	handle_cd.c \

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
