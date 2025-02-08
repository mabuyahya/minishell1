#include "minishell.h"

void free_array(char **str)
{
	int i;

	i = 0;
	while (str[i + 1] != NULL)
	{
		free(str[i]);
		i++;
	}
	free(str);
}

void free_all_with_exit(t_main *main)
{ 
	if (!main->prompt)
		free(main->prompt);
	// if (!main->envp)
	// 	free_array(main->envp);
	// if (!main->lex_str)
	// 	free_array(main->lex_str);
}
void free_array_i(char **str, int len)
{
	int i;

	i = 0;
	while (str[i] && i < len)
		free(str[i]);
	free(str);
}