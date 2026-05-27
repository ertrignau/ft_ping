/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eric <eric@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/20 11:42:29 by ertrigna          #+#    #+#             */
/*   Updated: 2026/05/27 18:48:39 by eric             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

void	set_socket_timeout(int sockfd, int seconds);
void	print_usage(char *progname);
int		parse_arguments(int ac, char *av[], t_ping *ping);
int		handle_count(t_ping *ping, char **av, int *i, int ac);
int		handle_interval(t_ping *ping, char **av, int *i, int ac);
int		handle_timeout(t_ping *ping, char **av, int *i, int ac);
int		handle_ttl(t_ping *ping, char **av, int *i, int ac);
int		is_valid_integer(const char *str);
int		is_valid_float(const char *str);
int		is_hostname_like(const char *str);
void	print_stat(t_ping *ping);

#endif