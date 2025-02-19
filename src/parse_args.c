/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 12:08:12 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/19 13:22:20 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_e_status;

static void	handle_fail_expand(t_prompt *prom, char **args, char **str)
{
	ft_free_matrix(&prom->envp);
	if (args && args[0])
		ft_free_matrix(&args);
	if (str && str[0])
		ft_free_matrix(&str);
	mini_perror(MEM, NULL, 1, prom);
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
		args[i] = expand_path(args[i], -1, quotes, get_env_var("HOME",
					prom->envp, 4, prom));
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

static void	*parse_args(char **args, t_prompt *prom)
{
	int		is_exit;
	char	**temp;

	is_exit = 0;
	temp = expand(args, prom);
	prom->cmds = make_node(temp, prom);
	if (!prom->cmds)
		return (prom);
	prom->size = ft_lstsize(prom->cmds);
	prom->exit_status = handle_built_in(prom, prom->cmds, &is_exit, args);
	while (wait(NULL) != -1)
		;
	if (args && is_exit)
	{
		ft_lstclear(&prom->cmds, free_content);
		return (NULL);
	}
	return (prom);
}

void	check_args_util(t_prompt *prom, t_node_content *node)
{
	if (prom && prom->cmds)
		node = prom->cmds->content;
	if (prom && prom->cmds && node && node->full_cmd
		&& ft_lstsize(prom->cmds) == 1)
		prom->envp = set_env_var("_",
				node->full_cmd[ft_matrixlen(node->full_cmd) - 1], prom,
				1);
	if (prom && prom->cmds)
		ft_lstclear(&prom->cmds, free_content);
}

// all solution.
void	*check_args(char *read, t_prompt *prom)
{
	char	**str;
	t_node_content	*node;

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
		mini_perror(QUOTE, NULL, 1, prom);
		return ("");
	}
	prom = parse_args(str, prom);
	check_args_util(prom, node);
	return (prom);
}
