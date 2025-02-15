/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim_all.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salam <salam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/02 20:59:58 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/13 17:32:06 by salam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	malloc_len(char const *s1)
{
	int	count;
	int	i;
	int	dquote;
	int	squote;

	i = 0;
	count = 0;
	dquote = 0;
	squote = 0;
	while (s1 && s1[i])
	{
		if (!dquote && s1[i] == '\'')
			squote = !squote;
		if (!squote && s1[i] == '\"')
			dquote = !dquote;
		if ((s1[i] == '\"' && !squote) || (s1[i] == '\'' && !dquote))
			count++;
		i++;
	}
	if (squote || dquote)
		return (-1);
	return (count);
}

char	*ft_strtrim_all(char const *s1, int squote, int dquote)
// return the string without ' and ".
{
	int count;
	int i[2];
	char *trimmed;

	i[1] = -1;
	i[0] = 0;
	count = malloc_len(s1);
	if (!s1 || count == -1)
		return (NULL);
	trimmed = malloc(sizeof(char) * (ft_strlen(s1) - count + 1));
	if (!trimmed)
		return (NULL);
	while (s1[i[0]])
	{
		if (!dquote && s1[i[0]] == '\'')
			squote = !squote;
		if (!squote && s1[i[0]] == '\"')
			dquote = !dquote;
		if ((s1[i[0]] != '\"' || squote) && (s1[i[0]] != '\'' || dquote)
				&& ++i[1] >= 0)
			trimmed[i[1]] = s1[i[0]];
		i[0]++;
	}
	trimmed[++i[1]] = '\0';
	return (trimmed);
}
