/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmdtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:28:36 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/11 18:07:04 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

#include <stdlib.h>
#include <stdio.h>

static int	ft_count_words(const char *s, char *delims)
{
	int		count;
	int		i;
	char	quote;

	i = 0;
	count = 0;
	quote = 0;
	while (s[i])
	{
		while (s[i] && ft_strchr(delims, s[i]))
			i++; // Skip delimiters
		if (s[i])
			count++;
		while (s[i] && (!ft_strchr(delims, s[i]) || quote))
		{
			if (!quote && (s[i] == '\'' || s[i] == '\"'))
				quote = s[i];
			else if (quote && s[i] == quote)
				quote = 0;
			i++;
		}
	}
	if (quote)
	    return (-1);
	return count;
}

static char	**ft_fill_array(char **arr, char *s, char *delims, t_prompt *p)
{
	int		i;
	int		start;
	int		word;
	char	quote = 0;

	i = 0;
	word = 0;
	while (s[i])
	{
		while (s[i] && ft_strchr(delims, s[i]))
			i++; // Skip delimiters
		start = i;
		while (s[i] && (!ft_strchr(delims, s[i]) || quote))
		{
			if (!quote && (s[i] == '\'' || s[i] == '\"'))
				quote = s[i];
			else if (quote && s[i] == quote)
				quote = 0;
			i++;
		}
		if (start < i)
		{
			arr[word] = ft_substr(s, start, i - start);
			if (!arr[word])
			{
				ft_free_matrix(&p->envp);
				free(s);
				mini_perror(MEM, NULL, 1);
				exit(1);
			}
			word++;
		}
	}
	arr[word] = NULL;
	return (arr);
}

char	**ft_cmdtrim(char *s, char *delims, t_prompt *p)
// split the command by spaces and ' and ".
{
	char	**arr;
	int		words;

	if (!s)
		return (NULL);
	words = ft_count_words(s, delims);
	if (words == -1)
		return (NULL);
	arr = malloc((words + 1) * sizeof(char *));
	if (!arr)
	{
		ft_free_matrix(&p->envp);
		free(s);
		mini_perror(MEM, NULL, 1);
		exit(1);
	}
	return (ft_fill_array(arr, s, delims, p));
}
