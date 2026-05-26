/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ertrigna <ertrigna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:42:31 by ertrigna          #+#    #+#             */
/*   Updated: 2026/01/22 15:38:33 by ertrigna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ICMP_H
#define ICMP_H

#include <netinet/ip_icmp.h>

#define ICMP_PACKET_SIZE 64

typedef struct s_ping t_ping;

typedef struct s_icmp_packet
{
	struct icmphdr	header;
	char			payload[ICMP_PACKET_SIZE - sizeof(struct icmphdr)];
} t_icmp_packet;

void		init_tcmp_packet(t_icmp_packet *packet, int seq);
uint16_t	icmp_checksum(void *data, int len);
ssize_t 	recv_packet(t_ping *ping, uint8_t *buffer, size_t size);
int			parse_packet(t_ping *ping, uint8_t *buf, ssize_t len);
int			parse_icmp(t_ping *ping, uint8_t *packet, ssize_t len, int seq, int ttl);
int			handle_echo_reply(t_ping *ping, uint8_t *packet, ssize_t len, int seq, int ttl);

#endif