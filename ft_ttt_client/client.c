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
#include "packet_protocol.h"
void	clean_infos(t_server *s_info, t_game *g_info)
{
	memset(&s_info, 0x00, sizeof(s_info));
	memset(&g_info, 0x00, sizeof(g_info));
}

void	config_network(t_server *s_info) {
	int	retval;

	//printf("Input server IP (-1 : 127.0.0.1): ");
	//scanf("%s", s_info->ip);
	// s_info->ip = "-1"; // test
	s_info->ip[0] = '-';
	s_info->ip[1] = '1';
	s_info->ip[2] = 0;
	if (strcmp(s_info->ip, "-1") == 0)
		strlcpy(s_info->ip, "127.0.0.1", sizeof(s_info->ip));
	//printf("Input server port: ");
	//scanf("%hd", &(s_info->port));
	s_info->port = 4242; // test
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
	recv_ret = recv(s_info->sock, (void *)buf, sizeof(buf), 0);
	printf("recvPACKET FUNCTION-    recv_ret: [%zd]\n", recv_ret); // test
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
		printf("value: [%s]\n", s_info->recv_buf);
		if (!strcmp((char *)(s_info->recv_buf), PROTO_WIN_P1))
		{
			print_ending(s_info->ptr_game, O_WIN);
			exit(0);
		}
		else if (!strcmp((char *)(s_info->recv_buf), PROTO_WIN_P2))
		{
			print_ending(s_info->ptr_game, X_WIN);
			exit(0);
		}
		else if (!strcmp((char *)(s_info->recv_buf), PROTO_DRAW))
		{
			print_ending(s_info->ptr_game, DRAW);
			exit(0);
		}
	}
	return ;
}

bool	check_recved_proto(t_server *s_info, char *proto) {
	printf("check_recv FUNCTION recv_buf: [%s]\n", s_info->recv_buf); // test
	if (strncmp((char *)s_info->recv_buf, proto, strlen(proto)) == 0)
	{
		if (strlen((char *)(s_info->recv_buf)) > strlen(proto))
		{
			memmove(s_info->recv_buf, &(s_info->recv_buf[strlen(proto)]), strlen(proto));
			s_info->recv_buf[strlen(proto)] = '\0';
			printf("recv_buf after memmove: [%s]\n", s_info->recv_buf); // test
		}
		else
		{
			clean_buf(s_info->recv_buf);
			printf("recv_buf after clean_buf() in check_recved_proto(): [%s]\n", s_info->recv_buf); // test
		}
		return (true);
	}
	return (false);
}

void	send_packet(t_server *s_info)
{
	int	ret_send;

	printf("sendbuf: [%s]\n", s_info->send_buf);
	ret_send = send(s_info->sock, s_info->send_buf, strlen((char *)(s_info->send_buf)) + 1, 0);
	printf("ret_send: [%d]\n", ret_send);
	if (ret_send == -1)
	{
		printf("send() failed\n");
		exit(1);
	}
	else
	{
		clean_buf(s_info->send_buf);
	}
	return ;
}

void	set_proto_in_send_buf(t_server *s_info, t_game *g_info)
{
	if (strlen((char *)(s_info->send_buf)) > 0)
		clean_buf(s_info->send_buf);
	s_info->send_buf[0] = '0' + g_info->y;
	s_info->send_buf[1] = ' ';
	s_info->send_buf[2] = '0' + g_info->x;
	s_info->send_buf[3] = ' ';
	if (g_info->stone == STONE_O)
		s_info->send_buf[4] = 'O';
	else if (g_info->stone == STONE_X)
		s_info->send_buf[4] = 'X';
	s_info->send_buf[5] = '\0';
}

void	clean_buf(unsigned char *buf) {
	memset((void *)buf, 0x00, sizeof(buf));
}

int check_range(int ny, int nx)
{
    if (ny < 0 || ny >= 3 || nx < 0 || nx >= 3)
        return (1);
    return (0);
}

void input_coord(int *coord_y, int *coord_x)
{
    int y, x;

    printf("Input coordinate to put -> ");
    scanf("%d %d", &y, &x);
    *coord_y = y;
    *coord_x = x;
	
	printf("y: [%d]\n", y);
	printf("x: [%d]\n", x);
}
