/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 13:42:33 by bbrock            #+#    #+#             */
/*   Updated: 2021/03/28 14:01:01 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "../inc/philo.h"
#include "../inc/utils.h"
#include "stdlib.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>  
#include <errno.h>
#include <string.h>
int parse_params(int argc, char **argv)
{
    if (argc < ARG_MIN || argc > ARG_MAX)
        return (-1);
    if (!ft_isnumber(argv[count]) || !ft_isnumber(argv[ttd]) || !ft_isnumber(argv[tte]) || !ft_isnumber(argv[tts]) || (argc == ARG_MAX && !ft_isnumber(argv[noe])))
        return (-1);
    g_params.count = ft_atoi(argv[count]);
    g_params.time_to_die = ft_atoi(argv[ttd]);
    g_params.time_to_eat = ft_atoi(argv[tte]);
    g_params.time_to_sleep = ft_atoi(argv[tts]);
    if (argc == ARG_MAX)
        g_params.num_of_eat = ft_atoi(argv[noe]);
    else
        g_params.num_of_eat = -1;
    return (0);
}

int check_philo(int id, unsigned long long time)
{
    sem_wait(g_life_check);
    if (g_philos[id].is_dead)
    {
        // printf("%llu\n", t);
        philo_kill(&(g_philos[id]), time);
        return (1);
    }
    sem_post(g_life_check);
    return (0);
}

int main(int argc, char **argv)
{

    int i;

    if (parse_params(argc, argv) < 0)
    {
        printf("Error: Wrong arguments\n");
        return (1);
    }
    i = 0;
    unsigned long long prev_time;
    prev_time = millis();
    sem_unlink("forks");
    sem_unlink("input");
    sem_unlink("life_check");
    sem_unlink("take");
    g_forks = sem_open("forks", O_CREAT, 0777, g_params.count);
    g_input = sem_open("input", O_CREAT, 0777, 1);
    g_life_check = sem_open("life_check", O_CREAT, 0777, 1);
    g_take = sem_open("take", O_CREAT, 0777, 1);
    while (i < g_params.count)
    {
        philo_init(&(g_philos[i]), i);
        i++;
    }
    while (1)
    {
        i = 0;
        prev_time = millis();
        while (i < g_params.count)
        {
            if (check_philo(i, prev_time))
                return (0);
            if (i + 1 < g_params.count)
                if (check_philo(i + 1, prev_time))
                    return (0);
            if (i + 2 < g_params.count)
                if (check_philo(i + 2, prev_time))
                    return (0);
            if (i + 3 < g_params.count)
                if (check_philo(i + 3, prev_time))
                    return (0);
            if (i + 4 < g_params.count)
                if (check_philo(i + 4, prev_time))
                    return (0);
            i += 5;
        }
        usleep(500);
    }

}