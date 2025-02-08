#include "minishell.h"

void skip_s(char *s, int *i)
{
	while (s[*i] && s[*i] == ' ')
	{
		(*i)++;
	}
}

int lex_count_word(char *str, int i[2])
{
	int q[2];

	q[0] = 0;
	q[1] = 0;
	while (str[i[0]] != '\0')
	{
		if ((str[i[0]] != ' '))
		{
			i[1]++;
			while (((str[i[0]] != ' ') || q[0]) && str[i[0]] != '\0')
			{
				if (!q[1] && (str[i[0]] == '\"' || str[i[0]] == '\''))
					q[1] = str[i[0]];
				q[0] = (q[0] + (str[i[0]] == q[1])) % 2; 
				q[1] *= q[0];
				i[0]++;
			}
			if (q[0])
				return (0);
		}
		else
			i[0]++;
	}
	return(i[1]);
}
void	fill_lex_str(t_main *main, int	i[4])
{
	int q[2];
	q[0] = 0;
	q[1] = 0;

	while (main->line[i[0]] != '\0')
	{
		if ((main->line[i[0]] != ' '))
		{
			i[1] = i[0];
			while (((main->line[i[0]] != ' ') || q[0]) && main->line[i[0]] != '\0')
			{
				if (!q[1] && (main->line[i[0]] == '\"' || main->line[i[0]] == '\''))
					q[1] = main->line[i[0]];
				q[0] = (q[0] + (main->line[i[0]] == q[1])) % 2; 
				q[1] *= q[0];
				i[0]++;
			}
			main->lex_str[i[2]] = ft_substr(main->line, i[1], i[0] - i[1]);
			i[2]++;
		}
		else
			i[0]++;
	}
}
int	lexer(t_main *main)
{
	int j[4];
	int c;
	int i[2];

	i[0] = 0;
	i[1] = 0;
	j[0] = 0;
	j[1] = 0;
	j[2] = 0;
	j[3] = 0;
	c = lex_count_word(main->line, i);
	if (!c)
		return (0);
	main->lex_str = malloc(c * sizeof(char *));
	if (!main->lex_str)
		return (0);
	fill_lex_str(main, j);
	return (1);
}
