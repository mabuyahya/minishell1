/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:29:06 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/15 17:30:56 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	mini_unset_util(int ij[2], t_prompt *prom, char **args, t_list *cmd)
{
	if (!ft_matrix_replace_in(&prom->envp, NULL, ij[1]))
	{
		stop_fill(cmd, args, prom->envp);
		mini_perror(MEM, NULL, 1);
		exit(1);
	}
}

static void	mini_unset_util_1(t_list *cmd, char **args, t_prompt *prom)
{
	stop_fill(cmd, args, prom->envp);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

int	mini_unset(t_prompt *prom, t_list *cmd, char **args)
{
	char	**argv;
	char	*aux;
	int		ij[2];

	ij[0] = 0;
	argv = ((t_mini *)prom->cmds->content)->full_cmd;
	if (ft_matrixlen(argv) >= 2)
	{
		while (argv[++ij[0]])
		{
			if (argv[ij[0]][ft_strlen(argv[ij[0]]) - 1] != '=')
			{
				aux = ft_strjoin(argv[ij[0]], "=");
				if (!aux)
					mini_unset_util_1(cmd, args, prom);
				free(argv[ij[0]]);
				argv[ij[0]] = aux;
			}
			if (var_in_envp(argv[ij[0]], prom->envp, ij))
				mini_unset_util(ij, prom, args, cmd);
		}
	}
	return (0);
}
