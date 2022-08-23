/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 22:18:21 by alee              #+#    #+#             */
/*   Updated: 2022/08/23 23:16:15 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H
# define CLIENT_MAX 2
# include <netinet/in.h>

typedef int SOCKET;

typedef struct s_server
{
	char				s_ip[128];
	unsigned short		s_port;
	SOCKET				s_listen_sock;
	struct sockaddr_in	s_addr_in;

    //client data
    SOCKET              c_sock[CLIENT_MAX];
    int                 current_client;
}		t_server;

void  accept_client(t_server *p_server);

#endif