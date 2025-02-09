/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmdtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuyahy <mabuyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:28:36 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/08 14:27:03 by mabuyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

#include <stdlib.h>
#include <stdio.h>

static int	ft_count_words(const char *s, char *delims)
{
	int	count = 0;
	int	i = 0;
	char	quote = 0;

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
	return (quote ? -1 : count);
}

static char	**ft_fill_array(char **arr, const char *s, char *delims)
{
	int		i = 0, start, word = 0;
	char	quote = 0;

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
			arr[word++] = ft_substr(s, start, i - start);
	}
	arr[word] = NULL;
	return (arr);
}

char	**ft_cmdtrim(const char *s, char *delims)
// split the command the user enter it bu spaces and ' and ".
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
		return (NULL);

	return (ft_fill_array(arr, s, delims));
}
