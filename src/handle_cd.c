/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_cd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 11:14:46 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/19 16:34:37 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int g_e_status;

static void	stop_cd(t_list *cmd, t_prompt *prom, char **args, char ***str)
{
    if (str && str[0] && str[1])
	    ft_free_matrix(str);
	stop_make_node(cmd, args, prom->envp);
	mini_perror(MEM, NULL, 1, prom);
	exit(1);
}

// str[1] = home + pwd + NULL.
static void	get_cd_path(t_list *cmd, t_prompt *prom, char **command,
		char **str[2])
{
	char	*temp;

	temp = getcwd(NULL, 0);
	str[1] = ft_extend_matrix(str[1], temp);
	free(temp);
	if (!str[1])
    {
        ft_free_matrix(&str[0]);
		stop_cd(cmd, prom, command, NULL);
    }
    check_cd(str, prom);
	if (!prom->exit_status)
		prom->envp = set_env_var("OLDPWD", str[1][1], prom, 6);
	temp = getcwd(NULL, 0);
	if (!temp)
	{
		temp = ft_strdup("");
		if (!temp)
			stop_cd(cmd, prom, command, str);
	}
	str[1] = ft_extend_matrix(str[1], temp);
	free(temp);
	if (!str[1])
		stop_cd(cmd, prom, command, str);
	prom->envp = set_env_var("PWD", str[1][2], prom, 3);
}

// handle command cd.
// str[0] = all the command, str[1] = the home + NULL.
int	handle_cd(t_prompt *prom, t_list *cmd, char **command)
{
	char	**str[2];
	char	*temp;

	prom->exit_status = 0;
	str[0] = ((t_node_content *)prom->cmds->content)->full_cmd;
	temp = get_env_var("HOME", prom->envp, 4, prom);
	if (!temp)
	{
		temp = ft_strdup("");
		if (!temp)
        {
            ft_free_matrix(&str[0]);
			stop_cd(cmd, prom, command, NULL);
	    }
    }
	str[1] = ft_extend_matrix(NULL, temp);
	free(temp);
	if (!str[1])
    {
		stop_cd(cmd, prom, command, NULL);
        ft_free_matrix(&str[0]);
    }
    get_cd_path(cmd, prom, command, str);
	ft_free_matrix(&str[1]);
	return (prom->exit_status);
}
