/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:22:54 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/15 19:07:19 by sbibers          ###   ########.fr       */
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

# define READ_END 0
# define WRITE_END 1

typedef struct s_expand_read
{
	int		in_double_quote;
	int		in_single_quote;
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
}			t_expand_read_2;

typedef struct s_prompt
{
	t_list	*cmds;
	char	**envp;
	pid_t	pid;
}			t_prompt;

typedef struct s_mini
{
	char	**full_cmd;
	char	*full_path;
	int		infile;
	int		outfile;
}			t_mini;

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

// get_next_line_utils.c
size_t		gnl_strlen(const char *s);
char		*gnl_substr(char const *s, unsigned int start, size_t len);
size_t		gnl_strlcpy(char *dst, const char *src, size_t size);
int			gnl_strchr_i(const char *s, int c);
size_t		gnl_strlcat(char *dst, const char *src, size_t size);

// expand_var.c
char		*expand_variables(t_prompt *prom, char *line);

char		**allocate_and_dup_args(char **args, t_prompt *prom);
void		trim_args(char **temp, char **args, t_prompt *prom);
t_mini		*mini_init(void);
t_mini		*get_params(t_mini *node, char **str[2], int *i);

int			mini_pwd(void);
int			mini_echo(t_list *cmd);
int			mini_cd(t_prompt *prompt, t_list *cmd, char **args);
t_list		*stop_fill(t_list *cmds, char **args, char **temp);

void		check_command_path(t_list *cmd, char ***s, char *path,
				t_prompt *prom);
DIR			*check_directory(t_list *cmd, char ***s, char *path,
				t_prompt *prom);

int			var_in_envp(char *argv, char **envp, int ij[2]);
int			is_numeric(char *str);
void		fail_allocate(t_prompt *prom, int flag);
int			ft_strchr_i(const char *s, int c);
char		**ft_matrix_replace_in(char ***big, char **small, int n);
int			ft_isspace(char c);
int			ft_strchars_i(const char *s, char *set);
int			ft_putmatrix_fd(char **m, int nl, int fd);
char		**ft_dup_matrix(char **m);
char		**ft_extend_matrix(char **in, char *newstr);
int			ft_matrixlen(char **m);
void		ft_free_matrix(char ***m);
int			ft_countchar(char *s, char c);
int			builtin(t_prompt *prompt, t_list *cmd, int *is_exit, char **args);
int			is_builtin(t_mini *n);
void		cd_error(char **str[2]);
int			mini_export(t_prompt *prompt, t_list *cmd, char **args);
int			mini_unset(t_prompt *prompt, t_list *cmd, char **args);
int			mini_exit(t_list *cmd, int *is_exit);
void		*check_args(char *out, t_prompt *p);
char		**handle_quote(char *str, char *set, t_prompt *p);
char		**ft_cmdsubsplit(char const *s, char *set);
char		*ft_strtrim_all(char const *s1, int squote, int dquote);
t_list		*fill_nodes(char **args, int i, t_prompt *p);
// int		get_fd(int oldfd, char *path, int flags[2]);
t_mini		*get_outfile1(t_mini *node, char **args, int *i);
t_mini		*get_outfile2(t_mini *node, char **args, int *i);
t_mini		*get_infile1(t_mini *node, char **args, int *i);
t_mini		*get_infile2(t_mini *node, char **args, int *i);
void		*exec_cmd(t_prompt *prompt, t_list *cmd);
void		*check_to_fork(t_prompt *prompt, t_list *cmd, int fd[2]);
void		child_builtin(t_prompt *prompt, t_mini *n, int l, t_list *cmd);
void		exec_custom(char ***out, char *full, char *args, char **envp);
int			exec_builtin(t_prompt *prompt, int (*func)(t_prompt *));
void		get_cmd(t_prompt *prompt, t_list *start, char **split_path,
				char *path);
char		*expand_path(char *str, int i, int quotes[2], char *var);
int			get_here_doc(char *str[2], char *aux[2]);
void		*mini_perror(int err_type, char *param, int err);
char		*mini_getenv(char *var, char **envp, int n);
char		**mini_setenv(char *var, char *value, char **envp, int n);
// char	*mini_getprompt(t_prompt prompt);
void		free_content(void *content);
void		handle_sigint(int sig);
void		handle_sigint_child(int sig);

#endif
