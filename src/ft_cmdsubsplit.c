/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cmdsubsplit.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: salam <salam@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/09/27 18:23:56 by aperez-b          #+#    #+#             */
/*   Updated: 2025/02/13 17:24:59 by salam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int ft_count_words(char *s, char *set, int count)
{
    int i = 0;
    int q[2] = {0, 0};

    while (s && s[i] != '\0')
    {
        count++;
        if (!ft_strchr(set, s[i])) 
        {
            while ((!ft_strchr(set, s[i]) || q[0] || q[1]) && s[i] != '\0')
            {
                if (s[i] == '\'' && !q[1])
					q[0] = !q[0];
                if (s[i] == '\"' && !q[0])
					q[1] = !q[1];
                i++;
            }
            if (q[0] || q[1])
				return (-1); 
        }
        else
            i++;
    }
    return count;
}

static char **ft_fill_array(char **aux, char *s, char *set, int i[3])
{
    int q[2] = {0, 0};

    while (s && s[i[0]] != '\0')
    {
        i[1] = i[0];
        if (!ft_strchr(set, s[i[0]]))
		{
            while (s[i[0]] && (!ft_strchr(set, s[i[0]]) || q[0] || q[1]))
            {
                if (s[i[0]] == '\'' && !q[1]) 
					q[0] = !q[0];
                if (s[i[0]] == '\"' && !q[0])
					q[1] = !q[1];
                i[0]++;
            }
        }
        else
            i[0]++;
        aux[i[2]++] = ft_substr(s, i[1], i[0] - i[1]);
	}
    return aux;
}

char **ft_cmdsubsplit(char const *s, char *set)
{
    char **words;
    int count_word;
    int i[3] = {0, 0, 0};

    if (!s)
		return (NULL);
    count_word = ft_count_words((char *)s, set, 0);
    if (count_word == -1)
		return (NULL);
    words = malloc((count_word + 1) * sizeof(char *));
    if (!words)
		return (NULL);
    words = ft_fill_array(words, (char *)s, set, i);
    words[count_word] = NULL;
    return (words);
}
