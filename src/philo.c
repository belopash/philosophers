/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 11:45:28 by bbrock            #+#    #+#             */
/*   Updated: 2021/01/27 23:07:52 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include "../inc/utils.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

typedef unsigned long long time_ms;

static void take(t_philo *philo, pthread_mutex_t *fork)
{
    int ret;
    time_ms time;

    pthread_mutex_lock(fork);
    time = millis();
    philo->forks_count += 1;
    pthread_mutex_lock(&(g_params.input));
    printf("%llums \x1B[3%dm%d\033[0m has taken a fork (%d/2)\n", time, philo->id % 10, philo->id + 1, philo->forks_count);
    pthread_mutex_unlock(&(g_params.input));
}

static void drop_forks(t_philo *philo)
{
    if (philo->id == g_params.count - 1)
    {
        pthread_mutex_unlock(&(g_forks[0]));
        pthread_mutex_unlock(&(g_forks[philo->id]));
    }
    else
    {
        pthread_mutex_unlock(&(g_forks[philo->id]));
        pthread_mutex_unlock(&(g_forks[philo->id + 1]));
    }
    philo->forks_count = 0;
}

static void ft_sleep(t_philo *philo)
{
    time_ms time;

    time = millis();
    drop_forks(philo);
    pthread_mutex_lock(&(g_params.input));
    printf("%llums \x1B[3%dm%d\033[0m is sleeping\n", time, philo->id % 10, philo->id + 1);
    pthread_mutex_unlock(&(g_params.input));
    usleep(g_params.time_to_sleep * 1000);
}

static void eat(t_philo *philo)
{
    time_ms time;

    time = millis();
    pthread_mutex_lock(&(philo->life_timer_mutex));
    philo->life_timer = 0;
    pthread_mutex_unlock(&(philo->life_timer_mutex));
    philo->num_of_eats++;
    pthread_mutex_lock(&(g_params.input));
    printf("%llums \x1B[3%dm%d\033[0m is eating (%d)\n", time, philo->id % 10, philo->id + 1, philo->num_of_eats);
    pthread_mutex_unlock(&(g_params.input));
    usleep(g_params.time_to_eat * 1000);
}

static void die(t_philo *philo)
{
    time_ms time;
    
    time = millis();
    drop_forks(philo);
    pthread_mutex_lock(&(g_params.input));
    printf("%llums \x1B[3%dm%d\033[0m died\n", time, philo->id % 10, philo->id + 1);
}

static void think(t_philo *philo)
{
    time_ms time;

    time = millis();
    pthread_mutex_lock(&(g_params.input));
    printf("%llums \x1B[3%dm%d\033[0m is thinking\n", time, philo->id % 10, philo->id + 1);
    pthread_mutex_unlock(&(g_params.input));
}

static void *start(void *philo_t)
{
    t_philo *philo = (t_philo *)philo_t;

    // if (philo->id % 2)
    //     usleep(1000);
    while (1)
    {
        think(philo);
        if (philo->id == g_params.count - 1)
        {
            take(philo, &(g_forks[0]));
            take(philo, &(g_forks[philo->id]));
        }
        else
        {
            take(philo, &(g_forks[philo->id]));
            take(philo, &(g_forks[philo->id + 1]));
        }
        eat(philo);
        ft_sleep(philo);
    }
}

void philo_destroy(t_philo *philo)
{
    die(philo);
    pthread_mutex_unlock(&(philo->life_timer_mutex));
    pthread_mutex_destroy(&(philo->life_timer_mutex));
}

int philo_init(t_philo *philo, int id)
{
    philo->life_timer = 0;
    philo->num_of_eats = 0;
    philo->forks_count = 0;
    philo->id = id;
    pthread_create(&(philo->pthread), NULL, start, philo);
    return (pthread_mutex_init(&(philo->life_timer_mutex), NULL));
}