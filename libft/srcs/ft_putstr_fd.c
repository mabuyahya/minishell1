/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabuyahy <mabuyahy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 16:48:24 by sbibers           #+#    #+#             */
/*   Updated: 2025/02/10 18:22:18 by mabuyahy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <unistd.h>

int	ft_putstr_fd(char *s, int fd)
{
    if (s != NULL)
        return ((int)write(fd, s, ft_strlen(s)));
    return (0);
}

/*
int	main(void)
{
	int	fd;

    fd = open("ron.txt", O_CREAT | O_WRONLY);
    if (fd == -1)
    {
        printf("%s", "error");
        return (0);
    }
    ft_putstr_fd("salam thaer baybars", fd);
    close(fd);
}
*/