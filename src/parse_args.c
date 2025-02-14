/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salam <salam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 12:08:12 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/14 17:27:20 by salam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void handle_fail(t_prompt *prom, char **args, char **str)
{
	ft_free_matrix(&prom->envp);
	if (args && args[0])
		ft_free_matrix(&args);
	if (str && str[0])
		ft_free_matrix(&str);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

static char	**expand(char **args, t_prompt *prom)
// remove and handle single double quotes and $ ~.
{
	char	**str;
	int		i;
	int		quotes[2];

	i = -1;
	while (args && args[++i])
	{
		args[i] = expand_vars(args[i], -1, quotes, prom, args);
		args[i] = expand_path(args[i], -1, quotes,
			mini_getenv("HOME", prom->envp, 4));
		if (!args[i])
			handle_fail(prom, args, str);
		str = ft_cmdsubsplit(args[i], "<|>");
		if (!str)
			handle_fail(prom, args, NULL);
		if (!ft_matrix_replace_in(&args, str, i))
			handle_fail(prom, args, str);
		i = i + ft_matrixlen(str) - 1;
		ft_free_matrix(&str);
	}
	return (args);
}

static void	*parse_args(char **args, t_prompt *prom)
// waitpid(-1, NULL, NULL) : to wait any chiled process.
{
	int	is_exit;
	int	i;

	is_exit = 0;
	prom->cmds = fill_nodes(expand(args, prom), -1, prom);
	if (!prom->cmds)
		return (prom);
	i = ft_lstsize(prom->cmds);
	g_status = builtin(prom, prom->cmds, &is_exit, args);
	while (i-- > 0)
		waitpid(-1, &g_status, 0);
	if (!is_exit && g_status == 13)
		g_status = 0;
	if (g_status > 255)
		g_status = g_status / 255;
	if (args && is_exit)
	{
		ft_lstclear(&prom->cmds, free_content);
		return (NULL);
	}
	return (prom);
}

int	is_numeric(char *str)
{
	int i = 0;

	if (!str || !str[0])
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	*check_args(char *read, t_prompt *prom)
// all solution.
{
	char	**str;
	t_mini	*node;

	if (!read)
	{
		printf("exit\n");
		return (NULL);
	}
	if (read[0] != '\0')
		add_history(read);
	str = handle_quote(read, " ", prom);
	if (read)
		free(read);
	if (!str)
		mini_perror(QUOTE, NULL, 1);
	if (!str)
		return ("");
	prom = parse_args(str, prom);
	if (prom && prom->cmds)
		node = prom->cmds->content;
	if (prom && prom->cmds && node && node->full_cmd && ft_lstsize(prom->cmds) == 1)
		prom->envp = mini_setenv("_", node->full_cmd[ft_matrixlen(node->full_cmd) - 1],
			prom->envp, 1);
	if (prom && prom->cmds)
		ft_lstclear(&prom->cmds, free_content);
	return (prom);
}
