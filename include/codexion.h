/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 00:27:12 by twaky             #+#    #+#             */
/*   Updated: 2026/05/18 23:10:12 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H

# define CODEXION_H
# define STATE_TAKEN      0
# define STATE_COMPILING  1
# define STATE_DEBUGGING  2
# define STATE_REFACTOR   3
# define STATE_BURNED     4
# define SCHED_FIFO 0
# define SCHED_EDF  1

# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_simulation	t_simulation;

typedef struct s_heap_node
{
	int			coder_id;
	long long	key;
}	t_heap_node;

typedef struct s_heap
{
	t_heap_node		*nodes;
	int				size;
	int				capacity;
}	t_heap;

typedef struct s_dongle
{
	int					id;
	int					is_taken;
	long long			release_time;
	pthread_mutex_t		mutex;
	pthread_cond_t		cond;
	t_heap				*queue;
}	t_dongle;

typedef struct s_coder
{
	int					id;
	pthread_t			thread;
	int					compile_count;
	long long			last_compile_start;
	pthread_mutex_t		state_mutex;
	t_dongle			*left_dongle;
	t_dongle			*right_dongle;
	t_simulation		*sim;
}	t_coder;

typedef struct s_config
{
	int			number_of_coders;
	long long	time_to_burnout;
	long long	time_to_compile;
	long long	time_to_debug;
	long long	time_to_refactor;
	int			compiles_required;
	long long	dongle_cooldown;
	int			scheduler;
}	t_config;

typedef struct s_simulation
{
	t_config			config;
	t_dongle			*dongles;
	t_coder				*coders;
	pthread_t			monitor_thread;
	int					sim_ended;
	pthread_mutex_t		end_mutex;
	pthread_mutex_t		log_mutex;
	long long			start_time;
}	t_simulation;

/* parsing.c */

int				ft_parser(int argc, char *argv[], t_config *config);
int				ft_parser2(char *argv[], t_config *config);
int				is_valid_number(char *str);
int				check_argv_8(char *str, t_config *t_config);
int				check_range(t_config *config);

/* init.c */

int				init_dongles(t_simulation *sim);
int				init_coders(t_simulation *sim);
int				init_simulation(t_simulation *sim);

/* cleanup.c */

void			destroy_dongles(t_simulation *sim, int count);
void			destroy_coders(t_simulation *sim, int count);
void			cleanup_simulation(t_simulation *sim);

/* utils.c */

long long		get_time_ms(void);
int				check_sim_ended(t_simulation *sim);
void			set_sim_ended(t_simulation *sim);
struct timespec	compute_abstime(long long delay_ms);
long long		compute_key(t_coder *coder, t_simulation *sim);

/* logs.c */

void			log_state(t_simulation *sim, int coder_id, int state);

/* coder.c */

void			coder_compile(t_coder *coder);
void			coder_rest(t_coder *coder);
void			*coder_routine(void *arg);

/* simulation.c */

int				start_simulation(t_simulation *sim);

/* dongle.c */

int				try_take(t_dongle *dongle, t_coder *coder, long long elapsed);
void			take_one_dongle_utils(t_coder *coder, t_dongle *dongle);
void			take_one_dongle(t_coder *coder, t_dongle *dongle);
void			take_dongles(t_coder *coder);
void			release_dongles(t_coder *coder);

/* monitor.c */

void			*monitor_routine(void *arg);
void			wake_all_dongles(t_simulation *sim);
int				check_burnout(t_simulation *sim, long long now);
int				check_all_done(t_simulation *sim);

/* heap.c */

t_heap			*heap_create(int capacity);
int				heap_push(t_heap *heap, int coder_id, long long key);
int				get_smallest(t_heap *heap, int i);
int				heap_pop(t_heap *heap, int *out_coder_id);

/* heap_utils.c */

void			heap_destroy(t_heap *heap);
int				swap(t_heap *heap, int i, int smallest);
int				heap_peek(t_heap *heap, int *out_coder_id);

#endif