/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 00:46:15 by twaky             #+#    #+#             */
/*   Updated: 2026/04/29 00:46:47 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h" 

void    log_state(t_simulation *sim, int coder_id, int state)
{
    long long time;
    
    time = get_time_ms() - sim->start_time;
    pthread_mutex_lock(&sim->log_mutex);

    if (state == STATE_TAKEN)
        printf("%lld %d has taken a dongle\n", time, coder_id);
    else if (state == STATE_COMPILING)
        printf("%lld %d is compiling\n", time, coder_id);
    else if (state == STATE_DEBUGGING)
        printf("%lld %d is debugging\n", time, coder_id);
    else if (state == STATE_REFACTOR)
        printf("%lld %d is refactoring\n", time, coder_id);
    else if (state == STATE_BURNED)
        printf("%lld %d burned out\n", time, coder_id);
    pthread_mutex_unlock(&sim->log_mutex);
}