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

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

int	main(void) {
	int	sock;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);

	// connect()
	struct sockaddr_in	c_addr_in;

	// get server ip and port
	char	s_ip[128];
	unsigned short	s_port;
	printf("Input server IP (-1 : 127.0.0.1): ");
	scanf("%s", s_ip);
	if (strcmp(s_ip, "-1") == 0)
		strlcpy(s_ip, "127.0.0.1", sizeof(s_ip));
	printf("Input server port: ");
	scanf("%d", &s_port);

	printf("ip: %s, port: %d\n", s_ip, s_port); // test ip and port

	/*
	memset(&c_addr_in, 0x00, sizeof(c_addr_in));
	c_addr_in.sin_family = AF_INET;
	c_addr_in.sin_port = htons(s_port);
	inet_pton(AF_INET, s_ip, (void *)&c_addr_in.sin_addr);
	*/

	// send()
	// recv()
	// close()
	return (0);
}
