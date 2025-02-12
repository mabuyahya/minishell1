/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 17:05:01 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/12 13:11:11 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_mini	*mini_init(void)
{
	t_mini	*mini;

	mini = malloc(sizeof(t_mini));
	if (!mini)
		return (NULL);
	mini->full_cmd = NULL;
	mini->full_path = NULL;
	mini->infile = STDIN_FILENO;
	mini->outfile = STDOUT_FILENO;
	return (mini);
}

static t_mini	*get_params(t_mini *node, char **a[2], int *i)
// put infile and outfile for every command.
// put the command in the content of the node.
// a[0] : with single and double quotes.
// a[1] = without single and double quotes.
{
	if (a[0][*i])
	{
		if (a[0][*i][0] == '>' && a[0][*i + 1] && a[0][*i + 1][0] == '>')
			node = get_outfile2(node, a[1], i);
		else if (a[0][*i][0] == '>')
			node = get_outfile1(node, a[1], i);
		else if (a[0][*i][0] == '<' && a[0][*i + 1] && \
			a[0][*i + 1][0] == '<')
			node = get_infile2(node, a[1], i);
		else if (a[0][*i][0] == '<')
			node = get_infile1(node, a[1], i);
		else if (a[0][*i][0] != '|')
			node->full_cmd = ft_extend_matrix(node->full_cmd, a[1][*i]);
		else
		{
			mini_perror(PIPENDERR, NULL, 2);
			*i = -2;
		}
		return (node);
	}
	mini_perror(PIPENDERR, NULL, 2);
	*i = -2;
	return (node);
}

static char	**get_trimmed(char **args, t_prompt *p)
// delete single and double quote.
{
	char	**temp;
	char	*aux;
	int		j;

	j = -1;
	temp = ft_dup_matrix(args);
	if (!temp)
	{
		ft_free_matrix(&args);
		ft_free_matrix(&p->envp);
		mini_perror(MEM, NULL, 1);
		exit(1);
	}
	while (temp && temp[++j])
	{
		aux = ft_strtrim_all(temp[j], 0, 0);
		if (!aux)
		{
			ft_free_matrix(&temp);
			ft_free_matrix(&args);
			ft_free_matrix(&p->envp);
			mini_perror(MEM, NULL, 1);
			exit(1);
		}
		free(temp[j]);
		temp[j] = aux;
	}
	return (temp);
}

static t_list	*stop_fill(t_list *cmds, char **args, char **temp)
{
	ft_lstclear(&cmds, free_content);
	ft_free_matrix(&temp);
	ft_free_matrix(&args);
	return (NULL);
}

t_list	*fill_nodes(char **args, int i, t_prompt *p)
 // cmd[0] = pointer to the first | cmd[1] = pointer to the last.
 // fill node, put the command inside the content of the node.
 // every command have the input and output and the path of the command. 
{
	t_list	*cmds[2];
	char	**temp[2];

	cmds[0] = NULL;
	temp[1] = get_trimmed(args, p);
	while (args[++i])
	{
		cmds[1] = ft_lstlast(cmds[0]);
		if (i == 0 || (args[i][0] == '|' && args[i + 1] && args[i + 1][0]))
		{
			if (args[i][0] == '|')
				i++;
			ft_lstadd_back(&cmds[0], ft_lstnew(mini_init()));
			cmds[1] = ft_lstlast(cmds[0]);
		}
		temp[0] = args;
		cmds[1]->content = get_params(cmds[1]->content, temp, &i);
		if (!cmds[1]->content)
		{
			ft_free_matrix(&temp[1]);
			ft_free_matrix(&temp[0]);
			ft_free_matrix(&args);
			stop_fill(cmds[0], NULL, NULL);
			stop_fill(cmds[1], NULL, NULL);
			ft_free_matrix(&p->envp);
			mini_perror(MEM, NULL, 1);
			exit(1);
		}
		if (i < 0)
			return (stop_fill(cmds[0], args, temp[1]));
		if (!args[i])
			break ;
	}
	ft_free_matrix(&temp[1]);
	ft_free_matrix(&args);
	return (cmds[0]);
}
