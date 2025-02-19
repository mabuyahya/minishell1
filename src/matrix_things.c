/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matrix_things.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbibers <sbibers@student.42amman.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 17:33:31 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/19 12:35:03 by sbibers          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_extend_matrix_utils(int *i, char **in, char **out)
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
	char	**str;
	int		len;
	int		i;

	i = -1;
	str = NULL;
	if (!newstr)
		return (in);
	len = ft_matrixlen(in);
	str = malloc(sizeof(char *) * (len + 2));
	if (!str)
		return (NULL);
	str[len + 1] = NULL;
	while (++i < len)
	{
		if (!ft_extend_matrix_utils(&i, in, str))
			return (NULL);
	}
	str[i] = ft_strdup(newstr);
	if (!str[i])
	{
		ft_free_matrix(&in);
		return (NULL);
	}
	ft_free_matrix(&in);
	return (str);
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
