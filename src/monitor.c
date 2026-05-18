/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 18:52:54 by twaky             #+#    #+#             */
/*   Updated: 2026/05/18 23:15:30 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

void	*monitor_routine(void *arg)
{
	t_simulation	*sim;
	long long		now;

	sim = (t_simulation *)arg;
	while (!check_sim_ended(sim))
	{
		now = get_time_ms();
		if (check_burnout(sim, now))
			return (NULL);
		if (check_all_done(sim))
		{
			set_sim_ended(sim);
			wake_all_dongles(sim);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	wake_all_dongles(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}

int	check_burnout(t_simulation *sim, long long now)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		if ((now - sim->coders[i].last_compile_start)
			> sim->config.time_to_burnout)
		{
			pthread_mutex_unlock(&sim->coders[i].state_mutex);
			log_state(sim, sim->coders[i].id, STATE_BURNED);
			set_sim_ended(sim);
			wake_all_dongles(sim);
			return (1);
		}
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		i++;
	}
	return (0);
}

int	check_all_done(t_simulation *sim)
{
	int	i;

	i = 0;
	while (i < sim->config.number_of_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		if (sim->coders[i].compile_count < sim->config.compiles_required)
		{
			pthread_mutex_unlock(&sim->coders[i].state_mutex);
			return (0);
		}
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		i++;
	}
	return (1);
}
