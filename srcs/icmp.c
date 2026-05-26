/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eric <eric@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:17:56 by ertrigna          #+#    #+#             */
/*   Updated: 2026/01/26 12:28:52 by eric             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

// Calcule le checksum ICMP pour un bloc de donnees brut.
uint16_t icmp_checksum(void *data, int len)
{
	uint16_t	*buf;
	uint32_t	sum;
	
	if (!data || len < 0)
		return (0);
	buf = data;
	sum = 0;
	while (len > 1)
	{
		sum += *buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(uint8_t *)buf;
	sum = (sum >> 16) + (sum & 0xffff);
	sum += (sum >> 16);
	return ((uint16_t)(~sum));
}

// Recoit un paquet sur la socket non bloquante et gere les erreurs temporaires.
ssize_t recv_packet(t_ping *ping, uint8_t *buffer, size_t size)
{
	struct sockaddr_in	addr;
	socklen_t			addrlen = sizeof(addr);
	ssize_t				bytes = recvfrom(ping->sockfd, buffer, size, 0, (struct sockaddr *)&addr, &addrlen);
	
	if (bytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (0);
		perror("recvfrom() failed");
		return (-1);
	}
	
	return (bytes);
}

// Verifie l'en-tete IP puis extrait la partie ICMP du paquet recu.
int	parse_packet(t_ping *ping, uint8_t *buf, ssize_t len)
{
	struct iphdr	*ip_hdr;
	struct icmphdr	*icmp_hdr;
	int				ip_len;

	if (len < (ssize_t)(sizeof(struct iphdr) + sizeof(struct icmphdr)))
		return (-1);
	ip_hdr = (struct iphdr *)buf;
	ip_len = ip_hdr->ihl * 4;
	if (len < (ssize_t)(ip_len + sizeof(struct icmphdr)))
		return (-1);
	icmp_hdr = (struct icmphdr *)(buf + ip_len);
	return (parse_icmp(ping, buf + ip_len, len - ip_len, ntohs(icmp_hdr->un.echo.sequence), ip_hdr->ttl));
}

// Valide le type, l'identifiant et la sequence avant de traiter la reponse.
int	parse_icmp(t_ping *ping, uint8_t *packet, ssize_t len, int seq, int ttl)
{
	t_icmp_packet *icmp_packet;

	if (!ping || !packet || len < (ssize_t)sizeof(t_icmp_packet))
		return (-1);
	icmp_packet = (t_icmp_packet *)packet;
	if (icmp_packet->header.type != ICMP_ECHOREPLY)
	{
		if (ping->verbose)
			printf("Received ICMP packet type %d, code %d\n", icmp_packet->header.type, icmp_packet->header.code);
		return (-1);
	}
	if (ntohs(icmp_packet->header.un.echo.id) != getpid())
		return (-1);
	if (ntohs(icmp_packet->header.un.echo.sequence) != seq)
		return (-1);
	return (handle_echo_reply(ping, packet, len, seq, ttl));
}

// Met a jour les statistiques de latence et affiche la reponse echo.
int	handle_echo_reply(t_ping *ping, uint8_t *packet, ssize_t len, int seq, int ttl)
{
	t_icmp_packet	*icmp_packet;
	struct timeval	sentime;
	struct timeval	now;
	double			rtt_ms;
	
	if (!ping || !packet || len < (ssize_t)sizeof(t_icmp_packet))
		return (-1);
	icmp_packet = (t_icmp_packet *)packet;
	memcpy(&sentime, icmp_packet->payload, sizeof(struct timeval));
	gettimeofday(&now, NULL);
	rtt_ms = (now.tv_sec - sentime.tv_sec) * 1000.0 + (now.tv_usec - sentime.tv_usec) / 1000.0;
	ping->received++;
	if (rtt_ms < ping->rtt_min)
		ping->rtt_min = rtt_ms;
	if (rtt_ms > ping->rtt_max)
		ping->rtt_max = rtt_ms;
	ping->rtt_sum += rtt_ms;
	ping->rtt_sum_sq += rtt_ms * rtt_ms;
	if (ping->verbose)
		printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms (type=%d, code=%d, id=%d)\n",
			len, inet_ntoa(ping->dest_addr.sin_addr), seq, ttl,
			rtt_ms, icmp_packet->header.type, icmp_packet->header.code,
			ntohs(icmp_packet->header.un.echo.id));
	else
		printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
			len, inet_ntoa(ping->dest_addr.sin_addr), seq, ttl, rtt_ms);
	return (0);
}
