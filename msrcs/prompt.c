#include "minishell.h"

// void	init_envp(t_main *main, char **envp)
// {
// 	int	i;

// 	i = 0;
// 	while (envp[i])
// 		i++;
// 	main->envp = (char **)malloc(sizeof(char *) * (i + 1));
// 	if (!main->envp)
// 	{
// 		perror("malloc failed");
// 		free_all_with_exit(main);
// 	}
// 	i = 0;
// 	while (envp[i])
// 	{
// 		main->envp[i] = strdup(envp[i]); 
// 		if (!main->envp[i])
// 		{
// 			perror("strdup failed");
// 			free_all_with_exit(main);
// 		}
// 		i++;
// 	}
// 	main->envp[i] = NULL;
// }

void	init_vars(t_main *main, char **envp)
{
	(void) envp;
	// init_envp(main, envp);
	get_prompt(main);
}

void	get_prompt(t_main *main)
{
	char	*user;
	char	*cwd;
	char	*user_with;
	char	*cwd_with;
	char	*temp;

	// if (!main->prompt)
	// 	free(main->prompt);
	user = getenv("USER");
	cwd = getcwd(NULL, 0);
	user_with = ft_strjoin(GREEN, user);
	temp = user_with;
	user_with = ft_strjoin(user_with, GREEN "@minishell ");
	free(temp);
	cwd_with = ft_strjoin(YELLOW, cwd);
	temp = cwd_with;
	cwd_with = ft_strjoin(cwd_with, GREEN " $>");
	free(temp);
	main->prompt = ft_strjoin(user_with, cwd_with);
	free(user_with);
	free(cwd_with);
	temp = main->prompt;
	main->prompt = ft_strjoin(main->prompt, RESET);
	free(temp);
	free(cwd);
}
