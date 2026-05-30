/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eric <eric@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 12:49:27 by ertrigna          #+#    #+#             */
/*   Updated: 2026/05/30 09:53:54 by eric             ###   ########.fr       */
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
	double	rtt_stddev;
	char	min_str[20], avg_str[20], max_str[20], std_str[20];

	printf("--- %s ping statistics ---\n", ping->hostname);
	lost = ping->transmitted - ping->received;
	loss_percent = 0.0;
	if (ping->transmitted > 0)
		loss_percent = ((float)lost / (float)ping->transmitted) * 100.0;
	printf("%d packets transmitted, %d packets received, %.0f%% packet loss\n", 
		ping->transmitted, ping->received, loss_percent);

	if (ping->received > 0)
	{
		rtt_avg = ping->rtt_sum / ping->received;
		rtt_stddev = sqrt((ping->rtt_sum_sq / ping->received) - (rtt_avg * rtt_avg));
		snprintf(min_str, sizeof(min_str), "%.3f", ping->rtt_min);
		snprintf(avg_str, sizeof(avg_str), "%.3f", rtt_avg);
		snprintf(max_str, sizeof(max_str), "%.3f", ping->rtt_max);
		snprintf(std_str, sizeof(std_str), "%.3f", rtt_stddev);
		for (int i = 0; min_str[i]; i++) if (min_str[i] == '.') min_str[i] = ',';
		for (int i = 0; avg_str[i]; i++) if (avg_str[i] == '.') avg_str[i] = ',';
		for (int i = 0; max_str[i]; i++) if (max_str[i] == '.') max_str[i] = ',';
		for (int i = 0; std_str[i]; i++) if (std_str[i] == '.') std_str[i] = ',';
		printf("round-trip min/avg/max/stddev = %s/%s/%s/%s ms\n", 
			min_str, avg_str, max_str, std_str);
	}
}

int	is_valid_integer(const char *str)
{
	int	i;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	while (str[i])
	{
		if (!isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_valid_float(const char *str)
{
	int	i;
	int	dot_count;

	if (!str || str[0] == '\0')
		return (0);
	i = 0;
	dot_count = 0;
	while (str[i])
	{
		if (str[i] == '.')
			dot_count++;
		else if (!isdigit(str[i]))
			return (0);
		if (dot_count > 1)
			return (0);
		i++;
	}
	return (1);
}

int	is_hostname_like(const char *str)
{
	if (!str || str[0] == '\0')
		return (0);
	if (is_valid_integer(str) || is_valid_float(str))
		return (0);
	if (str[0] == '-')
		return (0);
	return (1);
}
