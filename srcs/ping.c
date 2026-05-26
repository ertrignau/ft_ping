/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eric <eric@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 13:28:31 by ertrigna          #+#    #+#             */
/*   Updated: 2026/01/27 10:15:03 by eric             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

// Ouvre la socket brute ICMP et applique les options de timeout et de TTL.
void create_socket(t_ping *ping)
{
	if (!ping)
		return ;
	ping->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ping->sockfd < 0)
	{
		perror("socket() failed");
		exit (EXIT_FAILURE);
	}
	set_socket_timeout(ping->sockfd, ping->timeout);
	if (setsockopt(ping->sockfd, IPPROTO_IP, IP_TTL, &ping->ttl, sizeof(ping->ttl)) < 0)
	{
		perror("setsockopt(IP_TTL) failed");
		exit (EXIT_FAILURE);
	}
}

// Resout le nom d'hote cible et remplit l'adresse de destination.
void resolve_hosts(t_ping *ping, char *host)
{
	struct addrinfo hints;
	struct addrinfo *res;

	if (!ping || !host)
		return ;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_RAW;
	
	if (getaddrinfo(host, NULL, &hints, &res) != 0)
	{
		perror("getaddrinfo() failed");
		exit (1);
	}
	ping->dest_addr = *(struct sockaddr_in *)res->ai_addr;
	ping->addrlen = sizeof(ping->dest_addr);
	freeaddrinfo(res);
	freeaddrinfo(hints.ai_addr);
}

// Construit et envoie une requete ICMP Echo vers la cible resolue.
void send_ping(t_ping *ping)
{
	t_icmp_packet	packet;
	ssize_t			sent;

	if (!ping)
		return ;
	init_tcmp_packet(&packet, ping->seq);
	sent = sendto(ping->sockfd, &packet, sizeof(t_icmp_packet), 0, (struct sockaddr *)&ping->dest_addr, ping->addrlen);
	if (sent < 0)
	{
		perror("sendto() failed\n");
		return ;
	}
	if (ping->verbose)
		printf("Sent ICMP ECHO_REQUEST, seq=%d, id=%d, size=%ld bytes\n", ping->seq, getpid(), sent);
	ping->transmitted++;
	ping->seq++;
}
