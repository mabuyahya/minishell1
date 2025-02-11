/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/22 13:40:47 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/11 12:56:15 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_status;

static void	mini_getpid(t_prompt *p)
// get process id to handle $$.
// get parant pid.  = (child - 1).
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
	{
		mini_perror(FORKERR, NULL, 1);
		ft_free_matrix(&p->envp);
		exit(1);
	}
	if (!pid)
	{
		ft_free_matrix(&p->envp);
		exit(1);
	}
	waitpid(pid, NULL, 0);
	p->pid = pid - 1;
}

static t_prompt	init_vars(t_prompt prompt, char *str, char **argv)
// init variables and get the path of the minishell.
// SHILVL : how much shell open.
{
	char	*num;

	(void)(argv);
	str = getcwd(NULL, 0);
	if (!str)
	{
		ft_free_matrix(&prompt.envp);
		ft_putstr_fd("minishell : can not get the path\n", 2);
		exit(1);
	}
	prompt.envp = mini_setenv("PWD", str, prompt.envp, 3);
	if (!prompt.envp)
		exit(1);
	free(str);
	str = mini_getenv("SHLVL", prompt.envp, 5);
	if (!str || ft_atoi(str) <= 0)
	{
		num = ft_strdup("1");
		if (!num)
		{
			mini_perror(MEM, ".", 1);
			ft_free_matrix(&prompt.envp);
			free(str);
			exit(1);
		}
	}
	else
	{
		num = ft_itoa(ft_atoi(str) + 1);
		if (!num)
		{
			mini_perror(MEM, ".", 1);
			ft_free_matrix(&prompt.envp);
			free(str);
			exit(1);
		}
	}
	free(str);
	prompt.envp = mini_setenv("SHLVL", num, prompt.envp, 5);
	if (!prompt.envp)
		exit(1);
	free(num);
	str = mini_getenv("PATH", prompt.envp, 4);
	if (!str)
		prompt.envp = mini_setenv("PATH", 
		"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin", prompt.envp, 4);
	free(str);
	str = mini_getenv("_", prompt.envp, 1);
	if (!str)
		prompt.envp = mini_setenv("_", argv[0], prompt.envp, 1);
	free(str);
	return (prompt);
}

static t_prompt	init_prompt(char **argv, char **envp)
// init env and all the struct.
{
	t_prompt	prompt;
	char		*str;

	str = NULL;
	prompt.cmds = NULL;
	prompt.envp = ft_dup_matrix(envp);
	if (!prompt.envp || !prompt.envp[0])
	{
		mini_perror(MEM, ".", 1);
		exit(1);
	}
	g_status = 0;
	mini_getpid(&prompt);
	prompt = init_vars(prompt, str, argv);
	return (prompt);
}

int	main(int argc, char **argv, char **envp)
{
	char				*str;
	char				*out;
	t_prompt			prompt;

	prompt = init_prompt(argv, envp);
	while (argv && argc)
	{
		signal(SIGINT, handle_sigint);
		signal(SIGQUIT, SIG_IGN);
		str = mini_getprompt(prompt);
		if (str)
			out = readline(str);
		else
			out = readline("guest@minishell $ ");
		free(str);
		if (!check_args(out, &prompt))
			break;
	}
	ft_free_matrix(&prompt.envp);
	clear_history();
	return (g_status);
}
