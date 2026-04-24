/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: twaky <twaky@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 01:44:26 by twaky             #+#    #+#             */
/*   Updated: 2026/04/24 02:17:42 by twaky            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int ft_parser(int argc, char *argv[], t_config *config)
{
    if (argc == 9)
    {
        if (!is_valid_number(argv[1]))
            return (0);
        config->number_of_coders = atoi(argv[1]);
        if (!is_valid_number(argv[2]))
            return (0);
        config->time_to_burnout = atoi(argv[2]);
        if (!is_valid_number(argv[3]))
            return (0);
        config->time_to_compile = atoi(argv[3]);
        if (!is_valid_number(argv[4]))
            return (0);
        config->time_to_debug = atoi(argv[4]);
        if (!is_valid_number(argv[5]))
            return (0);
        config->time_to_refactor = atoi(argv[5]);
        return (ft_parser2(argc, argv, config));
    }
    else
        fprintf(stderr, "Error: 9 arguments required\n");
    return (0);
}

int ft_parser2(int argc, char *argv[], t_config *config)
{
    if (!is_valid_number(argv[6]))
        return (0);
    config->compiles_required = atoi(argv[6]);
    if (!is_valid_number(argv[7]))
        return (0);
    config->dongle_cooldown = atoi(argv[7]);
    if (!check_argv_8(argv[8], config))
        return (0);
    return (check_range(config));
}

int is_valid_number(char *str)
{
    int i;

    i = 0;
    while (str[i])
    {
        if( str[i] < '0' || str[i] > '9')
            return (0);
        else
            i++;
    }
    return (1);
}

int check_argv_8(char *str, t_config *t_config)
{
    if (strcmp(str, "fifo") == 0)
    {
        t_config->scheduler = 0;
        return (1);   
    }
    if (strcmp(str, "edf") == 0)
    {
        t_config->scheduler = 1;
        return (1);   
    }
    return (0);
}

int check_range(t_config *config)
{
    if (config->number_of_coders < 1)
        return (0);
    if (config->time_to_burnout < 1)
        return (0);
    if (config->time_to_compile < 1)
        return (0);
    if (config->time_to_debug < 1)
        return (0);
    if (config->time_to_refactor < 1)
        return (0);
    if (config->compiles_required < 0)
        return (0);
    if (config->dongle_cooldown < 0)
        return (0);
    return (1);
}