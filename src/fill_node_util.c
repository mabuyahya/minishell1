/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node_util.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 17:05:01 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/19 10:45:13 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node_content	*mini_init(void)
{
	t_node_content	*mini;

	mini = malloc(sizeof(t_node_content));
	if (!mini)
		return (NULL);
	mini->full_cmd = NULL;
	mini->full_path = NULL;
	mini->infile = STDIN_FILENO;
	mini->outfile = STDOUT_FILENO;
	return (mini);
}

// put infile and outfile for every command.
// put the command in the content of the node.
// str[0] : with single and double quotes.
// str[1] = without single and double quotes.
t_node_content	*get_params(t_node_content *node, char **str[2], int *i, t_prompt *prom)
{
	if (str[0][*i])
	{
		if (str[0][*i][0] == '>' && str[0][*i + 1] && str[0][*i + 1][0] == '>')
			node = out_redirction_double(node, str[1], i, prom);
		else if (str[0][*i][0] == '>')
			node = get_outfile1(node, str[1], i, prom);
		else if (str[0][*i][0] == '<' && str[0][*i + 1] && str[0][*i
				+ 1][0] == '<')
			node = get_infile2(node, str[1], i, prom);
		else if (str[0][*i][0] == '<')
			node = get_infile1(node, str[1], i, prom);
		else if (str[0][*i][0] != '|')
			node->full_cmd = ft_extend_matrix(node->full_cmd, str[1][*i]);
		else
		{
			mini_perror(PIPENDERR, NULL, 2, prom);
			*i = -2;
		}
		return (node);
	}
	mini_perror(PIPENDERR, NULL, 2, prom);
	*i = -2;
	return (node);
}

char	**allocate_and_dup_args(char **args, t_prompt *prom)
{
	char	**temp;

	temp = ft_dup_matrix(args);
	if (!temp)
	{
		ft_free_matrix(&args);
		ft_free_matrix(&prom->envp);
		mini_perror(MEM, NULL, 1, prom);
		exit(1);
	}
	return (temp);
}

void	trim_args(char **temp, char **args, t_prompt *prom)
{
	char	*str;
	int		j;

	j = -1;
	while (temp && temp[++j])
	{
		str = delete_qoutes(temp[j], 0, 0);
		if (!str)
		{
			ft_free_matrix(&temp);
			ft_free_matrix(&args);
			ft_free_matrix(&prom->envp);
			mini_perror(MEM, NULL, 1, prom);
			exit(1);
		}
		free(temp[j]);
		temp[j] = str;
	}
}
