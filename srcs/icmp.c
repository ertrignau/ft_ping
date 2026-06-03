/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eric <eric@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 17:17:56 by ertrigna          #+#    #+#             */
/*   Updated: 2026/05/30 09:51:12 by eric             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ping.h"

// Calcule le checksum ICMP pour un bloc de donnees brut.
// Permet de verifier l'integrite des paquets ICMP envoyes et recus. Retourne le checksum
// de 16 bits a inclure dans l'en-tete ICMP.

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
// @ping: contexte de l'execution (socket, stats, etc.)
// @buffer: tampon pour stocker les donnees recues
// @size: taille du tampon en octets
// Retourne le nombre d'octets recus, 0 si aucun paquet disponible, ou -1 en cas d'erreur (autre que EAGAIN/EWOULDBLOCK).

ssize_t recv_packet(t_ping *ping, uint8_t *buffer, size_t size)
{
	struct sockaddr_in	addr;
	socklen_t			addrlen = sizeof(addr);
	ssize_t				bytes = recvfrom(ping->sockfd, buffer, size, 0, (struct sockaddr *)&addr, &addrlen);
	
	if (bytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return (0);
		if (errno == EINTR)
			return (-1);
		perror("recvfrom() failed");
		return (-1);
	}
	
	return (bytes);
}

// Verifie l'en-tete IP puis extrait la partie ICMP du paquet recu.
// @ping: contexte de l'execution (stats, flags, etc.)
// @buf: tampon contenant les donnees recues (debut de l'en-tete IP)
// @len: taille totale des donnees recues (octets)
// Retourne 0 si le paquet a ete traite avec succes, ou -1 en cas d'erreur (paquet trop court, type ICMP non ECHO REPLY, id/seq mismatch).

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

// parse_icmp — valider et traiter une réponse ICMP ECHO
// @ping: contexte de l'exécution (stats, flags, etc.)
// @packet: pointeur sur le bloc ICMP (début de l'en-tête ICMP)
// @len: taille du bloc ICMP (octets)
// @seq: numéro de séquence attendu (ordre hôte)
// @ttl: TTL extrait de l'en-tête IP recevante
// Vérifie la longueur, le type (ICMP_ECHOREPLY), l'id (doit matcher getpid())
 //et la séquence. Si tout est OK, appelle handle_echo_reply() qui met à jour
// les statistiques et affiche la réponse. Retourne 0 si traité, -1 sinon.

int	parse_icmp(t_ping *ping, uint8_t *packet, ssize_t len, int seq, int ttl)
{
	t_icmp_packet *icmp_packet;

	if (!ping || !packet || len < (ssize_t)sizeof(t_icmp_packet))
		return (-1);
	icmp_packet = (t_icmp_packet *)packet;
	if (icmp_packet->header.type != ICMP_ECHOREPLY)
	{
		return (-1);
	}
	if (ntohs(icmp_packet->header.un.echo.id) != getpid())
		return (-1);
	if (ntohs(icmp_packet->header.un.echo.sequence) != seq)
		return (-1);
	return (handle_echo_reply(ping, packet, len, seq, ttl));
}

// Met a jour les statistiques de latence et affiche la reponse echo reply
// @ping: contexte de l'exécution (stats, flags, etc.)
// @packet: pointeur sur le bloc ICMP (début de l'en-tête
// @len: taille du bloc ICMP (octets)
// @seq: numéro de séquence de la requête correspondante
// @ttl: TTL extrait de l'en-tête IP recevante
// Calcule le RTT en ms, met à jour les stats (min, max, sum, ...)

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
	if (ping->debug)
	{
		printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.3f ms\n",
			len, inet_ntoa(ping->dest_addr.sin_addr), seq, ttl, rtt_ms);
		printf("  ICMP Echo Reply Details:\n");
		printf("    Type: %d (ICMP_ECHOREPLY)\n", icmp_packet->header.type);
		printf("    Code: %d\n", icmp_packet->header.code);
		printf("    Checksum: 0x%04x\n", ntohs(icmp_packet->header.checksum));
		printf("    ID: %d\n", ntohs(icmp_packet->header.un.echo.id));
		printf("    Sequence: %d\n", ntohs(icmp_packet->header.un.echo.sequence));
	}
	else
	{
		char formatted_rtt[20];
		snprintf(formatted_rtt, sizeof(formatted_rtt), "%.3f", rtt_ms);
		printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%s ms\n",
			len, inet_ntoa(ping->dest_addr.sin_addr), seq, ttl, formatted_rtt);
	}
	return (0);
}
