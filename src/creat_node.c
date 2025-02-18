/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_node.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:09:14 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/18 20:52:13 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	fail_make_node(char ***temp, char **args, t_prompt *prom,
		t_list **cmds)
{
	ft_free_matrix(temp);
	stop_make_node(cmds[0], prom->envp, args);
	stop_make_node(cmds[1], NULL, NULL);
	mini_perror(MEM, NULL, 1, prom);
	exit(1);
}

static int	creat_node(char **args, int *i, t_list **cmds)
{
	t_node_content *mini;
	t_list *lst;

	if (args[*i][0] == '|' && (*i != 0))
		(*i)++;
	mini = mini_init();
	if (!mini)
		return (0);
	lst = ft_lstnew(mini);
	if (!lst)
		return (0);
	ft_lstadd_back(&cmds[0], lst);
	cmds[1] = ft_lstlast(cmds[0]);
	return (1);
}

// temp[1] = all commands without single and double quotes.
// temp[0] = all commands with single and double quotes.
// cmd[0] = pointer to the first | cmd[1] = pointer to the last.
// fill node, put the command inside the content of the node.
// every command have the input and output and the path of the command.
t_list	*make_node(char **args, int i, t_prompt *prom)
{
	t_list		*cmds[2];
	t_fill_node	fill;
	
	init_make_node(cmds, &fill, args, prom);
	while (args[++i])
	{
		make_node_util(cmds, &fill, &i);
		if (i == 0 || (args[i][0] == '|' && args[i + 1] && args[i + 1][0]))
		{
			if (!creat_node(args, &i, cmds))
				hanlde_make_node(cmds, args, prom, fill.temp);
		}
		make_node_util_2(args, &fill, cmds, &i, prom);
		if (!cmds[1]->content)
			fail_make_node(fill.temp, args, prom, cmds);
		if (i < 0)
			return (stop_make_node(cmds[0], args, fill.temp[1]));
		if (!args[i])
			break ;
	}
	stop_make_node(NULL, fill.temp[1], args);
	return (cmds[0]);
}
