/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 11:40:02 by bbrock            #+#    #+#             */
/*   Updated: 2021/01/27 23:00:35 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_PHILO_H
#define T_PHILO_H

#include <pthread.h>

#define PHILO_MAX 500
#define ARG_MAX 6
#define ARG_MIN 5

enum ARGS
{
    count = 1,
    ttd = 2,
    tte = 3,
    tts = 4,
    noe = 5
};

typedef struct s_philo_params
{
    int count;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int num_of_eat;
    pthread_mutex_t input;
} t_philo_params;

t_philo_params g_params;
pthread_mutex_t g_forks[PHILO_MAX];

typedef struct s_philo
{
    int id;
    pthread_t pthread;
    int forks_count;
    unsigned long long life_timer;
    pthread_mutex_t life_timer_mutex;
    int num_of_eats;
} t_philo;
t_philo g_philos[PHILO_MAX];

int philo_init(t_philo *philo, int id);
void philo_destroy(t_philo *philo);

#endif