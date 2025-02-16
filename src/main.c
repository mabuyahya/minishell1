/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuyahy <mabuyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 11:26:13 by mabuyahy          #+#    #+#             */
/*   Updated: 2025/02/16 13:27:45 by mabuyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

void	fail_allocate(t_prompt *prom, int flag)
{
	ft_free_matrix(&prom->envp);
	mini_perror(flag, NULL, 1);
	exit(1);
}

// init variables and get the path of the minishell.
// SHILVL : how much shell open.
static void	init_vars(t_prompt *prom, char *str, char **argv)
{
	char	*sstr;

	(void)(argv);
	str = getcwd(NULL, 0);
	prom->envp = mini_setenv("PWD", str, prom->envp, 3);
	free(str);
	str = mini_getenv("SHLVL", prom->envp, 5);
	if (!str || ft_atoi(str) <= 0)
		sstr = ft_strdup("1");
	else
		sstr = ft_itoa(ft_atoi(str) + 1);
	free(str);
	if (!sstr)
		fail_allocate(prom, MEM);
	prom->envp = mini_setenv("SHLVL", sstr, prom->envp, 5);
	free(sstr);
	str = mini_getenv("PATH", prom->envp, 4);
	if (!str)
		prom->envp = mini_setenv("PATH",
				"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin",
				prom->envp, 4);
	free(str);
}

// init env and all the struct.
static void	init_prompt(t_prompt *prom, char **argv, char **envp)
{
	char	*str;

	str = NULL;
	prom->cmds = NULL;
	prom->envp = NULL;
	prom->envp = ft_dup_matrix(envp);
	if (!prom->envp)
	{
		mini_perror(MEM, NULL, 1);
		exit(1);
	}
	g_status = 0;
	init_vars(prom, str, argv);
}

int	main(int argc, char **argv, char **envp)
{
	char		*read;
	t_prompt	prom;

	(void)(argv);
	if (!isatty(0) || !isatty(1) || !isatty(2))
		return (1);
	init_prompt(&prom, argv, envp);
	while (argc)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		read = readline("sbibers $>");
		if (!check_args(read, &prom))
		{
			ft_free_matrix(&prom.envp);
			clear_history();
			exit(g_status);
		}
	}
	ft_free_matrix(&prom.envp);
	clear_history();
	return (g_status);
}
