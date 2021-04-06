/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 11:45:28 by bbrock            #+#    #+#             */
/*   Updated: 2021/03/28 19:55:42 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include "../inc/utils.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void wait(unsigned long long time)
{
    // t_ms end;

    // end = millis() + time;
    // while (millis() < end)
    // {
    usleep(time * 1000);
    // }
}

static void take(t_philo *philo, t_fork *fork)
{
    int ret;

    pthread_mutex_lock(&(fork->mutex));
    if (fork->owner == NULL)
    {
        fork->owner = philo;
        philo->forks_count += 1;
    }
    pthread_mutex_unlock(&(fork->mutex));
}

static void drop_forks(t_philo *philo)
{
    pthread_mutex_lock(&(philo->l_fork->mutex));
    if (philo->l_fork->owner == philo)
        philo->l_fork->owner = NULL;
    pthread_mutex_unlock(&(philo->l_fork->mutex));
    pthread_mutex_lock(&(philo->r_fork->mutex));
    if (philo->r_fork->owner == philo)
        philo->r_fork->owner = NULL;
    pthread_mutex_unlock(&(philo->r_fork->mutex));
    philo->forks_count = 0;
}

static void ft_sleep(t_philo *philo)
{
    pthread_mutex_lock(&(g_input));
    printf("%llums \x1B[3%dm%d\033[0m is sleeping\n", millis(), philo->id % 10 + 1, philo->id + 1);
    pthread_mutex_unlock(&(g_input));

    drop_forks(philo);

    wait(g_params.time_to_sleep);
}

static void eat(t_philo *philo)
{
    t_ms time;

    time = millis();

    pthread_mutex_lock(&(g_life_check));
    philo->last_eat_time = time;
    pthread_mutex_unlock(&(g_life_check));

    pthread_mutex_lock(&(g_input));
    printf("%llums \x1B[3%dm%d\033[0m has taken a fork\n", time, philo->id % 10 + 1, philo->id + 1);
    printf("%llums \x1B[3%dm%d\033[0m has taken a fork\n", time, philo->id % 10 + 1, philo->id + 1);
    printf("%llums \x1B[3%dm%d\033[0m is eating (%d)\n", time, philo->id % 10 + 1, philo->id + 1, philo->num_of_eats + 1);
    pthread_mutex_unlock(&(g_input));

    wait(g_params.time_to_eat);
    
    pthread_mutex_lock(&(g_life_check));
    philo->num_of_eats++;
    pthread_mutex_unlock(&(g_life_check));
}

static void think(t_philo *philo)
{
    pthread_mutex_lock(&(g_input));
    printf("%llums \x1B[3%dm%d\033[0m is thinking\n", millis(), philo->id % 10 + 1, philo->id + 1);
    pthread_mutex_unlock(&(g_input));
}

static void *start(void *philo_t)
{
    t_philo *philo = (t_philo *)philo_t;
    philo->last_eat_time = millis();
    while ((philo->num_of_eats < g_params.num_of_eat || g_params.num_of_eat <= 0))
    {
        think(philo);
        while (philo->forks_count < 2)
        {
            // pthread_mutex_lock(&g_take);
            if (philo->id % 2 == 0)
            {
                take(philo, philo->l_fork);
                if (philo->forks_count != 1)
                    continue;
                take(philo, philo->r_fork);
            }
            else
            {
                take(philo, philo->r_fork);
                if (philo->forks_count != 1)
                    continue;
                take(philo, philo->l_fork);
            }
            // pthread_mutex_unlock(&g_take);
            if (philo->forks_count != 2)
                drop_forks(philo);
        }
        eat(philo);
        ft_sleep(philo);
    }
}

int philo_init(t_philo *philo, int id)
{
    philo->num_of_eats = 0;
    philo->forks_count = 0;
    philo->finished = 0;
    philo->id = id;
    philo->l_fork = &(g_forks[philo->id]);
    if (id != g_params.count - 1)
        philo->r_fork = &(g_forks[philo->id + 1]);
    else
        philo->r_fork = &(g_forks[0]);
    return pthread_create(&(philo->pthread), NULL, start, philo);
}