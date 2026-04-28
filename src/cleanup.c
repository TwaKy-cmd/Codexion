/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 23:30:15 by twaky             #+#    #+#             */
/*   Updated: 2026/04/28 23:44:40 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void destroy_dongles(t_simulation *sim, int count)
{
    int i;

    if (sim->dongles == NULL)
        return ;
    i = 0;
    while (i < count)
    {
        pthread_mutex_destroy(&sim->dongles[i].mutex);
        pthread_cond_destroy(&sim->dongles[i].cond);
        i++;
    }
}

void destroy_coders(t_simulation *sim, int count)
{
    int i;

    if (sim->coders == NULL)
        return ;
    i = 0;
    while (i < count)
    {
        pthread_mutex_destroy(&sim->coders[i].state_mutex);
        i++;
    }
}

void cleanup_simulation(t_simulation *sim)
{
    int n;

    n = sim->config.number_of_coders;
    destroy_coders(sim, n);
    if (sim->coders)
    {
        free(sim->coders);
        sim->coders = NULL;
    }
    destroy_dongles(sim, n);
    if (sim->dongles)
    {
        free(sim->dongles);
        sim->dongles = NULL;
    }

    pthread_mutex_destroy(&sim->end_mutex);
    pthread_mutex_destroy(&sim->log_mutex);
}