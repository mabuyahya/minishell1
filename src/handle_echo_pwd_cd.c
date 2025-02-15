/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_echo_pwd_cd.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:15:34 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/15 18:44:27 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

int	mini_echo(t_list *cmd)
// handle echo.
{
	int		newline;
	int		i[2];
	char	**argv;
	t_mini	*node;

	i[0] = 0;
	i[1] = 0;
	newline = 1;
	node = cmd->content;
	argv = node->full_cmd;
	while (argv && argv[++i[0]])
	{
		if (!i[1] && !ft_strncmp(argv[i[0]], "-n", 2)
			&& (ft_countchar(argv[i[0]], 'n')
				== (int)(ft_strlen(argv[i[0]]) - 1)))
			newline = 0;
		else
		{
			i[1] = 1;
			ft_putstr_fd(argv[i[0]], 1);
			if (argv[i[0] + 1])
				ft_putchar_fd(' ', 1);
		}
	}
	return (write(1, "\n", newline) == 2);
}

int	mini_pwd(void)
// handle pwd.
{
	char	*buf;

	buf = getcwd(NULL, 0);
	ft_putendl_fd(buf, 1);
	free(buf);
	return (0);
}

static void	stop_solution(t_list *cmd, t_prompt *prom, char **args, char ***str)
{
	ft_free_matrix(&str[0]);
	stop_fill(cmd, args, prom->envp);
	mini_perror(MEM, NULL, 1);
	exit(1);
}

static void	handle_cd_paths(t_list *cmd, t_prompt *p, char **args,
		char **str[2])
{
	char	*aux;

	aux = getcwd(NULL, 0);
	str[1] = ft_extend_matrix(str[1], aux);
	free(aux);
	if (!str[1])
		stop_solution(cmd, p, args, str);
	cd_error(str);
	if (!g_status)
		p->envp = mini_setenv("OLDPWD", str[1][1], p->envp, 6);
	aux = getcwd(NULL, 0);
	if (!aux)
	{
		aux = ft_strdup("");
		if (!aux)
			stop_solution(cmd, p, args, str);
	}
	str[1] = ft_extend_matrix(str[1], aux);
	free(aux);
	if (!str[1])
		stop_solution(cmd, p, args, str);
	p->envp = mini_setenv("PWD", str[1][2], p->envp, 3);
}

int	mini_cd(t_prompt *prom, t_list *cmd, char **args)
// handle command cd.
{
	char	**str[2];
	char	*aux;

	g_status = 0;
	str[0] = ((t_mini *)prom->cmds->content)->full_cmd;
	aux = mini_getenv("HOME", prom->envp, 4);
	if (!aux)
	{
		aux = ft_strdup("");
		if (!aux)
			stop_solution(cmd, prom, args, str);
	}
	str[1] = ft_extend_matrix(NULL, aux);
	free(aux);
	if (!str[1])
		stop_solution(cmd, prom, args, str);
	handle_cd_paths(cmd, prom, args, str);
	ft_free_matrix(&str[1]);
	return (g_status);
}
