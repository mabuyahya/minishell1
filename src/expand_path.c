/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuyahy <mabuyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:17:55 by mbueno-g          #+#    #+#             */
/*   Updated: 2025/02/16 11:25:57 by mabuyahy         ###   ########.fr       */
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

static char	*replace_tilde(char *str, int *i, char *var)
{
	char	*temp;
	char	*path;
	char	*new_str;

	temp = ft_substr(str, 0, *i);
	if (!temp)
		return (handle_fail_expand_path(temp, NULL, var));
	path = ft_strjoin(temp, var);
	if (!path)
		return (handle_fail_expand_path(temp, path, var));
	free(temp);
	new_str = ft_strjoin(path, str + *i + 1);
	if (!new_str)
		return (handle_fail_expand_path(NULL, path, var));
	free(str);
	free(path);
	*i += ft_strlen(var) - 1;
	return (new_str);
}

char	*expand_path(char *str, int i, int quotes[2], char *var)
{
	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		update_quotes(str[i], quotes);
		if (!quotes[0] && !quotes[1] && str[i] == '~'
			&& (i == 0 || str[i - 1] != '$'))
		{
			str = replace_tilde(str, &i, var);
			if (!str)
				return (NULL);
		}
	}
	free(var);
	return (str);
}
