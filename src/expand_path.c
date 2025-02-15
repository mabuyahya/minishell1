/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:17:55 by mbueno-g          #+#    #+#             */
/*   Updated: 2025/02/15 18:38:24 by sbibers          ###   ########.fr       */
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
			path = ft_strjoin(aux, var);
			free(aux);
			aux = ft_strjoin(path, str + i + 1);
			free(str);
			free(path);
			return (expand_path(aux, i + ft_strlen(var) - 1, quotes, var));
		}
	}
	free(var);
	return (str);
}
