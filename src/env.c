/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuyahy <mabuyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:41:03 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/15 14:09:11 by mabuyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void stop_get(char **envp)
{
	ft_free_matrix(&envp);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

char	*mini_getenv(char *var, char **envp, int n)
{
	int	i;
	int	n2;
	char *sub;

	i = 0;
	if (ft_strncmp(var, "?", 1) == 0 && ft_strlen(var) == 1)
    {
        return (ft_itoa(g_status));
    }
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
				stop_get(envp);
			return (sub);
		}
		i++;
	}
	return (NULL);
}

static void stop_set(char *value, char **envp, char *new_entry)
{
	if (envp && envp[0])
		ft_free_matrix(&envp);
	if (value)
		free(value);
	if (new_entry)
		free(new_entry);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

static char **set_env_util(t_set_env *set_env, char **envp, char *var)
{
	set_env->equal_pos = ft_strchr(envp[set_env->i], '=');
	if (set_env->equal_pos && !ft_strncmp(envp[set_env->i], var, set_env->equal_pos - envp[set_env->i]))
	{
		free(envp[set_env->i]);
		envp[set_env->i] = set_env->new_entry;
		return (envp);
	}
	return (NULL);
}

// add to env if not exist, if exist edit it.
char	**mini_setenv(char *var, char *value, char **envp, int n)
{
	t_set_env set_env;

	if (n < 0)
		n = ft_strlen(var);
	set_env.add_equal = ft_strjoin(var, "=");
	if (!set_env.add_equal)
		stop_set(value, envp, NULL);
	set_env.new_entry = ft_strjoin(set_env.add_equal, value);
	if (!set_env.new_entry)
		stop_set(value ,envp, set_env.add_equal);
	free(set_env.add_equal);
	if (!set_env.new_entry)
	    return (envp);
	set_env.i = 0;
	while (envp && envp[set_env.i])
	{
		if (set_env_util(&set_env, envp, var))
			return(envp);
		set_env.i++;
	}
	envp = ft_extend_matrix(envp, set_env.new_entry);
	if (!envp)
		stop_set(NULL, NULL, set_env.new_entry);
	free(set_env.new_entry);
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

static void mini_unset_util(int ij[2], t_prompt *prom, char **args, t_list *cmd)
{
	if (!ft_matrix_replace_in(&prom->envp, NULL, ij[1]))
	{
		stop_fill(cmd, args, prom->envp);
		mini_perror(MEM, NULL, 1);
		exit(1);
	}
}

static void mini_unset_util_1(t_list *cmd, char **args, t_prompt *prom)
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
