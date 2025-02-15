/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/15 16:50:52 by sbibers          ###   ########.fr       */
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

static void	mini_getpid(t_prompt *prom)
// get process id to handle $$.
// get parant pid.  = (child - 1).
{
	pid_t pid;

	pid = fork();
	if (pid < 0)
	{
		mini_perror(FORKERR, NULL, 1);
		ft_free_matrix(&prom->envp);
		exit(1);
	}
	if (!pid)
	{
		ft_free_matrix(&prom->envp);
		exit(1);
	}
	waitpid(pid, NULL, 0);
	prom->pid = pid - 1;
}

static void	init_vars(t_prompt *prom, char *str, char **argv)
// init variables and get the path of the minishell.
// SHILVL : how much shell open.
{
	char *sstr;

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
	str = mini_getenv("_", prom->envp, 1);
	if (!str)
		prom->envp = mini_setenv("_", argv[0], prom->envp, 1);
	free(str);
}

static void	init_prompt(t_prompt *prom, char **argv, char **envp)
// init env and all the struct.
{
	char *str;

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
	mini_getpid(prom);
	init_vars(prom, str, argv);
}

int	main(int argc, char **argv, char **envp)
{
	char		*read;
	t_prompt	prom;

	(void)(argv);
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
