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

void	draw_map()
{
	
}

void	process_input(t_server *s_info, t_game *g_info)
{
	// input coord and draw map
	printf("Input coordinate to put -> ");
	input_coord(&(g_info->y), &(g_info->x));
	if (check_range(g_info->y, g_info->x)) {
		printf("%s", ERR_MSG_RANGE);
	}
	set_proto_in_send_buf(s_info, g_info);
	send_packet(s_info);
	recv_packet(s_info);
	//draw_map()
}

void	process_input(t_server *s_info, t_game *g_info)
{
	// wait opponent and draw map
	printf("Waiting for opponent...\n");
	recv_packet(s_info);
	//draw_map();
}

void	run_ttt(t_server *s_info, t_game *g_info) {
	int	retval;

	// logic
	if (g_info->turn == TURN_O)
	{
		// process_input
		process_input(s_info, g_info);
		
		// wait opponent and draw map
	}
	else if (g_info->turn == TURN_X)
	{
		// process_wait
		process_wait(s_info, g_info);
		

	}
	else
	{
		printf("unknown turn type...\n");
		exit(1);
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
	//clean_buf(s_info->recv_buf);
	if (strlen((char *)(s_info->recv_buf)) == 0)
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
