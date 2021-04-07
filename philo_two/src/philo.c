/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 11:45:28 by bbrock            #+#    #+#             */
/*   Updated: 2021/04/06 17:33:17 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include "../inc/utils.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

static void drop_forks(t_philo *philo)
{
    sem_post(g_forks);
    sem_post(g_forks);
}

static void ft_sleep(t_philo *philo)
{
    t_ms time;

    time = millis();
    ft_log(philo->id, a_sleep, time);

    ft_usleep(g_params.time_to_sleep);
}

static void eat(t_philo *philo)
{
    t_ms time;

    sem_wait(philo->life_check);
    time = millis();
    philo->last_eat_time = time;
    sem_post(philo->life_check);

    ft_log(philo->id, a_eat, time);

    ft_usleep(g_params.time_to_eat);

    drop_forks(philo);
    sem_wait(philo->life_check);
    philo->num_of_eats++;
    sem_post(philo->life_check);
}

static void take(t_philo *philo)
{
    sem_wait(g_take);

    sem_wait(g_forks);
    ft_log(philo->id, a_take, millis());

    sem_wait(g_forks);
    ft_log(philo->id, a_take, millis());

    sem_post(g_take);
}

static void think(t_philo *philo)
{
    ft_log(philo->id, a_think, millis());
}

static void *start(void *philo_t)
{
    t_philo *philo = (t_philo *)philo_t;
    philo->last_eat_time = millis();
    while (1)
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
    sem_unlink("life_check");
    philo->life_check = sem_open("life_check", O_CREAT, 0777, 1);
    return pthread_create(&(philo->pthread), NULL, start, philo);
}
