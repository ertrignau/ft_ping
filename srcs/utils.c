/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eric <eric@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:49:27 by ertrigna          #+#    #+#             */
/*   Updated: 2026/01/26 12:30:17 by eric             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

// Configure le timeout de reception sur la socket.
void	set_socket_timeout(int sockfd, int seconds)
{
	struct timeval timeout;

	timeout.tv_sec = seconds;
	timeout.tv_usec = 0;
	
	if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
	{
		perror("setsockopt() failed\n");
		exit (EXIT_FAILURE);
	}
}

// Affiche le bilan final avec les pertes et les statistiques RTT.
void	print_stat(t_ping *ping)
{
	int		lost;
	float	loss_percent;
	double	rtt_avg;
	double	rtt_mdev;

	printf("\n--- %s ping statistics ---\n", ping->hostname);
	lost = ping->transmitted - ping->received;
	loss_percent = 0.0;
	if (ping->transmitted > 0)
		loss_percent = ((float)lost / (float)ping->transmitted) * 100.0;
	printf("%d packets transmitted, %d packets received, %.1f%% packet loss\n", 
		ping->transmitted, ping->received, loss_percent);

	if (ping->received > 0)
	{
		rtt_avg = ping->rtt_sum / ping->received;
		rtt_mdev = sqrt((ping->rtt_sum_sq / ping->received) - (rtt_avg * rtt_avg));
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n", 
			ping->rtt_min, rtt_avg, ping->rtt_max, rtt_mdev);
	}
}