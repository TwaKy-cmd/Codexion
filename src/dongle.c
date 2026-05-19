/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khebert <khebert@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 16:27:42 by twaky             #+#    #+#             */
/*   Updated: 2026/05/20 01:43:42 by khebert          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int	try_take(t_dongle *dongle, t_coder *coder, long long elapsed)
{
	int	first_id;

	heap_peek(dongle->queue, &first_id);
	if (dongle->is_taken)
		return (0);
	if (elapsed < coder->sim->config.dongle_cooldown)
		return (0);
	if (first_id != coder->id)
		return (0);
	heap_pop(dongle->queue, &first_id);
	dongle->is_taken = 1;
	return (1);
}

void	take_one_dongle_utils(t_coder *coder, t_dongle *dongle)
{
	long long		elapsed;
	int				first_id;
	struct timespec	ts;
	long long		wait_ms;

	while (1)
	{
		elapsed = get_time_ms() - dongle->release_time;
		if (try_take(dongle, coder, elapsed))
			break ;
		heap_peek(dongle->queue, &first_id);
		if (dongle->is_taken || first_id != coder->id)
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
		else
		{
			wait_ms = coder->sim->config.dongle_cooldown - elapsed;
			ts = compute_abstime(wait_ms);
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		}
	}
}

void	take_one_dongle(t_coder *coder, t_dongle *dongle)
{
	long long		key;

	key = compute_key(coder, coder->sim);
	pthread_mutex_lock(&dongle->mutex);
	heap_push(dongle->queue, coder->id, key);
	take_one_dongle_utils(coder, dongle);
}

void	take_dongles(t_coder *coder)
{
	if (coder->left_dongle == coder->right_dongle)
	{
		take_one_dongle(coder, coder->right_dongle);
		log_state(coder->sim, coder->id, STATE_TAKEN);
		return ;
	}
	if (coder->id % 2 == 0)
	{
		take_one_dongle(coder, coder->left_dongle);
		log_state(coder->sim, coder->id, STATE_TAKEN);
		take_one_dongle(coder, coder->right_dongle);
		log_state(coder->sim, coder->id, STATE_TAKEN);
	}
	else
	{
		take_one_dongle(coder, coder->right_dongle);
		log_state(coder->sim, coder->id, STATE_TAKEN);
		take_one_dongle(coder, coder->left_dongle);
		log_state(coder->sim, coder->id, STATE_TAKEN);
	}
}

void	release_dongles(t_coder *coder)
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
