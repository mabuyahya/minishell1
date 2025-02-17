/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 11:36:47 by mbueno-g          #+#    #+#             */
/*   Updated: 2025/02/17 18:50:04 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	*mini_perror(int err_type, char *param, int err)
// print error if exist.
{
	g_status = err;
	if (err_type == QUOTE)
		ft_putstr_fd("minishell: error while looking for matching quote\n", 2);
	else if (err_type == NDIR)
		ft_putstr_fd("minishell: No such file or directory: ", 2);
	else if (err_type == NPERM)
		ft_putstr_fd("minishell: permission denied: ", 2);
	else if (err_type == NCMD)
		ft_putstr_fd("minishell: command not found: ", 2);
	else if (err_type == DUPERR)
		ft_putstr_fd("minishell: dup2 failed\n", 2);
	else if (err_type == FORKERR)
		ft_putstr_fd("minishell: fork failed\n", 2);
	else if (err_type == PIPERR)
		ft_putstr_fd("minishell: error creating pipe\n", 2);
	else if (err_type == PIPENDERR)
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	else if (err_type == MEM)
		ft_putstr_fd("minishell: no memory left on device\n", 2);
	else if (err_type == IS_DIR)
		ft_putstr_fd("minishell: Is a directory: ", 2);
	else if (err_type == NOT_DIR)
		ft_putstr_fd("minishell: Not a directory: ", 2);
	ft_putendl_fd(param, 2);
	return (NULL);
}

int	check_num_exit(const char *num_ptr, int *number)
{
	int	neg;

	neg = 1;
	*number = 0;
	while (ft_isspace(*num_ptr))
		num_ptr++;
	if (*num_ptr == '-')
		neg = -neg;
	if (*num_ptr == '-' || *num_ptr == '+')
		num_ptr++;
	if (!ft_isdigit(*num_ptr))
		return (-1);
	while (ft_isdigit(*num_ptr))
	{
		*number = 10 * *number + (*num_ptr - '0');
		num_ptr++;
	}
	if (*num_ptr && !ft_isspace(*num_ptr))
		return (-1);
	*number *= neg;
	return (0);
}

static void handle_print_exit(t_mini *node)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(node->full_cmd[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

// handle the command exit.
// status_arr[0] = status_arr[0] % 256 + 256 * (status_arr[0] < 0); : to handle over flow.
int	handle_exit(t_list *cmd, int *exit_num, t_prompt *prom)
{
	t_mini	*node;
	int		status_arr[2];

	node = cmd->content;
	*exit_num = (!cmd->next && (prom->size == 1));
	if (*exit_num)
		ft_putstr_fd("exit\n", 2);
	if (!node->full_cmd || !node->full_cmd[1])
		return (0);
	status_arr[1] = check_num_exit(node->full_cmd[1], &status_arr[0]);
	if (status_arr[1] == -1)
	{
		handle_print_exit(node);
		return (255);
	}
	else if (node->full_cmd[2])
	{
		*exit_num = 0;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	status_arr[0] = status_arr[0] % 256 + 256 * (status_arr[0] < 0);
	return (status_arr[0]);
}

void	cd_error(char **str[2])
// handle cd.
{
	DIR	*dir;

	dir = NULL;
	if (str[0][1])
		dir = opendir(str[0][1]);
	if (!str[0][1] && str[1][0] && !str[1][0][0])
	{
		g_status = 1;
		ft_putstr_fd("minishell: HOME not set\n", 2);
	}
	if (str[1][0] && !str[0][1])
		g_status = chdir(str[1][0]) == -1;
	if (str[0][1] && dir && access(str[0][1], F_OK) != -1)
		chdir(str[0][1]);
	else if (str[0][1] && access(str[0][1], F_OK) == -1)
		mini_perror(NDIR, str[0][1], 1);
	else if (str[0][1])
		mini_perror(NOT_DIR, str[0][1], 1);
	if (str[0][1] && dir)
		closedir(dir);
}

void	free_content(void *content)
// free the content of the node(full_cmd, full_path).
{
	t_mini	*node;

	node = content;
	ft_free_matrix(&node->full_cmd);
	free(node->full_path);
	if (node->infile > 0)
		close(node->infile);
	if (node->outfile > 1)
		close(node->outfile);
	free(node);
}
