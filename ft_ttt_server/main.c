/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:48:47 by alee              #+#    #+#             */
/*   Updated: 2022/08/23 21:23:18 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

typedef int	SOCKET;

typedef struct s_server
{
	char			s_ip[128];
	unsigned short	s_port;
	SOCKET			s_listen_sock;		
}		t_server;


int	main(void)
{
	t_server	server;
	printf("input IP (-1 : 127.0.0.1) : ");
	scanf("%s", server.s_ip);
	if (strcmp(server.s_ip, "-1") == 0)
		strlcpy(server.s_ip, "127.0.0.1", sizeof(server.s_ip));
	printf("input Port : ");
	scanf("%d", &server.s_port);

	server.s_listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server.s_listen_sock == -1)
	{
		printf("Err : socket(...) \n");
		return (1);
	}
	return (0);
}