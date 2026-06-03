/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertrigna <ertrigna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:57:09 by ertrigna          #+#    #+#             */
/*   Updated: 2026/06/03 14:53:01 by ertrigna         ###   ########.fr       */
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
	ping->debug = 0;
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

// Initialise un paquet ICMP Echo Request avec les champs appropries (type, code, id, sequence, timestamp) et calcule le checksum.
// @packet: pointeur vers la structure de paquet ICMP a remplir
// @seq: numero de sequence a inclure dans le paquet
// Remplit les champs de l'en-tete ICMP, ajoute un timestamp dans la charge utile, et calcule le checksum pour assurer l'integrite du paquet.
// Utilise gettimeofday() pour obtenir le temps d'envoi et le stocke dans la charge utile du paquet. Le checksum est calcule sur l'ensemble du paquet ICMP.
// icmp_checksum() est appele pour calculer le checksum et le champ checksum de l'en-tete est mis a jour en consequence. Le paquet est pret a etre envoye apres cette initialisation.
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
