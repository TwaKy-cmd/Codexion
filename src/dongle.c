/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 16:27:42 by twaky             #+#    #+#             */
/*   Updated: 2026/05/09 21:51:41 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

static void take_one_dongle(t_dongle *dongle, t_simulation *sim)
{
    long long       elapsed;
    long long       wait_ms;
    struct timespec ts;

    pthread_mutex_lock(&dongle->mutex);
    while (1)
    {
        elapsed = get_time_ms() - dongle->release_time;
        if (!dongle->is_taken && elapsed >= sim->config.dongle_cooldown)
            break ;
        if (dongle->is_taken)
            pthread_cond_wait(&dongle->cond, &dongle->mutex);
        else
        {
            wait_ms = sim->config.dongle_cooldown - elapsed;
            ts = compute_abstime(wait_ms);
            pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
        }
    }
    dongle->is_taken = 1;
}

void    take_dongles(t_coder *coder)
{
    if (coder->id % 2 == 0)
    {
        take_one_dongle(coder->left_dongle, coder->sim);
        log_state(coder->sim, coder->id, STATE_TAKEN);
        take_one_dongle(coder->right_dongle, coder->sim);
        log_state(coder->sim, coder->id, STATE_TAKEN);
    }
    else
    {
        take_one_dongle(coder->right_dongle, coder->sim);
        log_state(coder->sim, coder->id, STATE_TAKEN);
        take_one_dongle(coder->left_dongle, coder->sim);
        log_state(coder->sim, coder->id, STATE_TAKEN);
    }
}

void    release_dongles(t_coder *coder)
{
    coder->left_dongle->is_taken = 0;
    coder->left_dongle->release_time = get_time_ms();
    pthread_cond_broadcast(&coder->left_dongle->cond);
    pthread_mutex_unlock(&coder->left_dongle->mutex);

    coder->right_dongle->is_taken = 0;
    coder->right_dongle->release_time = get_time_ms();
    pthread_cond_broadcast(&coder->right_dongle->cond);
    pthread_mutex_unlock(&coder->right_dongle->mutex);
}

