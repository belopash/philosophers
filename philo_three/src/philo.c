/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 11:45:28 by bbrock            #+#    #+#             */
/*   Updated: 2021/03/28 14:00:27 by bbrock           ###   ########.fr       */
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
    unsigned long long end;
    long long delta;

    end = millis() + time;
    while (1)
    {
        delta = (end - millis()) * 1000;
        if (delta <= 0)
            break;
        if (millis() >= philo->life_timer + g_params.time_to_die)
        {
            sem_wait(g_life_check);
            philo->is_dead = 1;
            sem_post(g_life_check);
            break;
        }
        usleep(((delta < 10) ? delta : 10));
    }
}

static void take(t_philo *philo)
{
    int ret;
    time_ms time;

    sem_wait(g_take);
    sem_wait(g_forks);
    sem_wait(g_input);
    printf("%llums \x1B[3%dm%d\033[0m has taken a fork\n", millis(), philo->id % 10, philo->id + 1);
    sem_post(g_input);
    sem_wait(g_forks);
    printf("%llums \x1B[3%dm%d\033[0m has taken a fork\n", millis(), philo->id % 10, philo->id + 1);
    sem_wait(g_input);
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
    printf("%llums \x1B[3%dm%d\033[0m is sleeping\n", millis(), philo->id % 10, philo->id + 1);
    sem_post(g_input);
    drop_forks(philo);
    wait(philo, g_params.time_to_sleep);
}

static void eat(t_philo *philo)
{
    sem_wait(g_life_check);
    philo->life_timer = millis();
    sem_post(g_life_check);
    sem_wait(g_input);
    printf("%llums \x1B[3%dm%d\033[0m is eating (%d)\n", philo->life_timer, philo->id % 10, philo->id + 1, philo->num_of_eats);
    sem_post(g_input);
    philo->num_of_eats++;
    wait(philo, g_params.time_to_eat);
}

static void die(t_philo *philo, time_ms time)
{
    drop_forks(philo);
    sem_wait(g_input);
    printf("%llums \x1B[3%dm%d\033[0m died\n", time, philo->id % 10, philo->id + 1);
}

static void think(t_philo *philo)
{
    sem_wait(g_input);
    printf("%llums \x1B[3%dm%d\033[0m is thinking\n", millis(), philo->id % 10, philo->id + 1);
    sem_post(g_input);
}

static void *start(void *philo_t)
{
    t_philo *philo = (t_philo *)philo_t;
    philo->life_timer = millis();
    while ((philo->num_of_eats <= g_params.num_of_eat || g_params.num_of_eat <= 0) && !philo->is_dead)
    {
        sem_wait(g_life_check);
        if (philo->is_dead)
        {
            sem_post(g_life_check);
            break;
        }
        sem_post(g_life_check);
        think(philo);
        take(philo);
        eat(philo);
        ft_sleep(philo);
    }
}

void philo_kill(t_philo *philo, time_ms time)
{
    die(philo, time);
}

int philo_init(t_philo *philo, int id)
{
    philo->life_timer = 0;
    philo->num_of_eats = 0;
    philo->id = id;
    philo->is_dead = 0;
    return pthread_create(&(philo->pthread), NULL, start, philo);
}