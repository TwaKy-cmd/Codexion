/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 01:01:27 by twaky             #+#    #+#             */
/*   Updated: 2026/05/18 23:10:23 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	coder_compile(t_coder *coder)
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
}

void	coder_rest(t_coder *coder)
{
	log_state(coder->sim, coder->id, STATE_DEBUGGING);
	usleep(coder->sim->config.time_to_debug * 1000);
	log_state(coder->sim, coder->id, STATE_REFACTOR);
	usleep(coder->sim->config.time_to_refactor * 1000);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->left_dongle == coder->right_dongle)
	{
		take_dongles(coder);
		while (!check_sim_ended(coder->sim))
			usleep(1000);
		return (NULL);
	}
	while (!check_sim_ended(coder->sim))
	{
		coder_compile(coder);
		coder_rest(coder);
	}
	return (NULL);
}
