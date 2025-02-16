/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuyahy <mabuyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:21:27 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/16 11:28:10 by mabuyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*stop_fill(t_list *cmds, char **args, char **temp)
{
	ft_lstclear(&cmds, free_content);
	ft_free_matrix(&temp);
	ft_free_matrix(&args);
	return (NULL);
}

int	ft_matrixlen(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

void	ft_free_matrix(char ***str)
{
	int	i;

	i = 0;
	while (str && str[0] && str[0][i])
	{
		free(str[0][i]);
		i++;
	}
	if (str)
	{
		free(str[0]);
		*str = NULL;
	}
}

char	**ft_dup_matrix(char **s_str)
{
	char	**str;
	int		n;
	int		i;

	i = 0;
	n = ft_matrixlen(s_str);
	str = malloc(sizeof(char *) * (n + 1));
	if (!str)
		return (NULL);
	while (s_str[i])
	{
		str[i] = ft_strdup(s_str[i]);
		if (!str[i])
		{
			ft_free_matrix(&str);
			return (NULL);
		}
		i++;
	}
	str[i] = NULL;
	return (str);
}

int	ft_putmatrix_fd(char **m, int nl, int fd)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (m && m[i])
	{
		if (nl)
			count += ft_putendl_fd(m[i], fd);
		else
			count += ft_putstr_fd(m[i], fd);
		i++;
	}
	return (count);
}
