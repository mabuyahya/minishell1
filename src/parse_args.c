/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 12:08:12 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/17 16:34:32 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void	handle_fail_expand(t_prompt *prom, char **args, char **str)
{
	ft_free_matrix(&prom->envp);
	if (args && args[0])
		ft_free_matrix(&args);
	if (str && str[0])
		ft_free_matrix(&str);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

// remove and handle single double quotes and ~.
static char	**expand(char **args, t_prompt *prom)
{
	char	**str;
	int		i;
	int		quotes[2];

	i = -1;
	while (args && args[++i])
	{
		args[i] = expand_path(args[i], -1, quotes, mini_getenv("HOME",
					prom->envp, 4));
		if (!args[i])
			handle_fail_expand(prom, args, str);
		str = split_separator(args[i], "<|>");
		if (!str)
			handle_fail_expand(prom, args, NULL);
		if (!ft_matrix_replace_in(&args, str, i))
			handle_fail_expand(prom, args, str);
		i = i + ft_matrixlen(str) - 1;
		ft_free_matrix(&str);
	}
	return (args);
}

// waitpid(-1, NULL, NULL) : to wait any chiled process.
static void	*parse_args(char **args, t_prompt *prom)
{
	int		is_exit;
	int		i;
	char	**temp;

	is_exit = 0;
	temp = expand(args, prom);
	prom->cmds = fill_nodes(temp, -1, prom);
	if (!prom->cmds)
		return (prom);
	i = ft_lstsize(prom->cmds);
	prom->size = ft_lstsize(prom->cmds);
	g_status = builtin(prom, prom->cmds, &is_exit, args);
	while (i-- > 0)
		wait(NULL);
	if (args && is_exit)
	{
		ft_lstclear(&prom->cmds, free_content);
		return (NULL);
	}
	return (prom);
}

void	check_args_util(t_prompt *prom, t_mini *node)
{
	if (prom && prom->cmds)
		node = prom->cmds->content;
	if (prom && prom->cmds && node && node->full_cmd
		&& ft_lstsize(prom->cmds) == 1)
		prom->envp = mini_setenv("_",
				node->full_cmd[ft_matrixlen(node->full_cmd) - 1], prom->envp,
				1);
	if (prom && prom->cmds)
		ft_lstclear(&prom->cmds, free_content);
}

// all solution.
void	*check_args(char *read, t_prompt *prom)
{
	char	**str;
	t_mini	*node;

	node = NULL;
	if (!read)
	{
		printf("exit\n");
		return (NULL);
	}
	if (read[0] != '\0')
		add_history(read);
	read = expand_variables(prom, read);
	str = split_quote_space(read, " ", prom);
	free(read);
	if (!str)
	{
		mini_perror(QUOTE, NULL, 1);
		return ("");
	}
	prom = parse_args(str, prom);
	check_args_util(prom, node);
	return (prom);
}
