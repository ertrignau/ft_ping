/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertrigna <ertrigna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:00:23 by ertrigna          #+#    #+#             */
/*   Updated: 2026/06/03 15:01:40 by ertrigna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

volatile sig_atomic_t g_signal = 0;

void	signal_handler(int sig)
{
	(void)sig;
	g_signal = 1;
}

int main(int ac, char *av[])
{
	t_ping 	ping;
	uint8_t	buffer[1024];
	ssize_t	bytes;
	
	if (geteuid() != 0)
	{
		fprintf(stderr, "ft_ping: This program must be run with root privileges\n");
		return (1);
	}
	signal(SIGINT, signal_handler);
	init_ping(&ping);
	if (parse_arguments(ac, av, &ping) < 0)
		return (1);
	create_socket(&ping);
	resolve_hosts(&ping, ping.hostname);
	if (ping.verbose)
	{
		if (ping.numeric)
			printf("PING %s (%s): 56 data bytes, id 0x%x = %d\n", inet_ntoa(ping.dest_addr.sin_addr), inet_ntoa(ping.dest_addr.sin_addr), getpid() & 0xffff, getpid() & 0xffff);
		else
			printf("PING %s (%s): 56 data bytes, id 0x%x = %d\n", ping.hostname, inet_ntoa(ping.dest_addr.sin_addr), getpid() & 0xffff, getpid() & 0xffff);
	}
	else
	{
		if (ping.numeric)
			printf("PING %s (%s): 56 data bytes\n", inet_ntoa(ping.dest_addr.sin_addr), inet_ntoa(ping.dest_addr.sin_addr));
		else
			printf("PING %s (%s): 56 data bytes\n", ping.hostname, inet_ntoa(ping.dest_addr.sin_addr));
	}
	
	// warmup_ping(&ping);
	while (!g_signal && (ping.count == 0 || ping.transmitted < ping.count))
	{
		send_ping(&ping);
		while (1)
		{
			bytes = recv_packet(&ping, buffer, sizeof(buffer));
			if (bytes > 0)
			{
				if (parse_packet(&ping, buffer, bytes) == 0)
					break;
			}
			else if (bytes == 0)
			{
				break;
			}
			else
				break;
		}
		usleep((unsigned int)(ping.interval * 1000000));
	}
	if (g_signal || ping.transmitted > 0)
		print_stat(&ping);
	close(ping.sockfd);
	return (0);
}
