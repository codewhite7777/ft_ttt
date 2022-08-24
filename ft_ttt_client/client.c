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

void	clean_infos(t_server *s_info, t_game *g_info)
{
	memset(&s_info, 0x00, sizeof(s_info));
	memset(&g_info, 0x00, sizeof(g_info));
}

void	config_network(t_server *s_info) {
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
	printf("connect retval: [%d]\n", retval);
	if (retval == -1)
	{
		printf("connect() in config_network() failed\n");
		exit(1);
	}
	else
	{
		printf("Successfully connected with server\n");
	}
	return ;
}

void	recv_packet(t_server *s_info) {
	ssize_t			recv_ret;
	unsigned char	buf[PACKET_MAX];

	if (strlen((char *)(s_info->recv_buf)) != 0)
		clean_buf(s_info->recv_buf);
	printf("recv_buf before recv(): [%s]\n", s_info->recv_buf);
	recv_ret = recv(s_info->sock, (void *)buf, sizeof(buf), 0);
	printf("recv_ret: [%zd]\n", recv_ret);
	if (recv_ret == -1)
	{
		printf("recv() failed\n");
		exit(1);
	}
	else if (recv_ret == 0)
	{
		printf("exit(1) after recv()\n");
		exit(1);
	}
	else if (recv_ret > 0)
	{
		memcpy(s_info->recv_buf, buf, (size_t)recv_ret);
        s_info->recv_buf[recv_ret] = '\0';
	}
	return ;
}

bool	check_recved_proto(t_server *s_info, char *proto) {
	printf("recv_buf: [%s]\n", s_info->recv_buf);
	if (strncmp((char *)s_info->recv_buf, proto, strlen(proto)) == 0)
	{
		if (strlen((char *)(s_info->recv_buf)) > strlen(proto))
		{
			memmove(s_info->recv_buf, &(s_info->recv_buf[strlen(proto)]), strlen(proto));
			s_info->recv_buf[strlen(proto)] = '\0';
			printf("recv_buf after memmove: [%s]\n", s_info->recv_buf);
		}
		else
		{
			clean_buf(s_info->recv_buf);
		}
		return (true);
	}
	return (false);
}

void	clean_buf(unsigned char *buf) {
	memset((void *)buf, 0x00, sizeof(buf));
}
