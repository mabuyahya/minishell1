/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_params.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/04 19:48:14 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/18 21:00:21 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_e_status;

// if (flags[0] && flags[1]) // >>
// else if (flags[0] && !flags[1]) // >
// flags[0] = 1 write, 0 read.
// flags[1] = 1 append, 0 trunc.
static int	get_fd(int old_file, char *path, int flags[2], t_prompt *prom)
{
	int	fd;

	if (old_file > 2)
		close(old_file);
	if (!path)
		return (-1);
	if (access(path, F_OK) == -1 && !flags[0])
		mini_perror(NDIR, path, 127, prom);
	else if (!flags[0] && access(path, R_OK) == -1)
		mini_perror(NPERM, path, 126, prom);
	else if (flags[0] && access(path, W_OK) == -1 && access(path, F_OK) == 0)
		mini_perror(NPERM, path, 126, prom);
	if (flags[0] && flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0666);
	else if (flags[0] && !flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0666);
	else if (!flags[0] && old_file != -1)
		fd = open(path, O_RDONLY);
	else
		fd = old_file;
	return (fd);
}

// >
t_node_content	*get_outfile1(t_node_content *node, char **args, int *i, t_prompt *prom)
{
	char	*error;
	int		flags[2];

	flags[0] = 1;
	flags[1] = 0;
	error = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[*i])
		node->outfile = get_fd(node->outfile, args[*i], flags, prom);
	if (!args[*i] || node->outfile == -1)
	{
		*i = -1;
		if (node->outfile != -1)
		{
			ft_putendl_fd(error, 2);
			prom->exit_status = 2;
		}
		else
			prom->exit_status = 1;
	}
	return (node);
}

// args  with expanding.
// node = content.
// >>
// flags[0] = 1 write, 0 read.
// flags[1] = 1 append, 0 trunc.
t_node_content	*out_redirction_double(t_node_content *node, char **args, int *i, t_prompt *prom)
{
	char	*error;
	int		flags[2];

	flags[0] = 1;
	flags[1] = 1;
	error = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[++(*i)])
		node->outfile = get_fd(node->outfile, args[*i], flags, prom);
	if (!args[*i] || node->outfile == -1)
	{
		*i = -1;
		if (node->outfile != -1)
		{
			ft_putendl_fd(error, 2);
			prom->exit_status = 2;
		}
		else
			prom->exit_status = 1;
	}
	return (node);
}

// for infile.
t_node_content	*get_infile1(t_node_content *node, char **args, int *i, t_prompt *promt)
{
	char	*error;
	int		flags[2];

	flags[0] = 0;
	flags[1] = 0;
	error = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[*i])
		node->infile = get_fd(node->infile, args[*i], flags, promt);
	if (!args[*i] || node->infile == -1)
	{
		*i = -1;
		if (node->infile != -1)
		{
			ft_putendl_fd(error, 2);
			promt->exit_status = 2;
		}
		else
			promt->exit_status = 1;
	}
	return (node);
}

// for heardoc.
t_node_content	*get_infile2(t_node_content *node, char **args, int *i, t_prompt *prom)
{
	char	*aux[2];
	char	*error;
	char	*str[2];

	str[0] = NULL;
	str[1] = NULL;
	aux[0] = NULL;
	aux[1] = "minishell: warning: here-document delimited by end-of-file";
	error = "minishell: syntax error near unexpected token `newline'";
	(*i)++;
	if (args[++(*i)])
	{
		aux[0] = args[*i];
		node->infile = get_here_doc(str, aux, prom);
	}
	if (!args[*i] || node->infile == -1)
	{
		*i = -1;
		if (node->infile != -1)
		{
			ft_putendl_fd(error, 2);
			prom->exit_status = 2;
		}
	}
	return (node);
}
