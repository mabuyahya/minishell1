/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:22:54 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/19 13:06:30 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <dirent.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/wait.h>

typedef struct s_expand_read
{
	int		in_double_quote;
	int		in_single;
	char	*ptr;
	size_t	offset;
	char	*new_line;
}			t_expand_read;

typedef struct s_expand_read_2
{
	char	*key_eend;
	char	*env_value;
	char	*key;
	char	*expanded;
	size_t	len;
	size_t	new_len;
	size_t	prefix_len;
}			t_expand_read_2;

typedef struct s_prompt
{
	t_list	*cmds;
	char	**envp;
	int		size;
	int		exit_status;
	int		count_make_node;
	int		flag;
}			t_prompt;

typedef struct t_node_content
{
	char	**full_cmd;
	char	*full_path;
	int		infile;
	int		outfile;
}			t_node_content;

typedef struct s_expand_var
{
	char	*aux;
	int		pos;
	char	*path;
	char	*var;
}			t_expand_var;

typedef struct s_set_env
{
	int		i;
	char	*new_entry;
	char	*equal_pos;
	char	*add_equal;
}			t_set_env;

typedef struct s_fill_node
{
	char	**temp[2];
	int		stop;
}			t_fill_node;

typedef struct s_unset
{
	char	**argv;
	char	*temp;
	int 	i;
	int		j;
}	t_unset;

enum		e_mini_error
{
	QUOTE = 1,
	NDIR = 2,
	NPERM = 3,
	NCMD = 6,
	DUPERR = 7,
	FORKERR = 8,
	PIPERR = 9,
	PIPENDERR = 10,
	MEM = 11,
	IS_DIR = 12,
	NOT_DIR = 13
};

// what the hell
char		*ft_strndup(const char *s, int n);
char		*ft_strncpy(char *dest, const char *src, int n);
char		*ft_strcat(char *dest, const char *src);

char		*expand_variables(t_prompt *prom, char *line);

char		**allocate_and_dup_args(char **args, t_prompt *prom);
void		trim_args(char **temp, char **args, t_prompt *prom);
t_node_content		*mini_init(void);
t_node_content		*get_params(t_node_content *node, char **str[2], int *i, t_prompt *prom);

int			mini_pwd(void);
int			mini_echo(t_list *cmd);
int			handle_cd(t_prompt *prompt, t_list *cmd, char **args);
t_list		*stop_make_node(t_list *cmds, char **args, char **temp);

void		check_command_path(t_list *cmd, char ***s, char *path,
				t_prompt *prom);
DIR			*check_directory(t_list *cmd, char ***s, char *path,
				t_prompt *prom);

int			found_var(char *argv, char **envp, int *i);
int			is_numeric(char *str);
void		fail_allocate(t_prompt *prom, int flag);
int			ft_strchr_index(const char *str, int c);
char		**ft_matrix_replace_in(char ***big, char **small, int n);
int			ft_putmatrix_fd(char **m, int nl, int fd);
char		**ft_dup_matrix(char **m);
char		**ft_extend_matrix(char **in, char *newstr);
int			ft_matrixlen(char **m);
void		ft_free_matrix(char ***m);
int			ft_countchar(char *s, char c);
int			handle_built_in(t_prompt *prompt, t_list *cmd, int *is_exit, char **args);
int			check_if_built_in(t_node_content *n);
void		check_cd(char **str[2], t_prompt *prom);
int			handle_export(t_prompt *prompt, t_list *cmd, char **args);
int			handle_unset(t_prompt *prompt, t_list *cmd, char **args);
int			handle_exit(t_list *cmd, int *is_exit, t_prompt *prom);
void		*check_args(char *out, t_prompt *p);
char		**split_quote_space(char *str, char *set, t_prompt *p);
char		**split_separator(char const *s, char *set);
char		*delete_qoutes(char const *s1, int squote, int dquote);
t_list		*make_node(char **args, t_prompt *p);
// int		get_fd(int oldfd, char *path, int flags[2]);
t_node_content		*get_outfile1(t_node_content *node, char **args, int *i, t_prompt *prom);
t_node_content		*out_redirction_double(t_node_content *node, char **args, int *i, t_prompt *prom);
t_node_content		*get_infile1(t_node_content *node, char **args, int *i, t_prompt *prom);
t_node_content		*get_infile2(t_node_content *node, char **args, int *i, t_prompt *prom);
void		execute_command(t_prompt *prompt, t_list *cmd, char **args);
int			check_to_execute(t_prompt *prompt, t_list *cmd, int fd[2], char **args);
void		get_cmd(t_prompt *prompt, t_list *start, char **split_path,
				char *path);
char		*expand_path(char *str, int i, int quotes[2], char *var);
int			get_here_doc(char *str[2], char *aux[2], t_prompt *prom);
void		*mini_perror(int err_type, char *param, int err, t_prompt *prom);
char		*get_env_var(char *var, char **envp, int n, t_prompt *prom);
char		**set_env_var(char *var, char *value, t_prompt *prom, int n);
// char	*mini_getprompt(t_prompt prompt);
void		free_content(void *content);
void		handle_sigint(int sig);
void		hanlde_make_node(t_list **cmds, char **args, t_prompt *prom, char ***temp);
void		make_node_util(t_list **cmds, t_fill_node *fill, int *i);
void		make_node_util_2(char **args, t_fill_node *fill, t_list **cmds, t_prompt *prom);
void		init_make_node(t_list **cmds, t_fill_node *fill, char **args, t_prompt *prom);

#endif
