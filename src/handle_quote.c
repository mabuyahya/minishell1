/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:28:36 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/16 19:22:31 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// count the wrod by space and ' and "
static int	ft_count_words(const char *s, char *seo)
{
	int		count;
	int		i;
	char	quote;

	i = 0;
	count = 0;
	quote = 0;
	while (s[i])
	{
		while (s[i] && ft_strchr(seo, s[i]))
			i++;
		if (s[i])
			count++;
		while (s[i] && (!ft_strchr(seo, s[i]) || quote))
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
	return (count);
}

static void	handle_allocation_failure(char **array, t_prompt *prom)
{
	ft_free_matrix(&array);
	fail_allocate(prom, MEM);
}

static void	update_quotes(char *s, int *i, char *quote)
{
	if (!*quote && (s[*i] == '\'' || s[*i] == '"'))
		*quote = s[*i];
	else if (*quote && s[*i] == *quote)
		*quote = 0;
	(*i)++;
}

// split word by ' "
static char	**ft_fill_array(char **array, char *s, char *sep, t_prompt *prom)
{
	int		i;
	int		start;
	int		word;
	char	quote;

	i = 0;
	word = 0;
	quote = 0;
	while (s[i])
	{
		while (s[i] && ft_strchr(sep, s[i]))
			i++;
		start = i;
		while (s[i] && (!ft_strchr(sep, s[i]) || quote))
			update_quotes(s, &i, &quote);
		if (start < i)
		{
			array[word] = ft_substr(s, start, i - start);
			if (!array[word++])
				handle_allocation_failure(array, prom);
		}
	}
	array[word] = NULL;
	return (array);
}

// split the command by spaces and ' and ".
char	**split_quote_space(char *str, char *sep, t_prompt *prom)
{
	char	**array;
	int		words;

	if (!str)
		return (NULL);
	words = ft_count_words(str, sep);
	if (words == -1)
		return (NULL);
	array = malloc((words + 1) * sizeof(char *));
	if (!array)
	{
		free(str);
		fail_allocate(prom, MEM);
		exit(1);
	}
	array = ft_fill_array(array, str, sep, prom);
	return (array);
}
