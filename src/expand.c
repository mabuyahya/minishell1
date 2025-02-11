/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:17:55 by mbueno-g          #+#    #+#             */
/*   Updated: 2025/02/11 13:16:16 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

char	*expand_path(char *str, int i, int quotes[2], char *var)
// to handle ~ and $.
{
	char	*new_str;
	char	*aux;

	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		if (str[i] == '\'' && !quotes[1])
			quotes[0] = !quotes[0];
		else if (str[i] == '\"' && !quotes[0])
			quotes[1] = !quotes[1];
		if (!quotes[0] && !quotes[1] && str[i] == '~' && (i == 0 || str[i - 1] != '$'))
		{
			aux = ft_substr(str, 0, i);
			new_str = ft_strjoin(aux, var);
			free(aux);
			aux = ft_strjoin(new_str, str + i + 1);
			free(new_str);
			free(str);
			str = aux;
			i += ft_strlen(var) - 1;
		}
	}
	free(var);
	return (str);
}

static char	*get_substr_var(char *str, int i, t_prompt *prompt)
{
	char	*aux;
	int		pos;
	char	*path;
	char	*var;

	pos = ft_strchars_i(&str[i], "|\"\'$?>< ") + (ft_strchr("$?", str[i]) != 0);
	if (pos == -1)
		pos = ft_strlen(str) - 1;
	aux = ft_substr(str, 0, i - 1);
	if (!aux)
	{
		ft_free_matrix(&prompt->envp);
		mini_perror(MEM, ".", 1);
		exit(1);
	}
	var = mini_getenv(&str[i], prompt->envp, \
		ft_strchars_i(&str[i], "\"\'$|>< "));
	if (!var && str[i] == '$')
		var = ft_itoa(prompt->pid);
	else if (!var && str[i] == '?')
		var = ft_itoa(g_status);
	if (!var)
	{
		free(aux);
		ft_free_matrix(&prompt->envp);
		mini_perror(MEM, ".", 1);
		exit(1);
	}
	path = ft_strjoin(aux, var);
	if (!path)
	{
		free(aux);
		ft_free_matrix(&prompt->envp);
		mini_perror(MEM, ".", 1);
		exit(1);
	}
	free(aux);
	aux = ft_strjoin(path, &str[i + pos]);
	if (!aux)
	{
		free(aux);
		ft_free_matrix(&prompt->envp);
		mini_perror(MEM, ".", 1);
		exit(1);
	}
	free(var);
	free(path);
	free(str);
	return (aux);
}

char	*expand_vars(char *str, int i, int quotes[2], t_prompt *prompt)
// expand commands.
{
	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		if (str[i] == '\'' && !quotes[1])
			quotes[0] = !quotes[0];
		else if (str[i] == '\"' && !quotes[0])
			quotes[1] = !quotes[1];
		if (!quotes[0] && str[i] == '$' && str[i + 1] && 
			ft_strchars_i(&str[i + 1], "/~%^{}:; ") && (!quotes[1] || str[i + 1] != '\"'))
		{
			str = get_substr_var(str, ++i, prompt);
			i = -1;
		}
	}
	return (str);
}
