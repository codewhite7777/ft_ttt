/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alee <alee@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/23 22:18:18 by alee              #+#    #+#             */
/*   Updated: 2022/08/24 21:06:12 by alee             ###   ########.fr       */
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

// hena
int visited[3][3];
char board[3][3];
int count = 0;
int stone = 0;


int check_already_board(int visited[][3], int y, int x)
{
    if (visited[y - 1][x - 1])
        return (1);
    return (0);
}


int dy[] = {0, 1, 1, 1};
int dx[] = {1, 1, 0, -1};

int check_game_state(int stone, int count)
{
    char now;
    stone == 0 ? (now = type1) : (now = type2);

    printf("now :%c\n", now);
    for (int i = 0 ; i < 3 ; ++i)
    {
        for (int j = 0 ; j < 3 ; ++j)
        {
            // cout << "here is now " << ' ' << i << ' ' << j  << '\n'; 
            if ((i == 0 || j == 0) && board[i][j] == now)
            {
                // cout << "in \n";
                int cnt = 1;
                for (int d = 0 ; d < 4 ; ++d)
                {
                    int ny = i + dy[d];
                    int nx = j + dx[d];
                    
                    // cout << ny << ' ' << nx << '\n';
                    if (check_range(ny, nx) || board[ny][nx] != now)
                        continue ;
                    ny += dy[d];
                    nx += dx[d];
                    if (check_range(ny, nx) || board[ny][nx] != now)
                        continue ;
                    // cout << ny << ' ' << nx << '\n';
                    if (board[ny][nx] != now)
                        continue;
                    else
                    {
                        if (!stone)
                            return P1_WIN;
                        else
                            return P2_WIN;
                    }
                }
            }
        }
    }
    if (count == 9)
        return DRAW;
    return NOT_END;
}

int check_range(int ny, int nx)
{
    if (ny < 0 || ny >= 3 || nx < 0 || nx >= 3)
        return (1);
    return (0);
}
enum type
{
    type1 = (int)'O',
    type2 = (int)'X'
};

enum state
{
    P1_WIN = 0,
    P2_WIN = 1,
    DRAW = 2,
    NOT_END = 3
};

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
    FD_ZERO(&read_set);//읽기 셋 초기화
    FD_ZERO(&write_set);//쓰기 셋 초기화
    
    //listen socket add(read_set)
	FD_SET(p_server->s_listen_sock, &read_set);//서버의 리슨 소켓을 읽기 셋에 등록 -> 추후 서버의 리슨 소켓 이벤트 발생 (클라이언트에서의 connect(...)를 통한 접속 감지) 처리 가능

    //client socket add(read_set, write_set)
    for (int i = 0; i < p_server->current_client; i++)
    {
        FD_SET(p_server->c_session[i].c_sock, &read_set);//현재 서버에서 처리하는 클라이언트 소켓의 읽기 셋 등록
        FD_SET(p_server->c_session[i].c_sock, &write_set);//현재 서버에서 처리하는 클라이언트 소켓의 쓰기 셋 등록
    }

    //set timeout
	struct timeval  time_out;   //select(...)함수(등록된 소켓 셋에서의 이벤트 발생 시, 양수로 반환)에서 함수의 타임 아웃값을 설정 
	time_out.tv_sec = 0;
	time_out.tv_usec = 0;

    int select_result = select(getMaxFD(p_server) + 1, &read_set, &write_set, NULL, &time_out);
    if (select_result > 0)
    {
        //listen socket event
        if (FD_ISSET(p_server->s_listen_sock, &read_set))   //서버의 리슨 소켓에 대한 읽기 이벤트 검사
    		accept_client(p_server);    //접속된 클라이언트의 세션 설정
        //current client socket event
        for (int i = 0; i < p_server->current_client; i++)
        {
            if (FD_ISSET(p_server->c_session[i].c_sock, &read_set))//클라이언트의 소켓에 대한 읽기 이벤트 검사
                recvPacket(p_server->c_session[i].c_sock, i, p_server);
            if (FD_ISSET(p_server->c_session[i].c_sock, &write_set) &&
            strlen((const char *)p_server->c_session[i].s_buf) > 0)//클라이언트의 소켓에 대한 쓰기 이벤트 검사
                sendPacket(p_server->c_session[i].c_sock, i, p_server);
        }
    }
    return ;
}

SOCKET	getMaxFD(t_server *p_server)//select(...)에서 첫번째 파라미터는 서버에 접속한 클라이언트의 가장 큰 값 +1을 필요로 함, 해당 함수는 접속한 클라이언트의 소켓 번호 중 가장 큰 값을 반환
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
	unsigned char	buf[PACKET_MAX];//임시 버퍼

	ssize_t	recv_ret = recv(c_sock, buf, sizeof(buf), 0);
	//disconnect
	if (recv_ret == 0)//클라이언트가 접속 종료된 경우, 서버에 존재하는 데이터 및 소켓 자원 정리
        disconnect(c_sock, sock_idx, p_server);
	else if (recv_ret > 0)
	{
        memcpy(p_server->c_session[sock_idx].r_buf, buf, (size_t)recv_ret);//클라이언트의 recv buffer에 네트워크로 수신된 데이터 복사
        p_server->c_session[sock_idx].r_buf[recv_ret] = '\0';//문자열 처리
	}
    return ;
}

