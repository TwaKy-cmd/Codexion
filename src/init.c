/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 23:46:54 by twaky             #+#    #+#             */
/*   Updated: 2026/04/24 23:59:26 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
