/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   creat_node_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 16:05:16 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/19 10:46:12 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	**trim(char **args, t_prompt *prom)
// delete ' and " from the input.
{
	char	**temp;

	temp = allocate_and_dup_args(args, prom);
	trim_args(temp, args, prom);
	return (temp);
}

void	hanlde_make_node(t_list **cmds, char **args, t_prompt *prom, char ***temp)
{
	ft_free_matrix(temp);
	stop_make_node(cmds[0], args, prom->envp);
	stop_make_node(cmds[1], NULL, NULL);
	mini_perror(MEM, NULL, 1, prom);
	exit(1);
}

void	make_node_util(t_list **cmds, t_fill_node *fill, int *i)
{
	fill->stop = *i;
	cmds[1] = ft_lstlast(cmds[0]);
}

void	make_node_util_2(char **args, t_fill_node *fill, t_list **cmds, t_prompt *prom)
{
	fill->temp[0] = args;
	cmds[1]->content = get_params(cmds[1]->content, fill->temp, &prom->count_make_node, prom);
}

void	init_make_node(t_list **cmds, t_fill_node *fill, char **args, t_prompt *prom)
{
	cmds[0] = NULL;
	fill->temp[1] = trim(args, prom);
}
