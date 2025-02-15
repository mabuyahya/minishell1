/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:31:29 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/15 17:31:45 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	var_in_envp(char *argv, char **envp, int ij[2])
{
	int	pos;

	ij[1] = 0;
	pos = ft_strchr_i(argv, '=');
	if (pos == -1)
		return (-1);
	while (envp[ij[1]])
	{
		if (!ft_strncmp(envp[ij[1]], argv, pos + 1))
		// check if the argument exist in env variable.
			return (1);
		ij[1]++;
	}
	return (0);
}

static void	print_envp(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n", envp[i]);
		i++;
	}
}

static void	update_env_var(t_prompt *prom, t_list *cmd, char **args, char *arg)
{
	int	ij[2];
	int	pos;

	pos = var_in_envp(arg, prom->envp, ij);
	if (pos == 1)
	{
		free(prom->envp[ij[1]]);
		prom->envp[ij[1]] = ft_strdup(arg);
		if (!prom->envp[ij[1]])
		{
			stop_fill(cmd, args, prom->envp);
			mini_perror(MEM, NULL, 1);
			exit(1);
		}
	}
	else if (!pos)
	{
		prom->envp = ft_extend_matrix(prom->envp, arg);
		if (!prom->envp)
		{
			stop_fill(cmd, args, NULL);
			mini_perror(MEM, NULL, 1);
			exit(1);
		}
	}
}

int	mini_export(t_prompt *prompt, t_list *cmd, char **args)
{
	char	**argv;
	int		i;

	argv = ((t_mini *)prompt->cmds->content)->full_cmd;
	if (ft_strncmp(argv[0], "export", 6) == 0 && !argv[1])
		print_envp(prompt->envp);
	else
	{
		i = 1;
		while (argv[i])
		{
			update_env_var(prompt, cmd, args, argv[i]);
			i++;
		}
	}
	return (0);
}
