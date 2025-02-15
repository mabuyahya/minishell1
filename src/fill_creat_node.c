/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_creat_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:09:14 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/15 18:20:33 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**get_trimmed(char **args, t_prompt *prom)
// delete ' and " from the input.
{
	char	**temp;

	temp = allocate_and_dup_args(args, prom);
	trim_args(temp, args, prom);
	return (temp);
}

static void	fail_allocate_node(char ***temp, char **args, t_prompt *prom,
		t_list **cmds)
{
	ft_free_matrix(&temp[1]);
	ft_free_matrix(&temp[0]);
	ft_free_matrix(&args);
	stop_fill(cmds[0], NULL, NULL);
	stop_fill(cmds[1], NULL, NULL);
	ft_free_matrix(&prom->envp);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

static void	creat_node(char **args, int *i, t_list **cmds)
{
	if (args[*i][0] == '|')
		(*i)++;
	ft_lstadd_back(&cmds[0], ft_lstnew(mini_init()));
	cmds[1] = ft_lstlast(cmds[0]);
}

// cmd[0] = pointer to the first | cmd[1] = pointer to the last.
// fill node, put the command inside the content of the node.
// every command have the input and output and the path of the command.
t_list	*fill_nodes(char **args, int i, t_prompt *prom)
{
	t_list	*cmds[2];
	char	**temp[2];

	cmds[0] = NULL;
	temp[1] = get_trimmed(args, prom);
	while (args[++i])
	{
		cmds[1] = ft_lstlast(cmds[0]);
		if (i == 0 || (args[i][0] == '|' && args[i + 1] && args[i + 1][0]))
			creat_node(args, &i, cmds);
		temp[0] = args;
		cmds[1]->content = get_params(cmds[1]->content, temp, &i);
		if (!cmds[1]->content)
			fail_allocate_node(temp, args, prom, cmds);
		if (i < 0)
			return (stop_fill(cmds[0], args, temp[1]));
		if (!args[i])
			break ;
	}
	ft_free_matrix(&temp[1]);
	ft_free_matrix(&args);
	return (cmds[0]);
}
