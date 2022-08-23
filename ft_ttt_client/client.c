/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:48:44 by mgo               #+#    #+#             */
/*   Updated: 2022/08/23 20:48:45 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

int	config_network(t_server *s_info) {
	int	retval;

	printf("Input server IP (-1 : 127.0.0.1): ");
	scanf("%s", s_info->ip);
	if (strcmp(s_info->ip, "-1") == 0)
		strlcpy(s_info->ip, "127.0.0.1", sizeof(s_info->ip));
	printf("Input server port: ");
	scanf("%hd", &(s_info->port));
	printf("ip: %s, port: %d\n", s_info->ip, s_info->port); // test ip and port
	s_info->sock = socket(AF_INET, SOCK_STREAM, 0);
	s_info->addr_in.sin_family = AF_INET;
	s_info->addr_in.sin_port = htons(s_info->port);
	inet_pton(AF_INET, s_info->ip, (void *)&(s_info->addr_in.sin_addr));
	retval = connect(s_info->sock, (const struct sockaddr *)(&(s_info->addr_in)), sizeof(s_info->addr_in));
	printf("connect : %d \n", retval);
	return (retval);
}
