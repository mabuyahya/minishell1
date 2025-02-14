/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salam <salam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:07 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/14 17:04:41 by salam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void not_builtin(t_prompt *prom, t_list *cmd)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	exec_cmd(prom, cmd);
}

int	builtin(t_prompt *prom, t_list *cmd, int *exit_num, char **args)
// execute command, and check if the command built in.
{
	char	**str;
	int 	n;

	while (cmd)
	{
		str = ((t_mini *)cmd->content)->full_cmd;
		n = 0;
		if (str)
			n = ft_strlen(*str);
		if (str && !ft_strncmp(*str, "exit", n) && n == 4)
			g_status = mini_exit(cmd, exit_num);
		else if (!cmd->next && str && !ft_strncmp(*str, "cd", n) && n == 2)
			g_status = mini_cd(prom, cmd, args);
		else if (!cmd->next && str && !ft_strncmp(*str, "export", n) && n == 6)
			g_status = mini_export(prom, cmd, args);
		else if (!cmd->next && str && !ft_strncmp(*str, "unset", n) && n == 5)
			g_status = mini_unset(prom, cmd, args);
		else
			not_builtin(prom, cmd);
		cmd = cmd->next;
	}
	return (g_status);
}

int	is_builtin(t_mini *n)
// check if the command built in or not.
{
	int		l;

	if (!n->full_cmd)
		return (0);
	if ((n->full_cmd && ft_strchr(*n->full_cmd, '/')) || (n->full_path && \
		ft_strchr(n->full_path, '/')))
		return (0);
	l = ft_strlen(*n->full_cmd);
	if (!ft_strncmp(*n->full_cmd, "pwd", l) && l == 3)
		return (1);
	if (!ft_strncmp(*n->full_cmd, "env", l) && l == 3)
		return (1);
	if (!ft_strncmp(*n->full_cmd, "cd", l) && l == 2)
		return (1);
	if (!ft_strncmp(*n->full_cmd, "export", l) && l == 6)
		return (1);
	if (!ft_strncmp(*n->full_cmd, "unset", l) && l == 5)
		return (1);
	if (!ft_strncmp(*n->full_cmd, "echo", l) && l == 4)
		return (1);
	if (!ft_strncmp(*n->full_cmd, "exit", l) && l == 4)
		return (1);
	return (0);
}

static t_list	*stop_fill(t_list *cmds, char **args, char **temp)
{
	ft_lstclear(&cmds, free_content);
	ft_free_matrix(&temp);
	ft_free_matrix(&args);
	return (NULL);
}

static void		stop_solution(t_list *cmd, t_prompt *prom, char **args, char ***str)
{
	ft_free_matrix(&str[0]);
	stop_fill(cmd, args, prom->envp);
	mini_perror(MEM, NULL, 1);
	exit(1);	
}

static void	handle_cd_paths(t_list *cmd, t_prompt *p, char **args, char **str[2])
{
	char	*aux;

	aux = getcwd(NULL, 0);
	str[1] = ft_extend_matrix(str[1], aux);
	free(aux);
	if (!str[1])
		stop_solution(cmd, p, args, str);
	cd_error(str);
	if (!g_status)
		p->envp = mini_setenv("OLDPWD", str[1][1], p->envp, 6);
	aux = getcwd(NULL, 0);
	if (!aux)
	{
		aux = ft_strdup("");
		if (!aux)
			stop_solution(cmd, p, args, str);
	}
	str[1] = ft_extend_matrix(str[1], aux);
	free(aux);
	if (!str[1])
		stop_solution(cmd, p, args, str);
	p->envp = mini_setenv("PWD", str[1][2], p->envp, 3);
}

int	mini_cd(t_prompt *prom, t_list *cmd, char **args)
// handle command cd.
{
	char	**str[2];
	char	*aux;

	g_status = 0;
	str[0] = ((t_mini *)prom->cmds->content)->full_cmd;
	aux = mini_getenv("HOME", prom->envp, 4);
	if (!aux)
	{
		aux = ft_strdup("");
		if (!aux)
			stop_solution(cmd, prom, args, str);
	}
	str[1] = ft_extend_matrix(NULL, aux);
	free(aux);
	if (!str[1])
		stop_solution(cmd, prom, args, str);
	handle_cd_paths(cmd, prom, args, str);
	ft_free_matrix(&str[1]);
	return (g_status);
}

int	mini_pwd(void)
// handle pwd.
{
	char	*buf;

	buf = getcwd(NULL, 0);
	ft_putendl_fd(buf, 1);
	free(buf);
	return (0);
}

int	mini_echo(t_list *cmd)
// handle echo.
{
	int		newline;
	int		i[2];
	char	**argv;
	t_mini	*node;

	i[0] = 0;
	i[1] = 0;
	newline = 1;
	node = cmd->content;
	argv = node->full_cmd;
	while (argv && argv[++i[0]])
	{
		if (!i[1] && !ft_strncmp(argv[i[0]], "-n", 2) && \
			(ft_countchar(argv[i[0]], 'n') == \
			(int)(ft_strlen(argv[i[0]]) - 1)))
			newline = 0;
		else
		{
			i[1] = 1;
			ft_putstr_fd(argv[i[0]], 1);
			if (argv[i[0] + 1])
				ft_putchar_fd(' ', 1);
		}
	}
	return (write(1, "\n", newline) == 2);
}
