/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 23:46:54 by twaky             #+#    #+#             */
/*   Updated: 2026/04/28 23:46:23 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int init_dongles(t_simulation *sim)
{
    int i;
    int n;

    n = sim->config.number_of_coders;
    sim->dongles = malloc(sizeof(t_dongle) * n);
    if (!sim->dongles)
        return (0);
    i = 0;
    while (i < n)
    {
        sim->dongles[i].id = i;
        sim->dongles[i].is_taken = 0;
        sim->dongles[i].release_time = 0;
        sim->dongles[i].queue = NULL;

        if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
            return (0);

        if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
            return (0);
        i++;
    }
    return (1);
}

int init_coders(t_simulation *sim)
{
    int i;
    int n;

    i = 0;
    n = sim->config.number_of_coders;
    sim->coders = malloc(sizeof(t_coder) * n);
    if (!sim->coders)
        return (0);
    while (i < n)
    {
        sim->coders[i].id = i + 1;
        sim->coders[i].compile_count = 0;
        sim->coders[i].last_compile_start = 0;
        sim->coders[i].left_dongle = &sim->dongles[(i + n - 1) % n];
        sim->coders[i].right_dongle = &sim->dongles[i];
        sim->coders[i].sim = sim;

        if (pthread_mutex_init(&sim->coders[i].state_mutex, NULL) != 0)
            return (0);
        i++;
    }
    return (1);
}

int init_simulation(t_simulation *sim)
{
    sim->dongles = NULL;
    sim->coders = NULL;
    sim->start_time = 0;
    sim->sim_ended = 0;

    if (pthread_mutex_init(&sim->end_mutex, NULL) != 0)
        return (0);
    if (pthread_mutex_init(&sim->log_mutex, NULL) != 0)
        return (0);
    
    if (!init_dongles(sim))
        return (0);
    if (!init_coders(sim))
        return (0);
    return (1);
}