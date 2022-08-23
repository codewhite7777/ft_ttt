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
#include <stdbool.h>

typedef int SOCKET;

typedef struct s_server {
	char				ip[128];
	unsigned short		port;
	struct sockaddr_in	addr_in;
	SOCKET				sock;
}				t_server;

enum e_turn {
	TURN_O,
	TURN_X,
};

typedef struct s_client {
	bool	now_turn;
}		t_client;

int	main(void) {
	t_server	s_info;
	t_client	c_info;

	char buf[100];

	memset(&s_info, 0x00, sizeof(s_info));
	// get server ip and port
	printf("Input server IP (-1 : 127.0.0.1): ");
	scanf("%s", s_info.ip);
	if (strcmp(s_info.ip, "-1") == 0)
		strlcpy(s_info.ip, "127.0.0.1", sizeof(s_info.ip));
	printf("Input server port: ");
	scanf("%hd", &(s_info.port));
	printf("ip: %s, port: %d\n", s_info.ip, s_info.port); // test ip and port
	// socket()
	s_info.sock = socket(AF_INET, SOCK_STREAM, 0);

	// connect()
	//memset(&(s_info.addr_in), 0x00, sizeof(s_info.addr_in));
	s_info.addr_in.sin_family = AF_INET;
	s_info.addr_in.sin_port = htons(s_info.port);
	inet_pton(AF_INET, s_info.ip, (void *)&(s_info.addr_in.sin_addr));

	int	retval = connect(s_info.sock, (const struct sockaddr *)(&(s_info.addr_in)), sizeof(s_info.addr_in));
	if (retval == -1) {
		// error handling
	} else {
		printf("connected with server\n");
		retval = recv(s_info.sock, buf, sizeof(buf), 0);
		printf("recv retval: [%d]\n", retval);
		printf("buf: [%s]\n", buf);
		if (strcmp(buf, "0 0 O") == 0)
			c_info.now_turn = TURN_O;
		if (strcmp(buf, "0 0 X") == 0)
			c_info.now_turn = TURN_X;

		// print default map
		
	}

	// close()
	while (1);

	return (0);
}
