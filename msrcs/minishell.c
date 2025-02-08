#include "minishell.h"

int	g_e_status = 0;

int	main(int argc, char **argv, char **envp)
{
	t_main	main;
	int test;

	(void)argc;
	(void)argv;
	signal_handlers();
	init_vars(&main, envp);
	while (1)
	{
		main.line = readline(main.prompt);
		if (!main.line)
		{
			break;
		}
		else
		{
		test = handle_input(&main);
		if (test == 0)
			printf("hi");
			else
			printf("-");
			free_array(main.lex_str);
			add_history(main.line);
		free(main.line);
		}
	}
	// free_all_with_exit(&main);
	free(main.prompt);
	return (0);
}
