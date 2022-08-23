/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 02:38:28 by mgo               #+#    #+#             */
/*   Updated: 2022/08/24 02:38:30 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <stdbool.h>

typedef int SOCKET;

typedef enum e_packet
{
    PACKET_MAX = 1024,
}           t_packet;

typedef struct s_server {
	char				ip[128];
	unsigned short		port;
	struct sockaddr_in	addr_in;
	SOCKET				sock;

	unsigned char	send_buf[PACKET_MAX];
	unsigned char	recv_buf[PACKET_MAX];
}				t_server;

enum e_turn {
	TURN_O,
	TURN_X,
};

typedef struct s_client {
	char	buf[100];
	bool	now_turn;
	char	board[3][3];
	int		visited[3][3];
	int		x, y;
}		t_client;

void ft_print_board(char board[][3]);

void Input(int *posy, int *posx);
int check_range(int ny, int nx);

int	config_network(t_server *s_info);

#endif
