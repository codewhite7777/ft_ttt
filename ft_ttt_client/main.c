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

// check protocol
int check_space(char *str)
{
	if (str[1] == ' ' && str[3] == ' ')
		return (true);
	return (false);
}

int check_specified(char checked)
{
	if (checked == 'S' || checked == 'E' \
		|| checked == 'R' || checked == 'O' || checked == 'X')
		return (true);
	return (false);
}

int check_params(char *str)
{
	if (('1' <= str[0] && str[0] <= '3') \
		&& ('1' <= str[2] && str[2] <= '3') \
		&& (str[4] == 'O' || str[4] == 'X'))
		return (true);
	else if (str[0] == '-' && str[2] == '-' && check_specified(str[4]))
		return (true);
	return (false);
}

int	check_protocol(char *str)
{
	if (check_params(str) && check_space(str))
		return (true);
	return (false);
}
// check protocol

void    get_coord_and_turn_from_recv_buf(t_server *s_info, t_game *g_info)
{
    g_info->y = s_info->recv_buf[0] - '0';
    g_info->x = s_info->recv_buf[2] - '0';
    if (s_info->recv_buf[4] == 'O')
        g_info->now_turn = STONE_O;
    else if (s_info->recv_buf[4] == 'X')
        g_info->now_turn = STONE_X;
}

void    set_stone_in_board(t_game *g_info)
{
    if (g_info->now_turn == STONE_O)
        g_info->board[g_info->y - 1][g_info->x - 1] = 'O';
    else if (g_info->now_turn == STONE_X)
        g_info->board[g_info->y - 1][g_info->x - 1] = 'X';
}

void    set_board(t_server *s_info, t_game *g_info)
{
	get_coord_and_turn_from_recv_buf(s_info, g_info);
	set_stone_in_board(g_info);
}

void	process_input(t_server *s_info, t_game *g_info)
{
	// input coord and draw map
	while (1)
    {
        input_coord(&(g_info->y), &(g_info->x));
        if (check_range(g_info->y - 1, g_info->x - 1))
        {
            printf("%s", ERR_MSG_RANGE);
            continue ;
        }
        set_proto_in_send_buf(s_info, g_info);
        send_packet(s_info); // send user input to server
        // printf("under send()\n");
        recv_packet(s_info);
        // printf("under recv()\n");
        if (check_recved_proto(s_info, PROTO_REPOS) == false \
        && check_protocol((char *)(s_info->recv_buf)) == true)
            break ;
    }
    set_board(s_info, g_info);
    print_board(g_info->board);
}

void	process_wait(t_server *s_info, t_game *g_info)
{
	// wait opponent and draw map
	// printf("Waiting for opponent...\n");
	// recv_packet(s_info);
    set_board(s_info, g_info);
    print_board(g_info->board);
}

int check_recv_end(t_server *s_info, t_game *g_info)
{
    if (check_recved_proto(s_info, PROTO_WIN_P1))
        g_info->state = O_WIN;
    else if (check_recved_proto(s_info, PROTO_WIN_P2))
        g_info->state = X_WIN;
    else if (check_recved_proto(s_info, PROTO_DRAW))
        g_info->state = DRAW;
    if (g_info->state == NOT_END)
        return (false);
    return (true);
}

void	run_ttt(t_server *s_info, t_game *g_info) {
	while (1)
	{
		if (g_info->stone == STONE_O)
		{
			process_input(s_info, g_info);
			printf("Waiting for opponent...\n");
			recv_packet(s_info);
            if (check_recv_end(s_info, g_info))
                break ;
			process_wait(s_info, g_info);
		}
		else if (g_info->stone == STONE_X)
		{
			printf("Waiting for opponent...\n");
			recv_packet(s_info);
			process_wait(s_info, g_info);
            if (check_recv_end(s_info, g_info))
                break ;
			process_input(s_info, g_info);
		}
        if (check_recv_end(s_info, g_info))
            break ;
	}
}

void	init_game(t_server *s_info, t_game *g_info) {
	// start protocol
	recv_packet(s_info);
	if (check_recved_proto(s_info, PROTO_START) == true)
	{
		printf("game start!\n");
		print_board(g_info->board);
	}
	else
	{
		printf("no start protocol in buffer...\n");
	}
	// turn protocol
	if (strlen((char *)(s_info->recv_buf)) == 0)
		recv_packet(s_info);
	if (check_recved_proto(s_info, PROTO_O) == true)
	{
		g_info->stone = STONE_O;
		printf("stone: [O]\n");
	}
	else if (check_recved_proto(s_info, PROTO_X) == true)
	{
		g_info->stone = STONE_X;
		printf("stone: [X]\n");
	}
    g_info->state = NOT_END;
	s_info->ptr_game = g_info;
}

int	main(void)
{
	t_server	s_info;
	t_game		g_info;

    clean_infos(&s_info, &g_info);
	config_network(&s_info);
	init_game(&s_info, &g_info);
	run_ttt(&s_info, &g_info);
    close(s_info.sock);
    print_ending(&g_info, g_info.state);
	return (0);
}
