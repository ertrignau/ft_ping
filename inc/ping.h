/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertrigna <ertrigna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:40:24 by ertrigna          #+#    #+#             */
/*   Updated: 2026/05/28 16:41:48 by ertrigna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PING_H
#define PING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <math.h>
#include <ctype.h>

#include <netinet/ip.h>

#include "icmp.h"
#include "utils.h"

typedef struct s_ping
{
	int					sockfd;
	struct sockaddr_in	dest_addr;
	socklen_t			addrlen;
	
	int					seq;
	int					transmitted;
	int					received;
	int					verbose;
	int					debug;
	int					count;
	double				interval;
	int					timeout;
	int					ttl;
	int					numeric;
	char				*hostname;

	double				rtt_min;
	double				rtt_max;
	double				rtt_sum;
	double				rtt_sum_sq;
	
	struct timeval		send_time;
} t_ping;

void	init_ping(t_ping *ping);					// init de la structure
void	create_socket(t_ping *ping);				// creation du socket avec socket()
void 	resolve_hosts(t_ping *ping, char *host);	// transforme une destination humaine en destination reseau
void	send_ping(t_ping *ping);					// fonction principale qui envoie un ping formate en icmp

#endif