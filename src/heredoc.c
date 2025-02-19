/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/09 10:17:00 by mbueno-g          #+#    #+#             */
/*   Updated: 2025/02/19 13:12:29 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_e_status;


void	change_sigint(int sig)
{
	(void)sig;
	g_e_status = SIGINT;
	close(STDIN_FILENO);

}
char	*get_here_str(char *str[2], size_t len, char *limit, char *warn, t_prompt *prom)
{
	char	*temp;
	int 	messi;

	messi = dup(STDIN_FILENO);
	signal(SIGINT, change_sigint);
	while (g_e_status != 130 && (!str[0] || ft_strncmp(str[0], limit, len)
			|| ft_strlen(limit) != len))
	{
		temp = str[1];
		str[1] = ft_strjoin(str[1], str[0]);
		free(temp);
		free(str[0]);
		str[0] = readline("> ");
		if(g_e_status == SIGINT)
		{
			printf("\n");
			prom->exit_status = 130;
			prom->flag = 1;
			break;
		}
		if (!str[0])
		{
			printf("%s (wanted `%s\')\n", warn, limit);
			break ;
		}
		temp = str[0];
		str[0] = ft_strjoin(str[0], "\n");
		free(temp);
		len = ft_strlen(str[0]) - 1;
	}
	dup2(messi, STDIN_FILENO);
	close(messi);
	free(str[0]);
	rl_replace_line("", 0);
	rl_on_new_line();
	return (str[1]);
}

int	get_here_doc(char *str[2], char *aux[2], t_prompt *prom)
{
	int	fd[2];

	signal(SIGINT,SIG_DFL);
	g_e_status = 0;
	if (pipe(fd) == -1)
	{
		mini_perror(PIPERR, NULL, 1, prom);
		return (-1);
	}
	str[1] = get_here_str(str, 0, aux[0], aux[1], prom);
	write(fd[1], str[1], ft_strlen(str[1]));
	free(str[1]);
	close(fd[1]);
	if (g_e_status == 130)
	{
		signal(SIGINT, &handle_sigint);
		close(fd[0]);
		return (-1);
	}
	signal(SIGINT, &handle_sigint);
	return (fd[0]);
}
