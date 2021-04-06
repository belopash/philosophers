/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 11:45:28 by bbrock            #+#    #+#             */
/*   Updated: 2021/03/28 18:05:01 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include "../inc/utils.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void wait(t_philo *philo, unsigned long long time)
{
    // unsigned long long end;
    // long long delta;

    // end = millis() + time;
    // while (1)
    // {
    //     delta = (end - millis()) * 1000;
    //     if (delta <= 0)
    //         break;
    //     usleep(((delta < 10) ? delta : 10));
    // }
    usleep(time * 1000);
}

static void take(t_philo *philo)
{
    sem_wait(g_take);

    sem_wait(g_forks);

    sem_wait(g_input);
    printf("%llums \x1B[3%dm%d\033[0m has taken a fork\n", millis(), philo->id % 10 + 1, philo->id + 1);
    sem_post(g_input);

    sem_wait(g_forks);

    sem_wait(g_input);
    printf("%llums \x1B[3%dm%d\033[0m has taken a fork\n", millis(), philo->id % 10 + 1, philo->id + 1);
    sem_post(g_input);

    sem_post(g_take);
}

static void drop_forks(t_philo *philo)
{
    sem_post(g_forks);
    sem_post(g_forks);
}

static void ft_sleep(t_philo *philo)
{
    sem_wait(g_input);
    printf("%llums \x1B[3%dm%d\033[0m is sleeping\n", millis(), philo->id % 10 + 1, philo->id + 1);
    sem_post(g_input);

    drop_forks(philo);

    wait(philo, g_params.time_to_sleep);
}

static void eat(t_philo *philo)
{
    t_ms time;

    time = millis();

    sem_wait(g_life_check);
    philo->last_eat_time = time;
    sem_post(g_life_check);

    sem_wait(g_input);
    printf("%llums \x1B[3%dm%d\033[0m is eating (%d)\n", time, philo->id % 10 + 1, philo->id + 1, philo->num_of_eats + 1);
    sem_post(g_input);

    wait(philo, g_params.time_to_eat);
    
    sem_wait(g_life_check);
    philo->num_of_eats++;
    sem_post(g_life_check);
}

static void think(t_philo *philo)
{
    sem_wait(g_input);
    printf("%llums \x1B[3%dm%d\033[0m is thinking\n", millis(), philo->id % 10 + 1, philo->id + 1);
    sem_post(g_input);
}

static void *start(void *philo_t)
{
    t_philo *philo = (t_philo *)philo_t;
    philo->last_eat_time = millis();
    while ((philo->num_of_eats < g_params.num_of_eat || g_params.num_of_eat <= 0))
    {
        think(philo);
        take(philo);
        eat(philo);
        ft_sleep(philo);
    }
}

int philo_init(t_philo *philo, int id)
{
    philo->num_of_eats = 0;
    philo->id = id;
    return pthread_create(&(philo->pthread), NULL, start, philo);
}