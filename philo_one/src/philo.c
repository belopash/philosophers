/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 11:45:28 by bbrock            #+#    #+#             */
/*   Updated: 2021/04/06 20:36:08 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philo.h"
#include "../inc/utils.h"
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

static void *take_left(void *philo_t)
{
	t_philo *philo = (t_philo *)philo_t;

	// int ret;

	pthread_mutex_lock(&(philo->l_fork->mutex));

	// if (fork->is_free && fork->owner != philo)
	// {
	// 	fork->owner = philo;
	ft_log(philo->id, a_take, millis());
	// philo->forks_count += 1;
	// 	fork->is_free = 0;
	// }
}

static void *take_right(void *philo_t)
{
	t_philo *philo = (t_philo *)philo_t;

	pthread_mutex_lock(&(philo->r_fork->mutex));

	// if (fork->is_free && fork->owner != philo)
	// {
	// 	fork->owner = philo;
	ft_log(philo->id, a_take, millis());
	// 	philo->forks_count += 1;
	// 	fork->is_free = 0;
	// }
}

static void drop_forks(t_philo *philo)
{
	// pthread_mutex_lock(&(philo->l_fork->mutex));
	// if (philo->l_fork->owner == philo)
	// 	philo->l_fork->is_free = 1;
	pthread_mutex_unlock(&(philo->l_fork->mutex));
	// pthread_mutex_lock(&(philo->r_fork->mutex));
	// if (philo->r_fork->owner == philo)
	// 	philo->r_fork->is_free = 1;
	pthread_mutex_unlock(&(philo->r_fork->mutex));
	pthread_mutex_lock(&(philo->life_check));
	philo->forks_count = 0;
	pthread_mutex_unlock(&(philo->life_check));
}

static void ft_sleep(t_philo *philo)
{
	t_ms time;

	time = millis();

	ft_log(philo->id, a_sleep, time);
	drop_forks(philo);

	ft_usleep(g_params.time_to_sleep);
}

static void eat(t_philo *philo)
{
	t_ms time;

	pthread_mutex_lock(&(philo->life_check));
	time = millis();
	philo->last_eat_time = time;
	pthread_mutex_unlock(&(philo->life_check));

	ft_log(philo->id, a_eat, time);

	ft_usleep(g_params.time_to_eat);

	pthread_mutex_lock(&(philo->life_check));
	philo->num_of_eats++;
	pthread_mutex_unlock(&(philo->life_check));
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
		// while (philo->forks_count < 2)
		// {
		// pthread_mutex_lock(&(g_take));
		// take(philo, philo->l_fork);
		// take(philo, philo->r_fork);
		// pthread_mutex_lock(&(g_take));
		pthread_t pths[2];
		pthread_create(&(pths[0]), NULL, take_left, philo_t);
		pthread_create(&(pths[1]), NULL, take_right, philo_t);
		pthread_join((pths[0]), 0);
		pthread_join((pths[1]), 0);
		// pthread_mutex_unlock(&g_take);
		// }
		// pthread_mutex_lock(&(philo->life_check));
		// if (philo->id % 2 == 0)
		// {
		// if (philo->id % 2 == 0)
		// {
		// 	take(philo, philo->l_fork);
		// 	take(philo, philo->r_fork);
		// }
		// else
		// {
		// 	take(philo, philo->r_fork);
		// 	take(philo, philo->l_fork);
		// }
		// if (philo->forks_count >= 1)
		// {
		// if (philo->forks_count >= 2)
		// {
		// pthread_mutex_unlock(&(philo->life_check));
		// break;
		// }
		// }
		// }
		// else
		// {
		// 	take(philo, philo->r_fork);
		// 	if (philo->forks_count == 1)
		// 		take(philo, philo->l_fork);
		// }
		// }
		// else
		// {
		//     take(philo, philo->r_fork);
		//     if (philo->forks_count != 1)
		//         continue;
		//     take(philo, philo->l_fork);
		// }
		// if (philo->forks_count != 2)
		// 	drop_forks(philo);
		// pthread_mutex_unlock(&g_take);
		// pthread_mutex_unlock(&(philo->life_check));
		// }
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
	if (id != g_params.count - 1)
	{
		philo->l_fork = &(g_forks[philo->id]);
		philo->r_fork = &(g_forks[philo->id + 1]);
	}
	else
	{
		// if (g_params.count % 2)
		// {
			philo->r_fork = &(g_forks[philo->id]);
			philo->l_fork = &(g_forks[0]);
		// }
		// else
		// {
		// 	philo->l_fork = &(g_forks[philo->id]);
		// 	philo->r_fork = &(g_forks[0]);
		// }
	}
	pthread_mutex_init(&(philo->life_check), NULL);
	return pthread_create(&(philo->pthread), NULL, start, philo);
}
