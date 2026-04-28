/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 02:31:29 by twaky             #+#    #+#             */
/*   Updated: 2026/04/28 23:46:37 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/codexion.h"

int main(int argc, char *argv[])
{
    t_simulation sim;

    if (!ft_parser(argc, argv, &sim.config))
        return (1);
    if (!init_simulation(&sim))
    {
        cleanup_simulation(&sim);
        return (1);
    }
    /* ici plus tard : start_simulation, monitor, etc. */
    cleanup_simulation(&sim);
    return (0);
}