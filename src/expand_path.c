/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salam <salam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:17:55 by mbueno-g          #+#    #+#             */
/*   Updated: 2025/02/16 05:35:53 by salam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void	update_quotes(char c, int quotes[2])
{
	if (c == '\'' && !quotes[1])
		quotes[0] = !quotes[0];
	else if (c == '"' && !quotes[0])
		quotes[1] = !quotes[1];
}

static int	expand_path_util(char *aux, char *path, char *str, int *i)
{
	aux = ft_strjoin(path, str + *i + 1);
	if (!aux)
	{
		free(str);
		free(path);
		return (0);
	}
	free(str);
	free(path);
	return (1);
}

static char	*handle_fail_expand_path(char *aux, char *path, char *var)
{
	if (path)
		free(path);
	if (aux)
		free(aux);
	if (var)
		free(var);
	return (NULL);
}

char	*expand_path(char *str, int i, int quotes[2], char *var)
{
	char	*aux;
	char	*path;

	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		update_quotes(str[i], quotes);
		if (!quotes[0] && !quotes[1] && str[i] == '~'
			&& (i == 0 || str[i - 1] != '$'))
		{
			aux = ft_substr(str, 0, i);
			if (!aux)
				return (handle_fail_expand_path(aux, NULL, var));
			path = ft_strjoin(aux, var);
			if (!path)
				return (handle_fail_expand_path(aux, path, var));
			free(aux);
			if (!expand_path_util(aux, path, str, &i))
				return (handle_fail_expand_path(NULL, NULL, var));
			return (expand_path(aux, i + ft_strlen(var) - 1, quotes, var));
		}
	}
	free(var);
	return (str);
}
