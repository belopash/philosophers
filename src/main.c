/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 13:42:33 by bbrock            #+#    #+#             */
/*   Updated: 2021/01/27 23:18:56 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "../inc/philo.h"
#include "../inc/utils.h"
#include "stdlib.h"
#include <pthread.h>

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
    pthread_mutex_init(&(g_params.input), NULL);
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
    while (i < g_params.count)
    {
        pthread_mutex_init(&(g_forks[i]), NULL);
        i++;
    }
    i = 0;
    while (i < g_params.count)
    {
        philo_init(&(g_philos[i]), i);
        i++;
    }
    i = 0;
    while (i < g_params.count)
    {
        i++;
    }
    unsigned long long delta;

    delta = millis() - prev_time;
    int a = g_params.count / 2 + (g_params.count % 2) ? 1 : 0;
    while (1)
    {
        i = 0;
        while (i < a)
        {
            pthread_mutex_lock(&(g_philos[i].life_timer_mutex));
            if (g_philos[i].life_timer >= g_params.time_to_die)
            {
                printf("%llu\n", g_philos[i].life_timer);
                philo_destroy(&(g_philos[i]));
                return (0);
            }
            g_philos[i].life_timer += delta;
            pthread_mutex_unlock(&(g_philos[i].life_timer_mutex));
            if (i != g_params.count - i - 1)
            {
                pthread_mutex_lock(&(g_philos[g_params.count - i - 1].life_timer_mutex));
                if (g_philos[g_params.count - i - 1].life_timer >= g_params.time_to_die)
                {
                    philo_destroy(&(g_philos[g_params.count - i - 1]));
                    return (0);
                }
                g_philos[g_params.count - i - 1].life_timer += delta;
                pthread_mutex_unlock(&(g_philos[g_params.count - i - 1].life_timer_mutex));
            }
            i++;
        }
        delta = millis() - prev_time;
        prev_time = millis();
        usleep(1000);
    }
}