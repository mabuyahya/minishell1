/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_var.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuyahy <mabuyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 13:29:14 by mabuyahy          #+#    #+#             */
/*   Updated: 2025/02/16 13:29:59 by mabuyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_variable_util(t_expand_read_2 *var,
	char *line, int prefix_len)
{
	ft_strncpy(var->expanded, line, prefix_len);
	var->expanded[prefix_len] = '\0';
	ft_strcat(var->expanded, var->env_value);
	free(var->env_value);
	strcat(var->expanded, var->key_eend);
	free(line);
}

static char *get_env_value(char *key, t_prompt *main)
{
    char *env_value;

    env_value = mini_getenv(key, main->envp, -1);
    if (!env_value)
        env_value = ft_strdup("");
    return env_value;
}

// static void free_env_value(char **env_value)
// {
//     if (*env_value)
//         free(*env_value);
// }

static char *expand_unquoted_variable(t_expand_read_2 *var)
{
    char **words;
    char *temp;
    int i;

    words = ft_split(var->env_value, ' ');
    free(var->env_value);
    if (!words)
        return (NULL);
    var->env_value = ft_strdup("");
    i = -1;
    while (words[++i])
    {
        temp = ft_strjoin("\"", words[i]);
        free(words[i]);
        words[i] = ft_strjoin(temp, "\"");
        free(temp);
        temp = var->env_value;
        var->env_value = ft_strjoin(temp, words[i]);
        free(temp);
        free(words[i]);
        if (words[i + 1])
        {
            temp = var->env_value;
            var->env_value = ft_strjoin(temp, " ");
            free(temp);
        }
    }
    free(words);
    return (var->env_value);
}

static char *expand_quoted_variable(t_expand_read_2 *var, char *line, size_t new_len, size_t prefix_len)
{
    var->expanded = ft_calloc(new_len + 1, 1);
    if (!var->expanded)
    {
        free(var->env_value);
        return (line);
    }
    ft_strncpy(var->expanded, line, prefix_len);
    var->expanded[prefix_len] = '\0';
    ft_strcat(var->expanded, "'");
    ft_strcat(var->expanded, var->env_value);
    ft_strcat(var->expanded, "'");
    return (var->expanded);
}

static char *expand_variable_standard(char *line, char *key_start, t_expand_read_2 *var)
{
    size_t new_len = ft_strlen(line) - var->len + ft_strlen(var->env_value);
    size_t prefix_len = key_start - line - 1;

    var->expanded = ft_calloc(new_len + 1, 1);
    if (!var->expanded)
    {
        free(var->env_value);
        return (line);
    }
    expand_variable_util(var, line, prefix_len);
    return (var->expanded);
}

static char *expand_variable(char *line, char *key_start, t_prompt *main, int quoted)
{
    t_expand_read_2 var;
    size_t new_len;
    size_t prefix_len;

    var.key_eend = key_start;
    while (*var.key_eend && (ft_isalnum(*var.key_eend) || *var.key_eend == '?'))
        var.key_eend++;
    var.len = var.key_eend - key_start;
    var.key = ft_strndup(key_start, var.len);
    if (!var.key)
        return (line);

    var.env_value = get_env_value(var.key, main);
    free(var.key);

    // Handle unquoted expansion case
    if (!quoted)
    {
        return expand_unquoted_variable(&var);
    }

    // Special logic for single-quoted expansion: Keep single quotes
    if (quoted && line[0] == '\'' && line[strlen(line) - 1] == '\'')
    {
        new_len = ft_strlen(line) - var.len + ft_strlen(var.env_value) + 2;
        prefix_len = key_start - line - 1;
        return expand_quoted_variable(&var, line, new_len, prefix_len);
    }

    return expand_variable_standard(line, key_start, &var);
}


static void	init_expand_read(t_expand_read *var, char *line)
{
	var->in_double_quote = 0;
	var->in_single_quote = 0;
	var->ptr = NULL;
	var->offset = 0;
	var->new_line = NULL;
	var->ptr = line;
}

// remove $
char	*expand_variables(t_prompt *prom, char *line)
{
	t_expand_read	var;
	int	quoted;

	if (!line)
		return (NULL);
	init_expand_read(&var, line);
	quoted = 0;
	while (*var.ptr)
	{
		if (*var.ptr == '\'')
			var.in_single_quote = !var.in_single_quote;
		else if (*var.ptr == '"')
			var.in_double_quote = !var.in_double_quote;
		else if (*var.ptr == '$' && !var.in_single_quote)
		{
			var.offset = var.ptr - line;
			quoted = var.in_double_quote; // Detect if inside double quotes
			var.new_line = expand_variable(line, var.ptr + 1, prom, quoted);
			if (!var.new_line)
				return (line);
			var.ptr = var.new_line + var.offset;
			line = var.new_line;
		}
		var.ptr++;
	}
	return (line);
}