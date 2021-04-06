/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bbrock <bbrock@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/26 13:42:33 by bbrock            #+#    #+#             */
/*   Updated: 2021/03/28 18:04:52 by bbrock           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "../inc/philo.h"
#include "../inc/utils.h"
#include "stdlib.h"
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>  
#include <errno.h>
#include <string.h>

void kill_philo(int id){
    sem_wait(g_input);
    printf("%llums \x1B[3%dm%d\033[0m died\n", millis(), id % 10 + 1, id + 1);
}

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

int check_philo(int id)
{
    t_ms time;

    if (g_philos[id].finished)
        return (0);
    time = millis();
    sem_wait(g_life_check);
    if (g_philos[id].num_of_eats == g_params.num_of_eat)
    {
        pthread_detach(g_philos[id].pthread);
        g_philos[id].finished = 1;
        g_finished_philos++;  
    }
    else if (time >= g_philos[id].last_eat_time + g_params.time_to_die)
    {
        kill_philo(id);
        return (1);
    }
    sem_post(g_life_check);
    return (0);
}

int monitor(){
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
        usleep(500);
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
    sem_unlink("forks");
    sem_unlink("input");
    sem_unlink("life_check");
    sem_unlink("take");
    g_finished_philos = 0;
    g_forks = sem_open("forks", O_CREAT, 0777, g_params.count);
    g_input = sem_open("input", O_CREAT, 0777, 1);
    g_life_check = sem_open("life_check", O_CREAT, 0777, 1);
    g_take = sem_open("take", O_CREAT, 0777, 1);
    while (i < g_params.count)
    {
        philo_init(&(g_philos[i]), i);
        i++;
    }
    return monitor();
}