/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/08 14:33:18 by bbrock            #+#    #+#             */
/*   Updated: 2021/01/27 23:22:43 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <stdio.h>

static int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

static	int	ctoi(const char c)
{
	return (ft_isdigit(c)) ? (c - '0') : 0;
}

int			ft_atoi(const char *str)
{
	int			sign;
	long long	res;

	res = 0;
	sign = 1;
	while (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r'
			|| *str == '\v' || *str == '\f')
		str++;
	if (*str == '-' || *str == '+')
		sign = (*str++ == '-') ? -1 : 1;
	while (ft_isdigit(*str))
	{
		res = (res == 0) ? sign * ctoi(*str) : res * 10 + sign * ctoi(*str);
		if (res % 10 != sign * ctoi(*str) % 10)
			return ((sign < 0) ? 0 : -1);
		str++;
	}
	return (res);
}

int ft_isnumber(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (i && 1);
}

unsigned long long millis()
{
    struct timeval current_time;
    static unsigned long long start = 0;

    if (start == 0)
    {
        gettimeofday(&current_time, 0);
        start = ((current_time.tv_sec) * 1000 + (current_time.tv_usec) / 1000);
    }
    gettimeofday(&current_time, 0);
    return (((current_time.tv_sec) * 1000 + (current_time.tv_usec) / 1000) - start);
}