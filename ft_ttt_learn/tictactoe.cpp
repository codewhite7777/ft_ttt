#include <stdio.h>
#include <unistd.h>
#include <string.h>


int visited[3][3];
char board[3][3];

// default print
void ft_print_board(char board[][3])
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (!board[i][j])
                printf("@");
            else
                printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// upgrade print

// check board
int check_already_board(int visited[][3], int y, int x)
{
    if (visited[y - 1][x - 1])
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

const char * err1 = "already in board\n";
const char * err2 = "not in range (1 ~ 3)\n";

int dy[] = {0, 1, 1, 1};
int dx[] = {1, 1, 0, -1};

int check_range(int ny, int nx)
{
    // printf("%d %d\n", ny, nx);
    if (ny < 0 || ny >= 3 || nx < 0 || nx >= 3)
        return (1);
    return (0);
}

int check_win(bool stone, int count)
{
    char now;
    stone == 0 ? now = type1 : now = type2;

    printf("now :%c\n", now);
    for (int i = 0 ; i < 3 ; ++i)
    {
        for (int j = 0 ; j < 3 ; ++j)
        {
            // cout << "here is now " << ' ' << i << ' ' << j  << '\n'; 
            if (board[i][j] == now)
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


void Input(int *posy, int *posx)
{
    int y, x;
    scanf("%d %d", &y, &x);
    *posy = y;
    *posx = x;
}

const char * message1[3] = {
    "P1 Win\n",
    "P2 Win\n",
    "DRAW\n",
};
const char * message2[3] = {
    "P1 Lose\n",
    "P2 Lose\n",
    "DRAW\n",
};

int main()
{
    bool stone = 0;
    int count = 0;
    int flag;
    int y, x;


    while (true)
    {
        
        Input(&y, &x);
        if (check_range(y - 1, x - 1))
            printf("not in range(1 ~ 3)\n");
        else
        {
            if (check_already_board(visited, y, x))
                // write(2, err1, strlen(err1));
                printf("%s", err1);
            else
            {
                count++;
                stone == 0 ? board[y - 1][x - 1] = type1 : board[y - 1][x - 1] = type2;
                visited[y - 1][x - 1] = 1;
                ft_print_board(board);

                // win check logic
                int ret = check_win(stone, count);
                printf("ret: %d\n", ret);
                if (ret !=  NOT_END)
                {
                    flag = ret;
                    break;
                }

                // state change
                stone = !stone;
            }
        }
    }

    if (flag == P1_WIN)
        printf("%s",message1[flag]);
    else if (flag == P2_WIN)
        printf("%s",message1[flag]);
    else
        printf("%s",message1[flag]);
}