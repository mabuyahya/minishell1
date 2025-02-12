/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:41:03 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/12 14:50:55 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*mini_getenv(char *var, char **envp, int n)
{
	int	i;
	int	n2;
	char *sub;

	i = 0;
	if (n < 0)
		n = ft_strlen(var);
	while (!ft_strchr(var, '=') && envp && envp[i])
	{
		n2 = n;
		if (n2 < ft_strchr_i(envp[i], '='))
			n2 = ft_strchr_i(envp[i], '=');
		if (!ft_strncmp(envp[i], var, n2))
		{
			sub = ft_substr(envp[i], n2 + 1, ft_strlen(envp[i]));
			if (!sub)
			{
				ft_free_matrix(&envp);
				mini_perror(MEM, NULL, 1);
				exit(1);
			}
			return (sub);
		}
		i++;
	}
	return (NULL);
}

char	**mini_setenv(char *var, char *value, char **envp, int n)
// add to env if not exist, if exist edit it.
{
	int		i;
	char	*new_entry;
	char	*equal_pos;
	char	*add_equal;

	if (n < 0)
		n = ft_strlen(var);
	add_equal = ft_strjoin(var, "=");
	if (!add_equal)
	{
		free(value);
		ft_free_matrix(&envp);
		mini_perror(MEM, NULL, 1);
		exit(1);
	}
	new_entry = ft_strjoin(add_equal, value);
	if (!new_entry)
	{
		free(value);
		ft_free_matrix(&envp);
		mini_perror(MEM, NULL, 1);
		free(new_entry);
		exit(1);
	}
	free(add_equal);
	if (!new_entry)
	    return (envp);
	i = 0;
	while (envp && envp[i])
	{
	    equal_pos = ft_strchr(envp[i], '=');
	    if (equal_pos && !ft_strncmp(envp[i], var, equal_pos - envp[i]))
	    {
			free(envp[i]);
			envp[i] = new_entry;
			return (envp);
		}
		i++;
	}
	envp = ft_extend_matrix(envp, new_entry);
	if (!envp)
	{
		mini_perror(MEM, NULL, 1);
		free(new_entry);
		exit(1);
	}
	free(new_entry);
	return (envp);
}

static int	var_in_envp(char *argv, char **envp, int ij[2])
{
	int	pos;

	ij[1] = 0;
	pos = ft_strchr_i(argv, '=');
	if (pos == -1)
		return (-1);
	while (envp[ij[1]])
	{
		if (!ft_strncmp(envp[ij[1]], argv, pos + 1)) // check if the argument exist in env variable.
			return (1);
		ij[1]++;
	}
	return (0);
}

static t_list	*stop_fill(t_list *cmds, char **args, char **temp)
{
	ft_lstclear(&cmds, free_content);
	ft_free_matrix(&temp);
	ft_free_matrix(&args);
	return (NULL);
}

int	mini_export(t_prompt *prompt, t_list *cmd, char **args)
{
	int		ij[2];
	int		pos;
	char	**argv;

	argv = ((t_mini *)prompt->cmds->content)->full_cmd; // argv = export with a argument.
	if (ft_matrixlen(argv) >= 2)
	{
		ij[0] = 1;
		while (argv[ij[0]]) // ij[0] = 1 , this is the argument.
		{
			pos = var_in_envp(argv[ij[0]], prompt->envp, ij);
			if (pos == 1) // the argument inside the env.
			{
				free(prompt->envp[ij[1]]);
				prompt->envp[ij[1]] = ft_strdup(argv[ij[0]]);
				{
					if (!prompt->envp[ij[1]])
					{
						stop_fill(cmd, args, prompt->envp);
						mini_perror(MEM, NULL, 1);
						exit(1);
					}
				}
			}
			else if (!pos)
			{
				prompt->envp = ft_extend_matrix(prompt->envp, argv[ij[0]]);
				if (!prompt->envp)
				{
					stop_fill(cmd, args, NULL);
					mini_perror(MEM, NULL, 1);
					exit(1);
				}
			}
			ij[0]++;
		}
	}
	return (0);
}

int	mini_unset(t_prompt *prompt, t_list *cmd, char **args)
{
	char	**argv;
	char	*aux;
	int		ij[2];

	ij[0] = 0;
	argv = ((t_mini *)prompt->cmds->content)->full_cmd;
	if (ft_matrixlen(argv) >= 2)
	{
		while (argv[++ij[0]])
		{
			if (argv[ij[0]][ft_strlen(argv[ij[0]]) - 1] != '=')
			{
				aux = ft_strjoin(argv[ij[0]], "=");
				if (!aux)
				{
					stop_fill(cmd, args, prompt->envp);
					mini_perror(MEM, NULL, 1);
					exit(1);
				}
				free(argv[ij[0]]);
				argv[ij[0]] = aux;
			}
			if (var_in_envp(argv[ij[0]], prompt->envp, ij))
			{
				if (!ft_matrix_replace_in(&prompt->envp, NULL, ij[1]))
				{
					stop_fill(cmd, args, prompt->envp);
					mini_perror(MEM, NULL, 1);
					exit(1);
				}
			}
		}
	}
	return (0);
}