void    sendPacket(SOCKET c_sock, int sock_idx, t_server *p_server)
{
    unsigned char	buf[PACKET_MAX];//임시 버퍼

    memcpy(buf, p_server->c_session[sock_idx].s_buf, strlen((const char *)p_server->c_session[sock_idx].s_buf));//클라이언트 send buffer에 저장되어 있는 값을 임시 버퍼로 복사
	int	send_ret = send(c_sock, (const void *)buf, strlen((const char *)buf), 0);//TCP 프로토콜의 송신 버퍼에 데이터 복사
    // printf("sendPakcet(...), %d byte send \n", send_ret);
	if (send_ret == -1)//복사를 할 수 없는 경우, 클라이언트에서 수신된 데이터를 처리하지 않고 누적되어 있는 케이스가 일반적이므로 클라이언트 접속 해제
	{
        disconnect(c_sock, sock_idx, p_server);
        return ;
	}
	//iter->second->getSendBuf().erase(0, send_ret);
    //tmp
    memset(p_server->c_session[sock_idx].s_buf, 0x00, sizeof(p_server->c_session[sock_idx].s_buf));//버퍼 초기화
    return ;
}

void	ft_tictactoe(t_server *p_server)
{
    if (p_server->current_client == 2)//현재 접속된 클라이언트의 수
    {
        if (p_server->s_status == START)//게임 서버의 처리 상태
        {
            buildPacket(PROTO_START, 0, p_server);//주어진 프로토콜에 대해 패킷을 생성하고, 클라이언트 send buffer에 패킷 정보 삽입
            buildPacket(PROTO_START, 1, p_server);
            p_server->s_status = SET_ROLE;//게임 상태 변경 (대기 -> 시작)
        }
        else if (p_server->s_status == SET_ROLE)
        {
            buildPacket(PROTO_O, 0, p_server);
            buildPacket(PROTO_X, 1, p_server);
            p_server->s_status = PLAY;
        }
        else if (p_server->s_status == PLAY)
        {
            //1) 클라이언트 루프 -> strlen(r_buf) > 0 ? -> 임시 버퍼에 뽑아요
            for (int i = 0; i < p_server->current_client; i++)
            {
                if (strlen(p_server->c_session[i].r_buf) > 0)
                {                        
                    // 1 1 O
                    int y = p_server->c_session[i].r_buf[0] - ('0');
                    int x = p_server->c_session[i].r_buf[2] - ('0');
                    char ch = p_server->c_session[i].r_buf[4];

                    // 이미 존재한다면 리퀘스트 발송할 것
                    if (check_already_board(visited, y, x))
                    {
                        buildPacket(PROTO_REPOS, i, p_server);
                    }
                    else
                    {
                        count++;
                        stone == 0 ? (board[y - 1][x - 1] = type1) : (board[y - 1][x - 1] = type2);
                        visited[y - 1][x - 1] = 1;
                        char buf[12];
                        buf[0] = y + '0';
                        buf[1] = '-';
                        buf[2] = x + '0';
                        buf[3] = '-';
                        buf[4] = ch;
                        buf[5] = 0;
                        
                        // Check game state
                        int ret = check_game_state(stone, count);
                        if (ret !=  NOT_END)
                        {
                            
                            if (ret == P1_WIN)
                            {
                                // strcat(buf, PROTO_WIN_P1);
                                // buildPacket(buf, i, p_server);
                                buildPacket(PROTO_WIN_P1, i, p_server);
                            }
                            else if (ret == P2_WIN)
                            {
                                // strcat(buf, PROTO_WIN_P2);
                                // buildPacket(buf, i, p_server);
                                buildPacket(PROTO_WIN_P2, i, p_server);
                            }
                            else if (ret == DRAW)
                            {
                                // strcat(buf, PROTO_DRAW);
                                // buildPacket(buf, i, p_server);
                                buildPacket(PROTO_DRAW, i, p_server);
                            }
                            // 진행중이 아니라면 누가이겼는지 발송 

                            //hena가 건들임
                            p_server->s_status = END;           
                        }
                        else
                        {
                            buildPacket(buf, 0, p_server);
                            buildPacket(buf, 1, p_server);
                        }
                        // turn change ex) O -> X -> O
                        stone = !stone;
                    }
                }
            }
        }
        else if (p_server->s_status == END)
        {
            // printf("END \n");
            disconnect(0, 0, p_server);
            disconnect(0, 1, p_server);
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
    (void)c_sock;
    p_server->c_session[sock_idx].disconnect_flag = 1;
    return ;
}

void	insertPacket(unsigned char *buf, const char *packet_type)
{
    bzero(buf, PACKET_MAX);
    memcpy(buf, packet_type, strlen(packet_type));
    buf[strlen(packet_type)] = '\0';
    // printf("insert Packet length : [%lu] \n", strlen(packet_type));
    // for (int i = 0; i < strlen((char *)buf); i++)
    //     printf("[%c]", buf[i]);
    return ;
}

void	buildPacket(const char *packet_type, int sock_idx, t_server *p_server)
{
    if (strcmp(packet_type, PROTO_START) == 0 || strcmp(packet_type, PROTO_END) == 0 || 
    strcmp(packet_type, PROTO_REPOS) == 0 || strcmp(packet_type, PROTO_O) == 0 ||
    strcmp(packet_type, PROTO_X) == 0 || strcmp(packet_type, PROTO_WIN_P1) == 0 ||
    strcmp(packet_type, PROTO_WIN_P2) == 0 || strcmp(packet_type, PROTO_DRAW) == 0)
        insertPacket(p_server->c_session[sock_idx].s_buf, packet_type);
    printf("[%d] client sock, s_buf length : %lu \n", p_server->c_session[sock_idx].c_sock, strlen((const char *)p_server->c_session[sock_idx].s_buf));
    return ;
}
