/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 18:13:47 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/19 18:31:16 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*mini_perror(int err_type, char *param, int err, t_prompt *prom)
// print error if exist.
{
	prom->exit_status = err;
	if (err_type == ERR_NODIR)
		ft_putstr_fd("minishell: no such file or directory: ", 2);
	else if (err_type == ERR_QUO)
		ft_putstr_fd("minishell: error while looking for matching quote\n", 2);
	else if (err_type == ALLOC)
		ft_putstr_fd("minishell: no memory left on device\n", 2);
	else if (err_type == IS_DIR)
		ft_putstr_fd("minishell: is a directory: ", 2);
	else if (err_type == NOT_DIR)
		ft_putstr_fd("minishell: not a directory: ", 2);
	else if (err_type == DUP_ERR)
		ft_putstr_fd("minishell: dup2 failed\n", 2);
	else if (err_type == NOT_CMD)
		ft_putstr_fd("minishell: command not found: ", 2);
	else if (err_type == NO_PERM)
		ft_putstr_fd("minishell: permission denied: ", 2);
	else if (err_type == FORK_ERR)
		ft_putstr_fd("minishell: fork failed\n", 2);
	else if (err_type == PIPE_ERR)
		ft_putstr_fd("minishell: error creating pipe\n", 2);
	else if (err_type == SYN_PIP)
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", 2);
	ft_putendl_fd(param, 2);
	return (NULL);
}
