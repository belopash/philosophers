/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 13:42:33 by bbrock            #+#    #+#             */
/*   Updated: 2021/04/06 20:33:46 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "../inc/philo.h"
#include "../inc/utils.h"
#include "stdlib.h"
#include <pthread.h>
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

void ft_log(int id, int action, t_ms time)
{
    pthread_mutex_lock(&g_input);
    if (action == a_eat)
        printf("%lums \x1B[3%dm%d\033[0m is eating\n", time, id % 10 + 1, id + 1);
    else if (action == a_think)
        printf("%lums \x1B[3%dm%d\033[0m is thinking\n", time, id % 10 + 1, id + 1);
    else if (action == a_sleep)
        printf("%lums \x1B[3%dm%d\033[0m is sleeping\n", time, id % 10 + 1, id + 1);
    else if (action == a_take)
        printf("%lums \x1B[3%dm%d\033[0m has taken a fork\n", time, id % 10 + 1, id + 1);
    else if (action == a_die)
    {
        printf("%lums \x1B[3%dm%d\033[0m died\n", time, id % 10 + 1, id + 1);
        return;
    }
    pthread_mutex_unlock(&g_input);
}

void kill_philo(int id)
{
    ft_log(id, a_die, millis());
}

int check_philo(int id)
{
    t_ms time;

    if (g_philos[id].finished)
        return (0);
    pthread_mutex_lock(&(g_philos[id].life_check));
    time = millis();
    if (g_philos[id].num_of_eats == g_params.num_of_eat)
    {
        g_philos[id].finished = 1;
        g_finished_philos++;
    }
    else if (time >= g_philos[id].last_eat_time + g_params.time_to_die && g_philos[id].finished < 2)
    {
        kill_philo(id);
        return (1);
    }
    pthread_mutex_unlock(&(g_philos[id].life_check));
    return (0);
}

int monitor()
{
    int i;

    while (g_finished_philos < g_params.count)
    {
        i = 0;
        while (i < g_params.count)
        {
            if (check_philo(i))
                return (0);
            if (i + 1 < g_params.count)
                if (check_philo(i + 1))
                    return (0);
            if (i + 2 < g_params.count)
                if (check_philo(i + 2))
                    return (0);
            if (i + 3 < g_params.count)
                if (check_philo(i + 3))
                    return (0);
            if (i + 4 < g_params.count)
                if (check_philo(i + 4))
                    return (0);
            i += 5;
        }
        usleep(1000);
    }
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
    pthread_mutex_init(&g_input, NULL);
    pthread_mutex_init(&g_take, NULL);
    while (i < g_params.count)
    {
        pthread_mutex_init(&(g_forks[i].mutex), NULL);
        g_forks[i].is_free = 1;
        i++;
    }
    i = 0;
    while (i < g_params.count)
    {
        philo_init(&(g_philos[i]), i);
        i += 1;
    }
    // i = 1;
    // usleep(1000);

    // while (i < g_params.count)
    // {
    //     philo_init(&(g_philos[i]), i);
    //     i += 2;
    // }
    return monitor();
}
