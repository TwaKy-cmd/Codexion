/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 01:01:27 by twaky             #+#    #+#             */
/*   Updated: 2026/05/07 19:18:10 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void *coder_routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder*)arg;
    while (!check_sim_ended(coder->sim))
    {
        take_dongles(coder);
        log_state(coder->sim, coder->id, STATE_COMPILING);

        pthread_mutex_lock(&coder->state_mutex);
        coder->last_compile_start = get_time_ms();
        pthread_mutex_unlock(&coder->state_mutex);

        usleep(coder->sim->config.time_to_compile * 1000);
        
        pthread_mutex_lock(&coder->state_mutex);
        coder->compile_count++;
        pthread_mutex_unlock(&coder->state_mutex);

        release_dongles(coder);

        log_state(coder->sim, coder->id, STATE_DEBUGGING);
        usleep(coder->sim->config.time_to_debug * 1000);

        log_state(coder->sim, coder->id, STATE_REFACTOR);
        usleep(coder->sim->config.time_to_refactor * 1000);
    }
    return (NULL);
}