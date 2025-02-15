/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_things.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:33:31 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/15 17:33:31 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_matrixlen(char **m)
{
	int	i;

	i = 0;
	while (m && m[i])
		i++;
	return (i);
}

int	ft_extend_matrix_utils(int *i, char **in, char **out)
{
	out[*i] = ft_strdup(in[*i]);
	if (!out[*i])
	{
		ft_free_matrix(&in);
		ft_free_matrix(&out);
		return (0);
	}
	return (1);
}

char	**ft_extend_matrix(char **in, char *newstr)
{
	char	**out;
	int		len;
	int		i;

	i = -1;
	out = NULL;
	if (!newstr)
		return (in);
	len = ft_matrixlen(in);
	out = malloc(sizeof(char *) * (len + 2));
	if (!out)
		return (NULL);
	out[len + 1] = NULL;
	while (++i < len)
		if (!ft_extend_matrix_utils(&i, in, out))
			return (NULL);
	out[i] = ft_strdup(newstr);
	if (!out[i])
	{
		ft_free_matrix(&in);
		return (NULL);
	}
	ft_free_matrix(&in);
	return (out);
}

void	ft_free_matrix(char ***m)
{
	int	i;

	i = 0;
	while (m && m[0] && m[0][i])
	{
		free(m[0][i]);
		i++;
	}
	if (m)
	{
		free(m[0]);
		*m = NULL;
	}
}

char	**ft_dup_matrix(char **m)
{
	char	**str;
	int		n;
	int		i;

	i = 0;
	n = ft_matrixlen(m);
	str = malloc(sizeof(char *) * (n + 1));
	if (!str)
		return (NULL);
	while (m[i])
	{
		str[i] = ft_strdup(m[i]);
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

static char	**ft_copy_big_row(char **aux, char **big, int *i, int n)
{
	if (i[0] != n)
	{
		aux[++i[2]] = ft_strdup(big[i[0]]);
		if (!aux[i[2]])
		{
			ft_free_matrix(&aux);
			return (NULL);
		}
	}
	return (aux);
}

static char	**ft_insert_small_rows(char **aux, char **small, int *i)
{
	while (small && small[++i[1]])
	{
		aux[++i[2]] = ft_strdup(small[i[1]]);
		if (!aux[i[2]])
		{
			ft_free_matrix(&aux);
			return (NULL);
		}
	}
	return (aux);
}

char	**ft_matrix_replace_in(char ***big, char **small, int n)
{
	char	**aux;
	int		i[3];

	i[0] = -1;
	i[1] = -1;
	i[2] = -1;
	if (!big || !*big || n < 0 || n >= ft_matrixlen(*big))
		return (NULL);
	aux = ft_calloc(ft_matrixlen(*big) + ft_matrixlen(small), sizeof(char *));
	if (!aux)
		return (NULL);
	while (aux && (*big)[++i[0]])
	{
		if (i[0] != n)
			aux = ft_copy_big_row(aux, *big, i, n);
		else
			aux = ft_insert_small_rows(aux, small, i);
		if (!aux)
			return (NULL);
	}
	ft_free_matrix(big);
	*big = aux;
	return (*big);
}
