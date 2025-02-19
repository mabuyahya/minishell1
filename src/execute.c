/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/05 18:49:29 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/19 13:17:01 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_e_status;

static void put_export(char **str, int fd)
{
	int	i;
	
	i = 0;
	while (str && str[i])
	{
		ft_putstr_fd("export : ", fd);
		ft_putstr_fd(str[i], fd);
		ft_putstr_fd("\n", fd);
		i++;
	}
}

static void	child_check_built_in(t_node_content *n, size_t *size, t_prompt *prompt, t_list *cmd)
{
	if (n->full_cmd && !ft_strncmp(*n->full_cmd, "pwd", *size) && *size == 3)
	{
		prompt->exit_status = mini_pwd();
		ft_free_matrix(&prompt->envp);
	}
	else if (check_if_built_in(n) && n->full_cmd
		&& !ft_strncmp(*n->full_cmd, "echo", *size) && *size == 4)
	{
		prompt->exit_status = mini_echo(cmd);
		ft_free_matrix(&prompt->envp);
	}
	else if (check_if_built_in(n) && n->full_cmd
		&& !ft_strncmp(*n->full_cmd, "env", *size) && *size == 3)
	{
		ft_putmatrix_fd(prompt->envp, 1, 1);
		ft_free_matrix(&prompt->envp);
		prompt->exit_status = 0;
	}
}

// execute the command and check the command.
void	execve_the_command(t_prompt *prompt, t_node_content *node, t_list *cmd, char **args)
{
	size_t	size;

	if (prompt->flag == 1)
		return;
	if (node->full_cmd)
		size = ft_strlen(*node->full_cmd);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (!check_if_built_in(node) && node->full_cmd)
		execve(node->full_path, node->full_cmd, prompt->envp);
	else if (check_if_built_in(node) && node->full_cmd
		&& !ft_strncmp(*node->full_cmd, "cd", size) && size == 2)
	{
		prompt->exit_status = handle_cd(prompt, cmd, args);
		ft_free_matrix(&prompt->envp);
	}
	else if (check_if_built_in(node) && node->full_cmd
		&& !ft_strncmp(*node->full_cmd, "export", size) && size == 6)
	{
		put_export(prompt->envp, 1);
		ft_free_matrix(&prompt->envp);
	}
	else
		child_check_built_in(node, &size, prompt, cmd);
}

static void	*check_in_out(t_list *cmd, int fd[2], t_prompt *prom)
// redirection the command to input and output file or pipe.
{
	t_node_content	*node;

	node = cmd->content;
	if (node->infile != 0)
	{
		if (dup2(node->infile, 0) == -1)
			return (mini_perror(DUPERR, NULL, 1, prom));
		close(node->infile);
	}
	if (node->outfile != 1)
	{
		if (dup2(node->outfile, 1) == -1)
			return (mini_perror(DUPERR, NULL, 1, prom));
		close(node->outfile);
	}
	else if (cmd->next && dup2(fd[1], 1) == -1)
		return (mini_perror(DUPERR, NULL, 1, prom));
	close(fd[1]);
	return ("");
}

static void	*executer_2(t_prompt *prompt, t_list *cmd, int fd[2], char **command)
// after make fork this is chiled process.
{
	t_node_content	*node;
	node = cmd->content;
	check_in_out(cmd, fd, prompt);
	close(fd[0]);
	execve_the_command(prompt, node, cmd, command);
	ft_lstclear(&prompt->cmds, free_content);
	exit(prompt->exit_status);
}

static void	executer(t_prompt *prompt, t_list *cmd, int fd[2], char **args)
// make chiled process.
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		close(fd[0]);
		close(fd[1]);
		mini_perror(FORKERR, NULL, 1, prompt);
	}
	else if (pid == 0)
		executer_2(prompt, cmd, fd, args);
}

int 	check_to_execute(t_prompt *prompt, t_list *cmd, int pipes[2], char **args)
// check the command before make chiled process.
{
	t_node_content	*node;
	DIR		*directory;

	node = cmd->content;
	directory = NULL;
	if (node->full_cmd)
		directory = opendir(*node->full_cmd);
	if (node->infile == -1 || node->outfile == -1)
		return (0);
	if ((node->full_path && access(node->full_path, X_OK) == 0)
		|| check_if_built_in(node))
	{
		executer(prompt, cmd, pipes, args);
		if (prompt->exit_status == 130 && prompt->flag == 0)
			prompt->exit_status = 0;
	}
	else if (!check_if_built_in(node) && ((node->full_path
		&& !access(node->full_path, F_OK)) || directory))
		prompt->exit_status = 126;
	else if (!check_if_built_in(node) && node->full_cmd)
		prompt->exit_status = 127;
	if (directory)
		closedir(directory);
	return (1);
}
