/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salam <salam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/03 17:05:01 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/14 12:54:36 by salam            ###   ########.fr       */
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

static t_mini	*get_params(t_mini *node, char **str[2], int *i)
// put infile and outfile for every command.
// put the command in the content of the node.
// str[0] : with single and double quotes.
// str[1] = without single and double quotes.
{
	if (str[0][*i])
	{
		if (str[0][*i][0] == '>' && str[0][*i + 1] && str[0][*i + 1][0] == '>')
			node = get_outfile2(node, str[1], i);
		else if (str[0][*i][0] == '>')
			node = get_outfile1(node, str[1], i);
		else if (str[0][*i][0] == '<' && str[0][*i + 1]
			&& str[0][*i + 1][0] == '<')
			node = get_infile2(node, str[1], i);
		else if (str[0][*i][0] == '<')
			node = get_infile1(node, str[1], i);
		else if (str[0][*i][0] != '|')
			node->full_cmd = ft_extend_matrix(node->full_cmd, str[1][*i]);
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

static char	**allocate_and_dup_args(char **args, t_prompt *prom)
{
	char	**temp;

	temp = ft_dup_matrix(args);
	if (!temp)
	{
		ft_free_matrix(&args);
		ft_free_matrix(&prom->envp);
		mini_perror(MEM, NULL, 1);
		exit(1);
	}
	return (temp);
}

static void	trim_args(char **temp, char **args, t_prompt *prom)
{
	char	*aux;
	int		j;

	j = -1;
	while (temp && temp[++j])
	{
		aux = ft_strtrim_all(temp[j], 0, 0);
		if (!aux)
		{
			ft_free_matrix(&temp);
			ft_free_matrix(&args);
			ft_free_matrix(&prom->envp);
			mini_perror(MEM, NULL, 1);
			exit(1);
		}
		free(temp[j]);
		temp[j] = aux;
	}
}

static char	**get_trimmed(char **args, t_prompt *prom)
// delete ' and " from the input.
{
	char	**temp;

	temp = allocate_and_dup_args(args, prom);
	trim_args(temp, args, prom);
	return (temp);
}

static t_list	*stop_fill(t_list *cmds, char **args, char **temp)
{
	ft_lstclear(&cmds, free_content);
	ft_free_matrix(&temp);
	ft_free_matrix(&args);
	return (NULL);
}

static void fail_allocate_node(char ***temp, char **args, t_prompt *prom, t_list **cmds)
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

static void creat_node(char **args, int *i, t_list **cmds)
{
	if (args[*i][0] == '|')
		(*i)++;
	ft_lstadd_back(&cmds[0], ft_lstnew(mini_init()));
	cmds[1] = ft_lstlast(cmds[0]);
}

t_list	*fill_nodes(char **args, int i, t_prompt *prom)
// cmd[0] = pointer to the first | cmd[1] = pointer to the last.
// fill node, put the command inside the content of the node.
// every command have the input and output and the path of the command. 
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
