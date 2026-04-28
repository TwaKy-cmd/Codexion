/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 01:08:15 by twaky             #+#    #+#             */
/*   Updated: 2026/04/29 01:14:57 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int start_simulation(t_simulation *sim)
{
    int n;
    int i;

    i = 0;
    n = sim->config.number_of_coders;
    sim->start_time = get_time_ms();
    while (i < n)
    {
        if (pthread_create(&sim->coders[i].thread, NULL, coder_routine, &sim->coders[i]) != 0)
            return (0);
        i++;
    }
    i = 0;
    while (i < n)
    {
        pthread_join(sim->coders[i].thread, NULL);
        i++;
    }
    return (1);
}