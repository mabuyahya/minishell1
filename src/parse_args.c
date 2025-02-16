/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuyahy <mabuyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 12:08:12 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/16 13:29:41 by mabuyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;


#include <stdlib.h>
#include <stdbool.h>
#include "libft.h" // Assuming you have ft_strjoin, ft_strdup

static bool is_delim(char c, char *delimiters)
{
	while (*delimiters)
	{
		if (c == *delimiters)
			return (true);
		delimiters++;
	}
	return (false);
}

static char **split_by_multiple_delims(char *str, char *delimiters)
{
	char **result;
	char *start;
	int count;
	int i;

	count = 0;
	for (i = 0; str[i]; i++)
		if (!is_delim(str[i], delimiters) &&
			(i == 0 || is_delim(str[i - 1], delimiters)))
			count++;

	result = malloc((count + 1) * sizeof(char *));
	if (!result)
		return (NULL);

	i = 0;
	while (*str)
	{
		while (*str && is_delim(*str, delimiters))
			str++;
		start = str;
		while (*str && !is_delim(*str, delimiters))
			str++;
		if (start != str)
			result[i++] = ft_substr(start, 0, str - start);
	}
	result[i] = NULL;
	return (result);
}

char *ft_split_and_join(char *str, char *delimiters)
{
	char **split;
	char *result;
	int i;

	if (!str || !delimiters)
		return (NULL);

	// Split but KEEP original spacing intact
	split = split_by_multiple_delims(str, delimiters);
	if (!split || !split[0])
		return (NULL);

	result = ft_strdup(split[0]); // No space added
	if (!result)
		return (ft_free_matrix(&split), NULL);

	i = 1;
	while (split[i])
	{
		char *temp = ft_strjoin(result, split[i]); // Direct join, no extra spaces
		if (!temp)
		{
			ft_free_matrix(&split);
			free(result);
			return (NULL);
		}
		free(result);
		result = temp;
		i++;
	}
	ft_free_matrix(&split);
	return (result);
}

static void	handle_fail_expand(t_prompt *prom, char **args, char **str)
{
	ft_free_matrix(&prom->envp);
	if (args && args[0])
		ft_free_matrix(&args);
	if (str && str[0])
		ft_free_matrix(&str);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

static char	*get_substr_var(char *str, int i, t_prompt *prompt, int in_quotes)
{
	char	*aux;
	char	*var;
	char	*path;
	int	pos;

	pos = ft_strchars_i(&str[i], "|\"\'$?>< ") + (ft_strchr("$?", str[i]) != 0);
	if (pos == -1)
		pos = ft_strlen(&str[i]);
	aux = ft_substr(str, 0, i - 1);
	var = mini_getenv(&str[i], prompt->envp, pos);

	if (!var && str[i] == '$')
		var = ft_itoa(prompt->pid);
	else if (!var && str[i] == '?')
		var = ft_itoa(g_status);

	if (!in_quotes && var)
		var = ft_split_and_join(var, " \t");

	path = ft_strjoin(aux, var);
	free(aux);
	free(var);
	aux = ft_strjoin(path, &str[i + pos]);
	free(path);
	free(str);

	return (aux);
}

char	*expand_vars(char *str, int i, int quotes[2], t_prompt *prompt)
{
	quotes[0] = 0;
	quotes[1] = 0;
	while (str && str[++i])
	{
		if (str[i] == '\'' && !quotes[1])
			quotes[0] = !quotes[0];
		else if (str[i] == '"' && !quotes[0])
			quotes[1] = !quotes[1];
		if (!quotes[0] && str[i] == '$' && str[i + 1])
		{
			str = get_substr_var(str, ++i, prompt, quotes[1]);
			i = -1; // Reset index to reprocess new string
		}
	}
	return (str);
}

// remove and handle single double quotes and ~.
static char	**expand(char **args, t_prompt *prom)
{
	char	**str;
	int		i;
	int		quotes[2];

	i = -1;
	while (args && args[++i])
	{
		args[i] = expand_vars(args[i], -1, quotes, prom);
		args[i] = expand_path(args[i], -1, quotes, mini_getenv("HOME",
					prom->envp, 4));
		if (!args[i])
			handle_fail_expand(prom, args, str);

		// Re-tokenize after expansion to handle `|`, `>` correctly
		str = ft_cmdsubsplit(args[i], "<|>");
		if (!str)
			handle_fail_expand(prom, args, NULL);
		
		if (!ft_matrix_replace_in(&args, str, i))
			handle_fail_expand(prom, args, str);
		
		i = i + ft_matrixlen(str) - 1;
		ft_free_matrix(&str);
	}
	return (args);
}


// waitpid(-1, NULL, NULL) : to wait any chiled process.
static void	*parse_args(char **args, t_prompt *prom)
{
	int	is_exit;
	int	i;

	is_exit = 0;
	prom->cmds = fill_nodes(expand(args, prom), -1, prom);
	if (!prom->cmds)
		return (prom);
	i = ft_lstsize(prom->cmds);
	g_status = builtin(prom, prom->cmds, &is_exit, args);
	while (i-- > 0)
		waitpid(-1, &g_status, 0);
	if (!is_exit && g_status == 13)
		g_status = 0;
	if (g_status > 255)
		g_status = g_status / 255;
	if (args && is_exit)
	{
		ft_lstclear(&prom->cmds, free_content);
		return (NULL);
	}
	return (prom);
}

void	check_args_util(t_prompt *prom, t_mini *node)
{
	if (prom && prom->cmds)
		node = prom->cmds->content;
	if (prom && prom->cmds && node && node->full_cmd
		&& ft_lstsize(prom->cmds) == 1)
		prom->envp = mini_setenv("_",
				node->full_cmd[ft_matrixlen(node->full_cmd) - 1], prom->envp,
				1);
	if (prom && prom->cmds)
		ft_lstclear(&prom->cmds, free_content);
}

// all solution.
void	*check_args(char *read, t_prompt *prom)
// str is a splited read acording to spasess and quotes
{
	char	**str;
	t_mini	*node;

	node = NULL;
	if (!read)
	{
		printf("exit\n");
		return (NULL);
	}
	if (read[0] != '\0')
		add_history(read);
	str = split_quote_space(read, " ", prom);
	if (read)
		free(read);
	if (!str)
		mini_perror(QUOTE, NULL, 1);
	if (!str)
		return ("");
	prom = parse_args(str, prom);
	check_args_util(prom, node);
	return (prom);
}
