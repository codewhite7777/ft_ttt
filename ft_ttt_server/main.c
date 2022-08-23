/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 20:48:47 by alee              #+#    #+#             */
/*   Updated: 2022/08/23 21:05:37 by alee             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

typedef int	SOCKET;

int	main(void)
{
	char			s_ip[128];
	unsigned short	s_port;

	printf("input IP (-1 : 127.0.0.1) : ");
	scanf("%s", s_ip);
	if (strcmp(s_ip, "-1") == 0)
		strlcpy(s_ip, "127.0.0.1", sizeof(s_ip));
	printf("input Port : ");
	scanf("%d", &s_port);

	printf("ip : %s, port : %d \n", s_ip, s_port);


	return (0);
}