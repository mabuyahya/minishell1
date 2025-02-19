/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/19 13:16:15 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_e_status;

void	fail_allocate(t_prompt *prom, int flag)
{
	ft_free_matrix(&prom->envp);
	mini_perror(flag, NULL, 1, prom);
	exit(1);
}

// init variables and get the path of the minishell.
// SHILVL : how much shell open.
static void	init_vars(t_prompt *prom, char *str, char **argv)
{
	char	*sstr;

	// (void)(argv);
	str = getcwd(NULL, 0);
	prom->envp = set_env_var("PWD", str, prom, 3);
	free(str);
	str = get_env_var("SHLVL", prom->envp, 5, prom);
	if (!str || ft_atoi(str) <= 0)
		sstr = ft_strdup("1");
	else
		sstr = ft_itoa(ft_atoi(str) + 1);
	free(str);
	if (!sstr)
		fail_allocate(prom, MEM);
	prom->envp = set_env_var("SHLVL", sstr, prom, 5);
	free(sstr);
	str = get_env_var("PATH", prom->envp, 4, prom);
	if (!str)
		prom->envp = set_env_var("PATH",
				"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin",
				prom, 4);
	free(str);
	str = get_env_var("_", prom->envp, 1, prom);
	if (!str)
		prom->envp = set_env_var("_", argv[0], prom, 1);
	free(str);
}

// init env and all the struct.
static void	init_prompt(t_prompt *prom, char **argv, char **envp)
{
	char	*str;

	str = NULL;
	prom->cmds = NULL;
	prom->envp = NULL;
	prom->size = 0;
	prom->envp = ft_dup_matrix(envp);
	if (!prom->envp)
	{
		mini_perror(MEM, NULL, 1, prom);
		exit(1);
	}
	prom->exit_status = 0;
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
		prom.flag = 0;
		g_e_status = 0;
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		read = readline("⚽️⚽️⚽️⚽️⚽️⚽️⚽️⚽️⚽️⚽️⚽️ > ");
		if (g_e_status == SIGINT)
			prom.exit_status = 130;
		if (!check_args(read, &prom))
		{
			ft_free_matrix(&prom.envp);
			clear_history();
			exit(prom.exit_status);
		}
	}
	ft_free_matrix(&prom.envp);
	clear_history();
	return (prom.exit_status);
}
