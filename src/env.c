/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuyahy <mabuyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 11:41:03 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/16 11:56:00 by mabuyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int		g_status;

static void	stop_get(char **envp)
{
	ft_free_matrix(&envp);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

char	*mini_getenv(char *var, char **envp, int n)
{
	int		i;
	int		n2;
	char	*sub;

	i = 0;
	
	if (ft_strncmp(var, "?", 1) == 0 && ft_strlen(var) == 1)
		return (ft_itoa(g_status));
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

static void	stop_set(char *value, char **envp, char *new_entry)
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

static char	**set_env_util(t_set_env *set_env, char **envp, char *var)
{
	set_env->equal_pos = ft_strchr(envp[set_env->i], '=');
	if (set_env->equal_pos && !ft_strncmp(envp[set_env->i], var,
			set_env->equal_pos - envp[set_env->i]))
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
	t_set_env	set_env;

	if (n < 0)
		n = ft_strlen(var);
	set_env.add_equal = ft_strjoin(var, "=");
	if (!set_env.add_equal)
		stop_set(value, envp, NULL);
	set_env.new_entry = ft_strjoin(set_env.add_equal, value);
	if (!set_env.new_entry)
		stop_set(value, envp, set_env.add_equal);
	free(set_env.add_equal);
	if (!set_env.new_entry)
		return (envp);
	set_env.i = 0;
	while (envp && envp[set_env.i])
	{
		if (set_env_util(&set_env, envp, var))
			return (envp);
		set_env.i++;
	}
	envp = ft_extend_matrix(envp, set_env.new_entry);
	if (!envp)
		stop_set(NULL, NULL, set_env.new_entry);
	free(set_env.new_entry);
	return (envp);
}
