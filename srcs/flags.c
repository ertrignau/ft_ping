/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertrigna <ertrigna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:44:27 by ertrigna          #+#    #+#             */
/*   Updated: 2026/05/26 15:44:58 by ertrigna         ###   ########.fr       */
/*                                                                              */
/* ************************************************************************** */

#include "ping.h"

// Flag handlers

void	print_usage(char *progname)
{
	fprintf(stderr, "Usage: %s [-v] [-?] [-n] [-c count] [-i interval] [-W timeout] [-s size] <destination>\n", progname);
}

int	handle_count(t_ping *ping, char **av, int *i, int ac)
{
	if (*i + 1 >= ac || is_hostname_like(av[*i + 1]))
		return (fprintf(stderr, "ft_ping: option requires an argument -- 'c'\n"), -1);
	if (!is_valid_integer(av[*i + 1]))
		return (fprintf(stderr, "ft_ping: invalid count '%s' - must be numeric only\n", av[*i + 1]), -1);
	ping->count = atoi(av[*i + 1]);
	if (ping->count <= 0)
		return (fprintf(stderr, "ft_ping: bad number of packets to transmit.\n"), -1);
	(*i)++;
	return (0);
}

int	handle_interval(t_ping *ping, char **av, int *i, int ac)
{
	if (*i + 1 >= ac || is_hostname_like(av[*i + 1]))
		return (fprintf(stderr, "ft_ping: option requires an argument -- 'i'\n"), -1);
	if (!is_valid_float(av[*i + 1]))
		return (fprintf(stderr, "ft_ping: invalid interval '%s' - must be numeric only\n", av[*i + 1]), -1);
	ping->interval = atof(av[*i + 1]);
	if (ping->interval < 0.2)
		return (fprintf(stderr, "ft_ping: bad timing interval.\n"), -1);
	(*i)++;
	return (0);
}

int	handle_timeout(t_ping *ping, char **av, int *i, int ac)
{
	if (*i + 1 >= ac)
		return (fprintf(stderr, "ft_ping: option requires an argument -- 'W'\n"), -1);
	if (!is_valid_integer(av[*i + 1]))
		return (fprintf(stderr, "ft_ping: invalid timeout '%s' - must be numeric only\n", av[*i + 1]), -1);
	ping->timeout = atoi(av[*i + 1]);
	if (ping->timeout <= 0)
		return (fprintf(stderr, "ft_ping: bad wait time\n"), -1);
	(*i)++;
	return (0);
}

int	handle_size(t_ping *ping, char **av, int *i, int ac)
{
	if (*i + 1 >= ac || is_hostname_like(av[*i + 1]))
		return (fprintf(stderr, "ft_ping: option requires an argument -- 's'\n"), -1);
	if (!is_valid_integer(av[*i + 1]))
		return (fprintf(stderr, "ft_ping: invalid size '%s' - must be numeric only\n", av[*i + 1]), -1);
	if (ping->size != 56)
		fprintf(stderr, "ft_ping: warning - multiple -s options, using last value\n");
	ping->size = atoi(av[*i + 1]);
	if (ping->size < 0 || ping->size > 65507)
		return (fprintf(stderr, "ft_ping: size %d out of range\n", ping->size), -1);
	(*i)++;
	return (0);
}
