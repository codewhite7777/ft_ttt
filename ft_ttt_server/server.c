/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 22:18:18 by alee              #+#    #+#             */
/*   Updated: 2022/08/24 05:19:25 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "packet_protocol.h"

void  accept_client(t_server *p_server)
{
    printf("accept client(...) \n");
    struct sockaddr_in  c_addr_in;
    socklen_t           c_socklen;

    c_socklen = sizeof(c_addr_in);
    SOCKET  client_sock = accept(p_server->s_listen_sock, (struct sockaddr *)&c_addr_in, &c_socklen);
    if (p_server->current_client >= 2)
    {
        printf("[%d] client disconnected \n", client_sock);
        close(client_sock);
    }
    else
    {
        p_server->c_session[p_server->current_client].c_sock = client_sock;
        p_server->current_client += 1;
        printf("new client connect \n");
        printf("current client : %d \n", p_server->current_client);
    }
    return ;
}

void	ft_network(t_server *p_server)
{
    fd_set  read_set;
    fd_set  write_set;

    //FD_ZERO
    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    
    //listen socket add(read_set)
	FD_SET(p_server->s_listen_sock, &read_set);

    //client socket add(read_set, write_set)
    for (int i = 0; i < p_server->current_client; i++)
    {
        FD_SET(p_server->c_session[i].c_sock, &read_set);
        // FD_SET(p_server->c_session[i].c_sock, &write_set);
    }

    //set timeout
	struct timeval  time_out;
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;

    int select_result = select(getMaxFD(p_server) + 1, &read_set, NULL, NULL, &time_out);
    if (select_result > 0)
    {
        //listen socket event
        if (FD_ISSET(p_server->s_listen_sock, &read_set))
    		accept_client(p_server);
        //current client socket event
        for (int i = 0; i < p_server->current_client; i++)
        {
            if (FD_ISSET(p_server->c_session[i].c_sock, &read_set))
                recvPacket(p_server->c_session[i].c_sock, i, p_server);
            // if (FD_ISSET(p_server->c_session[i].c_sock, &write_set) &&
            // strlen((const char *)p_server->c_session[i].s_buf) > 0)
                sendPacket(p_server->c_session[i].c_sock, i, p_server);
        }
    }
    return ;
}

SOCKET	getMaxFD(t_server *p_server)
{
	SOCKET	max_fd = p_server->s_listen_sock;
	for (int i = 0; i < p_server->current_client; i++)
	{
		if (max_fd < p_server->c_session[i].c_sock)
			max_fd = p_server->c_session[i].c_sock;
	}
	return (max_fd);
}

void	recvPacket(SOCKET c_sock, int sock_idx, t_server *p_server)
{
	unsigned char	buf[PACKET_MAX];
	ssize_t	recv_ret = recv(c_sock, buf, sizeof(buf), 0);
	//disconnect
	if (recv_ret == 0)
        disconnect(c_sock, sock_idx, p_server);
	else if (recv_ret > 0)
	{
        memcpy(p_server->c_session[sock_idx].r_buf, buf, (size_t)recv_ret);
        p_server->c_session[sock_idx].r_buf[recv_ret] = '\0';
	}
    return ;
}

void    sendPacket(SOCKET c_sock, int sock_idx, t_server *p_server)
{
    unsigned char	buf[PACKET_MAX];

    memcpy(buf, p_server->c_session[sock_idx].s_buf, strlen((const char *)p_server->c_session[sock_idx].s_buf));
	int	send_ret = send(c_sock, (const void *)buf, strlen((const char *)buf), 0);
	if (send_ret == -1)
	{
        disconnect(c_sock, sock_idx, p_server);
        return ;
	}
	//iter->second->getSendBuf().erase(0, send_ret);
    //tmp
    memset(p_server->c_session[sock_idx].s_buf, 0x00, sizeof(p_server->c_session[sock_idx].s_buf));
    return ;
}

void	ft_tictactoe(t_server *p_server)
{
    if (p_server->current_client == 2)
    {
        if (p_server->s_status == START)
        {
            buildPacket(PROTO_START, 0, p_server);
            buildPacket(PROTO_START, 1, p_server);
            broadcast(p_server);
            buildPacket(PROTO_O, 0, p_server);
            buildPacket(PROTO_X, 1, p_server);
            broadcast(p_server);
            p_server->s_status = PLAY;
        }
        else if (p_server->s_status == PLAY)
        {
            printf("PLAY \n");
        }
        else if (p_server->s_status == END)
        {
            printf("END \n");
        }
    }
    return ;
}

void	ft_disconnect(t_server *p_server)
{
    for (int i = 0; i < p_server->current_client; i++)
    {
        if (p_server->c_session[i].disconnect_flag == 1)
        {
            close(p_server->c_session[i].c_sock);
            p_server->current_client -= 1;
            memset((void *)&p_server->c_session[i], 0x00, sizeof(p_server->c_session[i]));
        }
    }
    return ;
}

void	disconnect(SOCKET c_sock, int sock_idx, t_server *p_server)
{
    p_server->c_session[sock_idx].disconnect_flag = 1;
    return ;
}

void	insertPacket(unsigned char *buf, const char *packet_type)
{
    memcpy(buf, packet_type, strlen(packet_type));
    return ;
}

void	buildPacket(const char *packet_type, int sock_idx, t_server *p_server)
{
    if (strcmp(packet_type, PROTO_START) == 0 || strcmp(packet_type, PROTO_END) == 0)
        insertPacket(p_server->c_session[sock_idx].s_buf, packet_type);
    return ;
}

void	broadcast(t_server *p_server)
{
    for (int i = 0; i < p_server->current_client; i++)
        sendPacket(p_server->c_session[i].c_sock, i, p_server);
    return ;
}