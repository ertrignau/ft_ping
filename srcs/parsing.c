/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertrigna <ertrigna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/22 17:10:01 by ertrigna          #+#    #+#             */
/*   Updated: 2026/05/28 16:41:47 by ertrigna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

int	parse_arguments(int ac, char *av[], t_ping *ping)
{
	char	*host;
	int		i;

	if (ac < 2)
	{
		print_usage(av[0]);
		return (-1);
	}
	host = NULL;
	i = 1;
	while (i < ac)
	{
		if (av[i][0] != '-')
		{
			host = av[i];
			i++;
			continue;
		}
		if (strcmp(av[i], "-v") == 0)
			ping->verbose = 1;
		else if (strcmp(av[i], "-d") == 0)
			ping->debug = 1;
		else if (strcmp(av[i], "-n") == 0)
			ping->numeric = 1;
		else if (strcmp(av[i], "-?") == 0)
		{
			print_usage(av[0]);
			exit (0);
		}
		else if (strcmp(av[i], "-c") == 0)
		{
			if (handle_count(ping, av, &i, ac) < 0)
				return (-1);
		}
		else if (strcmp(av[i], "-i") == 0)
		{
			if (handle_interval(ping, av, &i, ac) < 0)
				return (-1);
		}
		// For timeout testing ip : 203.0.113.1
		else if (strcmp(av[i], "-W") == 0)
		{
			if (handle_timeout(ping, av, &i, ac) < 0)
				return (-1);
		}
		else if (strcmp(av[i], "-t") == 0 || strcmp(av[i], "--ttl") == 0)
		{
			if (handle_ttl(ping, av, &i, ac) < 0)
				return (-1);
		}
		else
		{
			fprintf(stderr, "ft_ping: invalid option -- '%s'\n", av[i]);
			print_usage(av[0]);
			return (-1);
		}
		i++;
	}
	if (!host)
	{
		fprintf(stderr, "ft_ping: missing host operand\n");
		return (-1);
	}
	resolve_hosts(ping, host);
	ping->hostname = host;
	return (0);
}
