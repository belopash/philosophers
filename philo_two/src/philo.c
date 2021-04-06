/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 11:45:28 by bbrock            #+#    #+#             */
/*   Updated: 2021/04/06 15:06:40 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include "../inc/utils.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

void ft_wait(t_philo *philo, unsigned long long time)
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



static void drop_forks(t_philo *philo)
{
    sem_post(g_forks);
    sem_post(g_forks);
}

static void ft_sleep(t_philo *philo)
{
    ft_log(philo->id, a_sleep, millis());

    ft_wait(philo, g_params.time_to_sleep);
}

static void eat(t_philo *philo)
{
    t_ms time;


    sem_wait(philo->life_check);
    time = millis();
    philo->last_eat_time = time;
    sem_post(philo->life_check);

    ft_log(philo->id, a_eat, time);

    ft_wait(philo, g_params.time_to_eat);

    drop_forks(philo);
    sem_wait(philo->life_check);
    philo->num_of_eats++;
    sem_post(philo->life_check);
    ft_sleep(philo);
}

static void take(t_philo *philo)
{
    sem_wait(g_take);

    sem_wait(g_forks);
    ft_log(philo->id, a_take, millis());

    sem_wait(g_forks);
    ft_log(philo->id, a_take, millis());

    sem_post(g_take);
        eat(philo);
}

static void think(t_philo *philo)
{
    ft_log(philo->id, a_think, millis());
    take(philo);
}

static void *start(void *philo_t)
{
    t_philo *philo = (t_philo *)philo_t;
    philo->last_eat_time = millis();
    while (1)
    {
        think(philo);
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
