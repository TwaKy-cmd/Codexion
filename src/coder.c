/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 01:01:27 by twaky             #+#    #+#             */
/*   Updated: 2026/04/29 01:20:22 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void *coder_routine(void *arg)
{
    t_coder *coder;
    int i;

    i = 0;
    coder = (t_coder*)arg;
    while (i < 3)
    {
        log_state(coder->sim, coder->id, STATE_COMPILING);
        usleep(coder->sim->config.time_to_compile * 1000);

        log_state(coder->sim, coder->id, STATE_DEBUGGING);
        usleep(coder->sim->config.time_to_debug * 1000);

        log_state(coder->sim, coder->id, STATE_REFACTOR);
        usleep(coder->sim->config.time_to_refactor * 1000);
   
        i++;
    }
    return (NULL);
}