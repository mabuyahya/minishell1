/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/21 15:51:24 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/15 18:50:37 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	stop_find_command(char **paths, t_mini *n, t_prompt *prom)
{
	ft_free_matrix(&prom->envp);
	if (paths && paths[0])
		ft_free_matrix(&paths);
	free_content(n);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

// find the file of the command in a PATH.
static char	*find_command(char **paths, t_mini *n, t_prompt *prom)
{
	char	*temp;
	int		i;

	i = -1;
	n->full_path = NULL;
	while (paths && paths[++i])
	{
		free(n->full_path);
		temp = ft_strjoin(paths[i], "/");
		if (!temp)
			stop_find_command(paths, n, prom);
		n->full_path = ft_strjoin(temp, *n->full_cmd);
		free(temp);
		if (!n->full_path)
			stop_find_command(paths, n, prom);
		if (access(n->full_path, F_OK) == 0)
			break ;
	}
	if (!paths || !paths[i])
	{
		free(n->full_path);
		return (NULL);
	}
	return (n->full_path);
}

static void	stop_check_cmd(char *path, t_list *cmd, char ***s, t_prompt *prom)
{
	if (path)
		free(path);
	if (s)
		ft_free_matrix(s);
	stop_fill(cmd, NULL, prom->envp);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

DIR	*check_directory(t_list *cmd, char ***s, char *path, t_prompt *prom)
{
	t_mini	*n;
	DIR		*dir;

	dir = NULL;
	n = cmd->content;
	if (n && n->full_cmd)
		dir = opendir(*n->full_cmd);
	if (n && n->full_cmd && ft_strchr(*n->full_cmd, '/') && !dir)
	{
		*s = ft_split(*n->full_cmd, '/');
		if (!*s)
			stop_check_cmd(path, cmd, s, prom);
		n->full_path = ft_strdup(*n->full_cmd);
		if (!n->full_path)
			stop_check_cmd(path, cmd, s, prom);
		free(n->full_cmd[0]);
		n->full_cmd[0] = ft_strdup(s[0][ft_matrixlen(*s) - 1]);
		if (!n->full_cmd[0])
			stop_check_cmd(path, cmd, s, prom);
	}
	return (dir);
}

void	check_command_path(t_list *cmd, char ***s, char *path,
		t_prompt *prom)
{
	t_mini	*n;

	n = cmd->content;
	if (!is_builtin(n) && n && n->full_cmd && !opendir(*n->full_cmd))
	{
		path = mini_getenv("PATH", prom->envp, 4);
		*s = ft_split(path, ':');
		if (!*s)
			stop_check_cmd(path, cmd, s, prom);
		free(path);
		n->full_path = find_command(*s, n, prom);
		if (!n->full_path || !n->full_cmd[0] || !n->full_cmd[0][0])
			mini_perror(NCMD, *n->full_cmd, 127);
	}
}
