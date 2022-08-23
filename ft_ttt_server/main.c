/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:48:47 by alee              #+#    #+#             */
/*   Updated: 2022/08/24 02:45:24 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"
#include "client_session.h"
#include <stdlib.h>
#include <fcntl.h>

int	main(void)
{
	t_server	server;

	bzero(&server, sizeof(server));
	//input ip, port
	printf("input IP (-1 : 127.0.0.1) : ");
	scanf("%s", server.s_ip);
	if (strcmp(server.s_ip, "-1") == 0)
		strlcpy(server.s_ip, "127.0.0.1", sizeof(server.s_ip));
	printf("input Port : ");
	scanf("%hd", &server.s_port);

	//socket(...)
	server.s_listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server.s_listen_sock == -1)
	{
		printf("Err : socket(...) \n");
		exit(1);
	}

	//set nonblock socket opt
	fcntl(server.s_listen_sock, F_SETFL, O_NONBLOCK);
	
	//bind(...)
	memset(&server.s_addr_in, 0x00, sizeof(server.s_addr_in));
	server.s_addr_in.sin_family = AF_INET;
	server.s_addr_in.sin_addr.s_addr = htonl(INADDR_ANY);
	server.s_addr_in.sin_port = htons(server.s_port);
	socklen_t	s_addr_len = sizeof(server.s_addr_in);
	int bind_result = bind(server.s_listen_sock, (const struct sockaddr *)&server.s_addr_in, s_addr_len);
	if (bind_result == -1)
	{
		printf("Err : bind(...) \n");
		exit(1);
	}

	//listen(...)
	int	listen_result = listen(server.s_listen_sock, SOMAXCONN);
	if (listen_result == -1)
	{
		printf("Err : listen(...) \n");
		exit(1);
	}
	
	printf("server ip : %s, port : %d \n", server.s_ip, server.s_port);
	printf("run \n");
	while (1)
	{
		ft_network(&server);
		ft_tictactoe(&server);
		ft_disconnect(&server);
	}
	return (0);
}