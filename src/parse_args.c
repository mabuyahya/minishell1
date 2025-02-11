/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 12:08:12 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/11 14:07:10 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static char	**split_all(char **args, t_prompt *prompt)
// remove and handle single double quotes and $ ~.
{
	char	**subsplit;
	int		i;
	int		quotes[2];

	i = -1;
	while (args && args[++i])
	{
		args[i] = expand_vars(args[i], -1, quotes, prompt);
		args[i] = expand_path(args[i], -1, quotes,
			mini_getenv("HOME", prompt->envp, 4));
		if (!args[i])
		{
			ft_free_matrix(&args);
			ft_free_matrix(&prompt->envp);
			if (subsplit != NULL)
				ft_free_matrix(&prompt->envp);
			mini_perror(MEM, ".", 1);
			exit(1);
		}
		subsplit = ft_cmdsubsplit(args[i], "<|>");
		if (!subsplit)
		{
			ft_free_matrix(&args);
			ft_free_matrix(&prompt->envp);
			mini_perror(MEM, ".", 1);
			exit(1);
		}
		if (!ft_matrix_replace_in(&args, subsplit, i))
		{
			ft_free_matrix(&args);
			ft_free_matrix(&prompt->envp);
			ft_free_matrix(&subsplit);
			mini_perror(MEM, ".", 1);
			exit(1);
		}
		i = i + ft_matrixlen(subsplit) - 1;
		ft_free_matrix(&subsplit);
	}
	return (args);
}

////////////////////////////////////////////////////
static void	*parse_args(char **args, t_prompt *p)
// waitpid(-1, NULL, NULL) : to wait any chiled process.
{
	int	is_exit;
	int	i;

	is_exit = 0;
	p->cmds = fill_nodes(split_all(args, p), -1);
	if (!p->cmds)
		return (p);
	i = ft_lstsize(p->cmds);
	g_status = builtin(p, p->cmds, &is_exit, 0);
	while (i-- > 0)
		waitpid(-1, &g_status, 0);
	if (!is_exit && g_status == 13)
		g_status = 0;
	if (g_status > 255)
		g_status = g_status / 255;
	if (args && is_exit)
	{
		ft_lstclear(&p->cmds, free_content);
		return (NULL);
	}
	return (p);
}

void	*check_args(char *out, t_prompt *p)
// all solution.
{
	char	**a;
	t_mini	*n;

	if (!out)
	{
		printf("exit\n");
		return (NULL);
	}
	if (out[0] != '\0')
		add_history(out);
	a = ft_cmdtrim(out, " ", p);
	free(out);
	if (!a)
		mini_perror(QUOTE, NULL, 1);
	if (!a)
		return ("");
	p = parse_args(a, p);
	if (p && p->cmds)
		n = p->cmds->content;
	if (p && p->cmds && n && n->full_cmd && ft_lstsize(p->cmds) == 1)
		p->envp = mini_setenv("_", n->full_cmd[ft_matrixlen(n->full_cmd) - 1],
			p->envp, 1);
	if (p && p->cmds)
		ft_lstclear(&p->cmds, free_content);
	return (p);
}
