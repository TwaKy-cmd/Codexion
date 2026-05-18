/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/29 00:16:58 by twaky             #+#    #+#             */
/*   Updated: 2026/05/18 22:59:01 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	check_sim_ended(t_simulation *sim)
{
	int	res;

	pthread_mutex_lock(&sim->end_mutex);
	res = sim->sim_ended;
	pthread_mutex_unlock(&sim->end_mutex);
	return (res);
}

void	set_sim_ended(t_simulation *sim)
{
	pthread_mutex_lock(&sim->end_mutex);
	sim->sim_ended = 1;
	pthread_mutex_unlock(&sim->end_mutex);
}

struct timespec	compute_abstime(long long delay_ms)
{
	struct timeval	tv;
	struct timespec	ts;
	long long		total_us;

	gettimeofday(&tv, NULL);
	total_us = tv.tv_usec
		+ delay_ms * 1000;
	ts.tv_sec = tv.tv_sec + total_us / 1000000;
	ts.tv_nsec = (total_us % 1000000) * 1000;
	return (ts);
}

long long	compute_key(t_coder *coder, t_simulation *sim)
{
	if (sim->config.scheduler == SCHED_FIFO)
		return (get_time_ms());
	if (sim->config.scheduler == SCHED_EDF)
		return (coder->last_compile_start + sim->config.time_to_burnout);
	return (0);
}
