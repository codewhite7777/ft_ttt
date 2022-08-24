/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 04:16:46 by mgo               #+#    #+#             */
/*   Updated: 2022/08/24 10:10:33 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"
#include "packet_protocol.h"

void	run_ttt(t_server *s_info, t_game *g_info) {
	int	retval;
	// logic
	if (g_info->turn == TURN_O) {
		// input coordinate
		Input(&(g_info->y), &(g_info->x));
		if (check_range(g_info->y - 1, g_info->x - 1)) {
			printf("%s", ERR_MSG_RANGE);
		}
		// set send msg
		s_info->send_buf[0] = '0' + g_info->x;
		s_info->send_buf[1] = ' ';
		s_info->send_buf[2] = '0' + g_info->y;
		s_info->send_buf[3] = ' ';
		s_info->send_buf[4] = 'O';
		s_info->send_buf[5] = 0;
		retval = send(s_info->sock, s_info->send_buf, strlen((char *)(s_info->send_buf)), 0);
		if (retval == -1) {
			printf("send() failed\n");
		}
	}
	else
	{
		printf("waiting for opponent...\n");
		// wait recv
		retval = recv(s_info->sock, s_info->recv_buf, sizeof(s_info->recv_buf), 0);
		if (retval == -1) {
			printf("recv() failed\n");
		}
	}
}

void	init_game(t_server *s_info, t_game *g_info) {
	// start protocol
	recv_packet(s_info);
	printf("recv_buf: [%s]\n", s_info->recv_buf);
	if (check_recved_proto(s_info, PROTO_START) == true)
	{
		printf("game start!\n");
		ft_print_board(g_info->board);
	}
	else
	{
		printf("no start protocol in buffer...\n");
	}
	// turn protocol
	clean_buf(s_info->recv_buf);
	recv_packet(s_info);
	if (check_recved_proto(s_info, PROTO_O) == true)
	{
		g_info->turn = TURN_O;
		printf("turn: [O]\n");
	}
	else if (check_recved_proto(s_info, PROTO_X) == true)
	{
		g_info->turn = TURN_X;
		printf("turn: [X]\n");
	}
}

int	main(void)
{
	t_server	s_info;
	t_game		g_info;

	config_network(&s_info);
	init_game(&s_info, &g_info);
	run_ttt(&s_info, &g_info);
	// close
	return (0);
}
