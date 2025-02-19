/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 15:41:34 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/19 17:02:03 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_e_status;

void	*mini_perror(int err_type, char *param, int err, t_prompt *prom)
// print error if exist.
{
	prom->exit_status = err;
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

static int	check_number_exit(const char *str, int *num)
{
	int		neg;

	neg = 1;
	*num = 0;
	while ((*str == ' ') || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-')
		neg = -neg;
	if (*str == '-' || *str == '+')
		str++;
	if (!(*str >= '0' && *str <= '9'))
		return (-1);
	while (*str >= '0' && *str <= '9')
	{
		*num = 10 * *num + (*str - '0');
		str++;
	}
	if (*str && !((*str == ' ') || (*str >= 9 && *str <= 13)))
		return (-1);
	*num *= neg;
	return (0);
}

static void handle_print_exit(t_node_content *node)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(node->full_cmd[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
}

// handle the command exit.
// status_arr[0] = status_arr[0] % 256 + 256 * (status_arr[0] < 0); : to handle over flow.
int	handle_exit(t_list *cmd, int *exit_num, t_prompt *prom)
{
	t_node_content	*node;
	int				status_num[2];

	node = cmd->content;
	*exit_num = (!cmd->next && (prom->size == 1));
	if (*exit_num)
		ft_putstr_fd("exit\n", 2);
	if (!node->full_cmd || !node->full_cmd[1])
		return (0);
	status_num[1] = check_number_exit(node->full_cmd[1], &status_num[0]);
	if (status_num[1] == -1)
	{
		handle_print_exit(node);
		return (2);
	}
	else if (node->full_cmd[2])
	{
		*exit_num = 0;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (1);
	}
	status_num[0] = status_num[0] % 256 + 256 * (status_num[0] < 0);
	return (status_num[0]);
}

void	check_cd(char **str[2], t_prompt *prom)
// handle cd.
{
	DIR	*directory;

	directory = NULL;
	if (str[0][1])
		directory = opendir(str[0][1]);
	if (!str[0][1] && str[1][0] && !str[1][0][0])
	{
		g_e_status = 1;
		ft_putstr_fd("minishell: HOME not set\n", 2);
	}
	if (str[1][0] && !str[0][1])
		g_e_status = chdir(str[1][0]) == -1;
	if (str[0][1] && directory && access(str[0][1], F_OK) != -1)
		chdir(str[0][1]);
	else if (str[0][1] && access(str[0][1], F_OK) == -1)
		mini_perror(NDIR, str[0][1], 1, prom);
	else if (str[0][1])
		mini_perror(NOT_DIR, str[0][1], 1, prom);
	if (str[0][1] && directory)
		closedir(directory);
}

void	free_content(void *content)
// free the content of the node(full_cmd, full_path).
{
	t_node_content	*node;

	node = content;
	ft_free_matrix(&node->full_cmd);
	free(node->full_path);
	if (node->in > 0)
		close(node->in);
	if (node->out > 1)
		close(node->out);
	free(node);
}
