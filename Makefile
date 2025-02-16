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
SRC = builtins.c \
	custom_cmd.c \
	env.c \
	error.c \
	exec.c \
	expand_path.c \
	expand_var.c \
	fill_node_util.c \
	ft_cmdsubsplit.c \
	ft_strtrim_all.c \
	get_cmd.c \
	get_cmd_2.c \
	get_params.c \
	handle_quote.c \
	heredoc.c \
	main.c \
	matrix_things.c \
	parse_args.c \
	signal.c \
	utilities.c \
	fill_creat_node.c \
	handle_echo_pwd_cd.c \
	utils_2.c \
	mini_unset.c \
	mini_export.c \
	utils_3.c \

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
	make -s -C $(LIBFT_DIR)

# Cleanup
clean:
	make clean -s -C $(LIBFT_DIR)
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
