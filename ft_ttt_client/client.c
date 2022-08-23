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

typedef int SOCKET;

typedef struct s_server {
	char			ip[128];
	unsigned short	port;

	struct sockaddr_in	c_addr_in;
}				t_server;

int	main(void) {
	t_server	s_info;

	// get server ip and port
	printf("Input server IP (-1 : 127.0.0.1): ");
	scanf("%s", s_info.ip);
	if (strcmp(s_info.ip, "-1") == 0)
		strlcpy(s_info.ip, "127.0.0.1", sizeof(s_info.ip));
	printf("Input server port: ");
	scanf("%d", &(s_info.port));
	printf("ip: %s, port: %d\n", s_info.ip, s_info.port); // test ip and port

	// socket()
	SOCKET	c_sock;
	c_sock = socket(AF_INET, SOCK_STREAM, 0);

	// connect()
	memset(&(s_info.c_addr_in), 0x00, sizeof(s_info.c_addr_in));
	s_info.c_addr_in.sin_family = AF_INET;
	s_info.c_addr_in.sin_port = htons(s_info.port);
	inet_pton(AF_INET, s_info.ip, (void *)&(s_info.c_addr_in.sin_addr));

	int	connect_result = connect(c_sock, (const struct sockaddr *)(&(s_info.c_addr_in)), sizeof(s_info.c_addr_in));
	printf("connect_result: [%d]\n", connect_result);
	// send()


	// recv()
	// close()
	return (0);
}
