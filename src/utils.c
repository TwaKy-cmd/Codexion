/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 00:16:58 by twaky             #+#    #+#             */
/*   Updated: 2026/05/09 21:53:53 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

long long get_time_ms(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);
    return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int  check_sim_ended(t_simulation *sim)
{
    int res;
    
    pthread_mutex_lock(&sim->end_mutex);
    res = sim->sim_ended;
    pthread_mutex_unlock(&sim->end_mutex);
    return (res);
}

void set_sim_ended(t_simulation *sim)
{
    pthread_mutex_lock(&sim->end_mutex);
    sim->sim_ended = 1;
    pthread_mutex_unlock(&sim->end_mutex);
}

struct timespec    compute_abstime(long long delay_ms)
{
    struct timeval  tv;
    struct timespec ts;
    long long       total_us;

    gettimeofday(&tv, NULL);
    total_us = tv.tv_usec + delay_ms * 1000;       /* µs actuelles + délai en µs */
    ts.tv_sec = tv.tv_sec + total_us / 1000000;    /* on ajoute les secondes "carry over" */
    ts.tv_nsec = (total_us % 1000000) * 1000;      /* le reste en ns */
    return (ts);
}