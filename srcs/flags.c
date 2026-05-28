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
	fprintf(stderr, "Usage: %s [-v] [-d] [-?] [-n] [-c count] [-i interval] [-W timeout] [-t ttl] <destination>\n", progname);
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

int	handle_ttl(t_ping *ping, char **av, int *i, int ac)
{
	if (*i + 1 >= ac || is_hostname_like(av[*i + 1]))
		return (fprintf(stderr, "ft_ping: option requires an argument -- 'ttl'\n"), -1);
	if (!is_valid_integer(av[*i + 1]))
		return (fprintf(stderr, "ft_ping: invalid ttl '%s' - must be numeric only\n", av[*i + 1]), -1);
	if (ping->ttl != 64)
		fprintf(stderr, "ft_ping: warning - multiple -t options, using last value\n");
	ping->ttl = atoi(av[*i + 1]);
	if (ping->ttl < 1 || ping->ttl > 255)
		return (fprintf(stderr, "ft_ping: ttl %d out of range\n", ping->ttl), -1);
	(*i)++;
	return (0);
}
