/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 15:08:07 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/15 17:21:16 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int		g_status;

static void	not_builtin(t_prompt *prom, t_list *cmd)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	exec_cmd(prom, cmd);
}

int	builtin(t_prompt *prom, t_list *cmd, int *exit_num, char **args)
// execute command, and check if the command built in.
{
	char **str;
	int n;

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
	int l;

	if (!n->full_cmd)
		return (0);
	if ((n->full_cmd && ft_strchr(*n->full_cmd, '/')) || (n->full_path
			&& ft_strchr(n->full_path, '/')))
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
