/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 18:49:26 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/18 20:41:47 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// static DIR	*cmd_checks(t_prompt *prompt, t_list *cmd, char ***s, char *path)
// {
// 	DIR		*dir;

// 	dir = check_directory(cmd, s, path, prompt);
// 	check_command_path(cmd, s, path, prompt);
// 	return (dir);
// }

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

static DIR	*cmd_checks(t_prompt *prompt, t_list *cmd, char ***s, char *path)
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
		n->full_path = ft_strdup(*n->full_cmd);
		free(n->full_cmd[0]);
		n->full_cmd[0] = ft_strdup(s[0][ft_matrixlen(*s) - 1]);
	}
	else if (!check_if_built_in(n) && n && n->full_cmd && !dir)
	{
		path = mini_getenv("PATH", prompt->envp, 4, prompt);
		*s = ft_split(path, ':');
		free(path);
		n->full_path = find_command(*s, *n->full_cmd, n->full_path);
		if (!n->full_path || !n->full_cmd[0] || !n->full_cmd[0][0])
			mini_perror(NCMD, *n->full_cmd, 127, prompt);
	}
	return (dir);
}

void	get_cmd(t_prompt *prom, t_list *cmd, char **str, char *path)
{
	t_node_content	*node;
	DIR		*dir;

	node = cmd->content;
	dir = cmd_checks(prom, cmd, &str, path);
	if (!check_if_built_in(node) && node && node->full_cmd && dir)
		mini_perror(IS_DIR, *node->full_cmd, 126, prom);
	else if (!check_if_built_in(node) && node && node->full_path
		&& access(node->full_path, F_OK) == -1)
		mini_perror(NDIR, node->full_path, 127, prom);
	else if (!check_if_built_in(node) && node && node->full_path
		&& access(node->full_path, X_OK) == -1)
		mini_perror(NPERM, node->full_path, 126, prom);
	if (dir)
		closedir(dir);
	ft_free_matrix(&str);
}

// execute the command, if the command does not built in.
void	execute_command(t_prompt *prom, t_list *cmd, char **command)
{
	int	pipes[2];

	get_cmd(prom, cmd, NULL, NULL);
	if (pipe(pipes) == -1)
	{
		mini_perror(PIPERR, NULL, 1, prom);
		return ;
	}
	if (!check_to_execute(prom, cmd, pipes, command))
		return ;
	close(pipes[1]);
	if (cmd->next && !((t_node_content *)cmd->next->content)->infile)
		((t_node_content *)cmd->next->content)->infile = pipes[0];
	else
		close(pipes[0]);
	if (((t_node_content *)cmd->content)->infile > 1)
		close(((t_node_content *)cmd->content)->infile);
	if (((t_node_content *)cmd->content)->outfile > 1)
		close(((t_node_content *)cmd->content)->outfile);
}
