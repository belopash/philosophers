/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/25 11:40:02 by bbrock            #+#    #+#             */
/*   Updated: 2021/04/06 14:53:32 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef T_PHILO_H
#define T_PHILO_H

#include <semaphore.h>
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

enum ACTIONS
{
    a_eat,
    a_think,
    a_sleep,
    a_die,
    a_take
};

typedef struct s_philo_params
{
    int count;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int num_of_eat;
} t_philo_params;

t_philo_params g_params;

typedef unsigned long long t_ms;

typedef struct s_philo
{
    int id;
    pthread_t pthread;
    t_ms last_eat_time;
    int num_of_eats;
    int finished;
    sem_t *life_check;
} t_philo;
t_philo g_philos[PHILO_MAX];

sem_t *g_input;
sem_t *g_forks;
sem_t *g_take;

int g_finished_philos;

int philo_init(t_philo *philo, int id);
void ft_log(int id, int action, t_ms time);

#endif
