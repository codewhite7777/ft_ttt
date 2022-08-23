/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 22:18:18 by alee              #+#    #+#             */
/*   Updated: 2022/08/23 23:35:09 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

void  accept_client(t_server *p_server)
{
    struct sockaddr_in  c_addr_in;
    socklen_t           c_socklen;

    c_socklen = sizeof(c_addr_in);
    SOCKET  client_sock = accept(p_server->s_listen_sock, (struct sockaddr *)&c_addr_in, &c_socklen);
    if (client_sock == -1)
    {
        printf("Err : socket(...) \n");
        exit(1);
    }
    if (p_server->current_client > 2)
        close(client_sock);
    else
    {
        int send_result = send(client_sock, "ft_ttt", 6, 0);
        p_server->current_client += 1;
        p_server->c_sock[p_server->current_client] = client_sock;
        printf("%d client , %dbyte send \n", p_server->c_sock[p_server->current_client], send_result);
    }
    return ;
}
