/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 02:31:29 by twaky             #+#    #+#             */
/*   Updated: 2026/04/24 02:44:06 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int main(int argc, char *argv[])
{
    t_config config;

    if (!ft_parser(argc, argv, &config))
        return (1);
    printf("%d, %lld, %lld, %lld, %lld, %d, %lld, %d\n",
        config.number_of_coders, config.time_to_burnout,
        config.time_to_compile, config.time_to_debug,
        config.time_to_refactor, config.compiles_required,
        config.dongle_cooldown, config.scheduler);
    return (0);
}