/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 17:02:44 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/18 20:24:43 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	stop_find_command(char **paths, t_node_content *n, t_prompt *prom)
{
	ft_free_matrix(&prom->envp);
	if (paths && paths[0])
		ft_free_matrix(&paths);
	free_content(n);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

static void	stop_check_cmd(char *path, t_list *cmd, char ***s, t_prompt *prom)
{
	if (path)
		free(path);
	if (s)
		ft_free_matrix(s);
	stop_make_node(cmd, NULL, prom->envp);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

// find the file of the command in a PATH.
static char	*find_command(char **env_path, char *cmd, char *full_path)
{
	char	*temp;
	int		i;

	i = -1;
	full_path = NULL;
	while (env_path && env_path[++i])
	{
		free(full_path);
		temp = ft_strjoin(env_path[i], "/");
		if (!temp)
			return (NULL);
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
			return (NULL);
		if (access(full_path, F_OK) == 0)
			break ;
	}
	if (!env_path || !env_path[i])
	{
		free(full_path);
		return (NULL);
	}
	return (full_path);
}


void	check_command_path(t_list *cmd, char ***s, char *path,
		t_prompt *prom)
{
	t_node_content	*n;

	n = cmd->content;
	if (!check_if_built_in(n) && n && n->full_cmd && !opendir(*n->full_cmd))
	{
		path = mini_getenv("PATH", prom->envp, 4, prom);
		*s = ft_split(path, ':');
		if (!*s)
			stop_check_cmd(path, cmd, s, prom);
		free(path);
		n->full_path = find_command(*s, n, prom);
		if (!n->full_path || !n->full_cmd[0] || !n->full_cmd[0][0])
			mini_perror(NCMD, *n->full_cmd, 127);
	}
}

DIR	*check_directory(t_list *cmd, char ***s, char *path, t_prompt *prom)
{
	t_node_content	*n;
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
