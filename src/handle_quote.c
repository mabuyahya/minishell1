/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quote.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/28 15:28:36 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/19 14:53:55 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// count the wrod by space and ' and "
static int	ft_count_words(const char *str, char *seo)
{
	int		count;
	int		i;
	char	quote;

	i = 0;
	count = 0;
	quote = 0;
	while (str[i])
	{
		while (str[i] && ft_strchr(seo, str[i]))
			i++;
		if (str[i])
			count++;
		while (str[i] && (!ft_strchr(seo, str[i]) || quote))
		{
			if (!quote && (str[i] == '\'' || str[i] == '\"'))
				quote = str[i];
			else if (quote && str[i] == quote)
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

static void	update_quotes(char *str, int *i, char *quote)
{
	if (!*quote && (str[*i] == '\'' || str[*i] == '"'))
		*quote = str[*i];
	else if (*quote && str[*i] == *quote)
		*quote = 0;
	(*i)++;
}

// split word by ' "
static char	**make_command(char **array, char *str, char *sep, t_prompt *prom)
{
	int		i;
	int		start;
	int		word;
	char	quote;

	i = 0;
	word = 0;
	quote = 0;
	while (str[i])
	{
		while (str[i] && ft_strchr(sep, str[i]))
			i++;
		start = i;
		while (str[i] && (!ft_strchr(sep, str[i]) || quote))
			update_quotes(str, &i, &quote);
		if (start < i)
		{
			array[word] = ft_substr(str, start, i - start);
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
	array = make_command(array, str, sep, prom);
	return (array);
}
