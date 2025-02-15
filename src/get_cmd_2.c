/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_cmd_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 18:49:26 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/15 18:55:54 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// if the argument is directory.
// if the command like this : /bin/ls, or ./minishell.
static DIR	*check_cmd(t_prompt *prom, t_list *cmd, char ***s, char *path)
{
	DIR	*dir;

	dir = check_directory(cmd, s, path, prom);
	if (!dir)
		check_command_path(cmd, s, path, prom);
	return (dir);
}

void	get_cmd(t_prompt *prompt, t_list *cmd, char **s, char *path)
{
	t_mini	*n;
	DIR		*dir;

	n = cmd->content;
	dir = check_cmd(prompt, cmd, &s, path);
	if (!is_builtin(n) && n && n->full_cmd && dir)
		mini_perror(IS_DIR, *n->full_cmd, 126);
	else if (!is_builtin(n) && n && n->full_path
		&& access(n->full_path, F_OK) == -1)
		mini_perror(NDIR, n->full_path, 127);
	else if (!is_builtin(n) && n && n->full_path
		&& access(n->full_path, X_OK) == -1)
		mini_perror(NPERM, n->full_path, 126);
	if (dir)
		closedir(dir);
	ft_free_matrix(&s);
}

// execute the command, if the command does not built in.
void	*exec_cmd(t_prompt *prompt, t_list *cmd)
{
	int	pipe_fd[2];

	get_cmd(prompt, cmd, NULL, NULL);
	if (pipe(pipe_fd) == -1)
		return (mini_perror(PIPERR, NULL, 1));
	if (!check_to_fork(prompt, cmd, pipe_fd))
		return (NULL);
	close(pipe_fd[WRITE_END]);
	if (cmd->next && !((t_mini *)cmd->next->content)->infile)
		((t_mini *)cmd->next->content)->infile = pipe_fd[READ_END];
	else
		close(pipe_fd[READ_END]);
	if (((t_mini *)cmd->content)->infile > 2)
		close(((t_mini *)cmd->content)->infile);
	if (((t_mini *)cmd->content)->outfile > 2)
		close(((t_mini *)cmd->content)->outfile);
	return (NULL);
}
