/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgo <mgo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/24 04:16:46 by mgo               #+#    #+#             */
/*   Updated: 2022/08/24 04:16:47 by mgo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "client.h"

const char * errorMessage[2] = {
    "not in range (1 ~ 3)\n",
    "already in board\n"
};

int	main(void) {
	t_server	s_info;
	t_client	c_info;
	int			retval;

	memset(&s_info, 0x00, sizeof(s_info));
	memset(&c_info, 0x00, sizeof(c_info));

	// config_network()
	retval = config_network(&s_info);
	if (retval == -1) {
		printf("connect() in config_network() failed\n");
	} else {
		printf("connected with server\n");

		// wait for starting
		retval = recv(s_info.sock, (void *)s_info.recv_buf, sizeof(s_info.recv_buf), 0);
		if (retval == -1)
			printf("recv() failed\n");
		if (strcmp((char *)s_info.recv_buf, "- - S") == 0) {
			printf("game start!\n");
			// print default map
			ft_print_board(c_info.board);
		}

		retval = recv(s_info.sock, s_info.recv_buf, sizeof(s_info.recv_buf), 0);
		if (retval == -1)
			printf("recv() failed\n");
		printf("recv retval: [%d]\n", retval);
		printf("buf: [%s]\n", s_info.recv_buf);
		if (strcmp((char *)s_info.recv_buf, "- - O") == 0) {
			c_info.now_turn = TURN_O;
			printf("turn: [O]\n");
		}
		if (strcmp((char *)s_info.recv_buf, "- - X") == 0) {
			c_info.now_turn = TURN_X;
			printf("turn: [X]\n");
		}

		if (c_info.now_turn == TURN_O) {
			// input coordinate
			Input(&(c_info.y), &(c_info.x));
			if (check_range(c_info.y - 1, c_info.x - 1)) {
            	printf("%s", errorMessage[0]);
			}
			// set send msg
			s_info.send_buf[0] = '0' + c_info.x;
			s_info.send_buf[1] = ' ';
			s_info.send_buf[2] = '0' + c_info.y;
			s_info.send_buf[3] = ' ';
			s_info.send_buf[4] = 'O';
			s_info.send_buf[5] = 0;
			retval = send(s_info.sock, s_info.send_buf, strlen((char *)(s_info.send_buf)), 0);
			if (retval == -1) {
				printf("send() failed\n");
			}
		} else {
			printf("waiting for opponent...\n");
			// wait recv
			retval = recv(s_info.sock, s_info.recv_buf, sizeof(s_info.recv_buf), 0);
			if (retval == -1) {
				printf("recv() failed\n");
			}
		}	
	}
	// close()
	while (1);

	return (0);
}
