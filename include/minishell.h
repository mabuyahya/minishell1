/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuyahy <mabuyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 19:29:29 by sbibers           #+#    #+#             */
/*   Updated: 2025/01/30 20:06:46 by mabuyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <curses.h>
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define RESET "\033[0m"

typedef struct s_main
{
	char	**envp;
	char	*line;
	char	*prompt;
	char	**lex_str;
}			t_main;

extern int	g_e_status;
void		signal_handlers(void);
void		get_prompt(t_main *main);
char		**ft_realloc(char **str, int old_size, int new_size);
void	free_all_with_exit(t_main *main);
int	lexer(t_main *main);
void	init_vars(t_main *main, char **envp);
void free_array(char **str);
int  handle_input(t_main *main);
#endif
