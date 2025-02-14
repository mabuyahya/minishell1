/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salam <salam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/08 18:17:55 by mbueno-g          #+#    #+#             */
/*   Updated: 2025/02/14 13:22:44 by salam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void update_quotes(char c, int quotes[2])
{
    if (c == '\'' && !quotes[1])
        quotes[0] = !quotes[0];
    else if (c == '"' && !quotes[0])
        quotes[1] = !quotes[1];
}

char *expand_path(char *str, int i, int quotes[2], char *var)
{
    char *aux;
    char *path;

    quotes[0] = 0;
    quotes[1] = 0;
    while (str && str[++i])
    {
        update_quotes(str[i], quotes);
        if (!quotes[0] && !quotes[1] && str[i] == '~' && (i == 0 || str[i - 1] != '$'))
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
    return str;
}

static void allocate_fail(t_prompt *prom, char **args, char *str)
{
	if (str != NULL)
		free(str);
	ft_free_matrix(&prom->envp);
	ft_free_matrix(&args);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

static int convert_pid_to_string(t_prompt *prom, t_expand_var *s_str, char *str, int *i)
{
	if (!s_str->var && str[*i] == '$')
		s_str->var = ft_itoa(prom->pid);
	else if (!s_str->var && str[*i] == '?')
		s_str->var = ft_itoa(g_status);
	if (!s_str->var)
		return (0);
	return (1);
}

static char	*get_vars(char *str, int i, t_prompt *prom, char **args)
{
	t_expand_var s_str;

	s_str.pos = ft_strchars_i(&str[i], "|\"\'$?>< ") + (ft_strchr("$?", str[i]) != 0);
	if (s_str.pos == -1)
		s_str.pos = ft_strlen(str) - 1;
	s_str.aux = ft_substr(str, 0, i - 1);
	if (!s_str.aux)
		allocate_fail(prom, args, NULL);
	if (!s_str.aux)
		return (NULL);
	s_str.var = mini_getenv(&str[i], prom->envp,
		ft_strchars_i(&str[i], "\"\'$|>< "));
	if (!convert_pid_to_string(prom, &s_str, str, &i))
		allocate_fail(prom, args, s_str.aux);
	s_str.path = ft_strjoin(s_str.aux, s_str.var);
	free(s_str.aux);
	if (!s_str.path)
		allocate_fail(prom, args, s_str.var);	
	s_str.aux = ft_strjoin(s_str.path, &str[i + s_str.pos]);
	free(s_str.var);
	free(s_str.path);
	free(str);
	if (!s_str.aux)
		allocate_fail(prom, args, NULL);
	return (s_str.aux);
}

char *expand_vars(char *str, int i, int quotes[2], t_prompt *prompt, char **args)
// expand the commands.
{
    quotes[0] = 0;
    quotes[1] = 0;
    while (str && str[++i])
    {
        update_quotes(str[i], quotes);
        if (!quotes[0] && str[i] == '$' && str[i + 1] &&
            ft_strchars_i(&str[i + 1], "/~%^{}:; ") &&
            (!quotes[1] || str[i + 1] != '"'))
        {
            str = get_vars(str, ++i, prompt, args);
			i = -1;
		}
    }
    return (str);
}
