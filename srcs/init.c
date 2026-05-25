/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eric <eric@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:57:09 by ertrigna          #+#    #+#             */
/*   Updated: 2026/01/27 10:06:25 by eric             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"
#include "icmp.h"

void	init_ping(t_ping *ping)
{
	if (!ping)
		return ;
	
	memset(ping, 0, sizeof(t_ping));
	
	ping->seq = 1; 								// compteur de sequence
	ping->transmitted = 0;						// paquets envoyes
	ping->received = 0; 						// paquets recu
	ping->addrlen = sizeof(ping->dest_addr);	// taille de l'addresse
	ping->verbose = 0;
	ping->count = 0;							// 0 = infini
	ping->interval = 1.0;						// interval a 1 de base
	ping->timeout = 1;							// timeout defini a 1sec de base
	ping->hostname = " ";						// pour le flag -v
	ping->ttl = 64;
	ping->numeric = 0;
	ping->rtt_min = 999999.0;
	ping->rtt_max = 0.0;
	ping->rtt_sum = 0.0;
	ping->rtt_sum_sq = 0.0;
}

void	init_tcmp_packet(t_icmp_packet *packet, int seq)
{
	struct timeval tv;
	
	if (!packet)
		return ;
	
	memset(packet, 0, sizeof(t_icmp_packet));

	packet->header.type = ICMP_ECHO;
	packet->header.code = 0;
	packet->header.un.echo.id = htons(getpid());
	packet->header.un.echo.sequence = htons(seq);

	gettimeofday(&tv, NULL);
	memcpy(packet->payload, &tv, sizeof(tv));

	packet->header.checksum = 0;
	packet->header.checksum = icmp_checksum(packet, sizeof(t_icmp_packet));
}
