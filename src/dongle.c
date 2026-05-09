/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 16:27:42 by twaky             #+#    #+#             */
/*   Updated: 2026/05/07 17:43:30 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void    take_dongles(t_coder *coder)
{
    if (coder->id % 2 == 0)
    {
        pthread_mutex_lock(&coder->left_dongle->mutex);
        log_state(coder->sim, coder->id, STATE_TAKEN);
        pthread_mutex_lock(&coder->right_dongle->mutex);
        log_state(coder->sim, coder->id, STATE_TAKEN);
    }
    else
    {
        pthread_mutex_lock(&coder->right_dongle->mutex);
        log_state(coder->sim, coder->id, STATE_TAKEN);
        pthread_mutex_lock(&coder->left_dongle->mutex);
        log_state(coder->sim, coder->id, STATE_TAKEN);
    }
}

void    release_dongles(t_coder *coder)
{
    pthread_mutex_unlock(&coder->left_dongle->mutex);
    pthread_mutex_unlock(&coder->right_dongle->mutex);
}
