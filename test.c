#include <readline/readline.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int	g_signum;

void	test_handler(int signum)
{
	g_signum = signum;
	write(1, "\n", 1);
	close(0);
}

int main()
{
	signal(SIGINT, test_handler);
	int i = 0;
	while (1)
	{
		i++;
		printf("%d", i);
		char *line = readline("> ");
		if (g_signum == SIGINT)
		{
			dup2(2, 0);
			break;
		}
		if (!line)
		{
			printf("Success\n");
		}
	}
}
