/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_session.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 22:18:50 by alee              #+#    #+#             */
/*   Updated: 2022/08/23 22:23:27 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_SESSION_H
# define CLIENT_SESSION_H

typedef int SOCKET;

typedef struct s_client_session
{
	SOCKET				c_sock;
	struct sockaddr_in	c_addr_in;
	unsigned short		c_port;
}	t_client_session;

#endif