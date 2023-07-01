#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include <cstring>


using namespace sf;


int size = 56;

Sprite v[32];
int board[8][8] =
{
    -1,-2,-3,-4,-5,-3,-2,-1,
    -6,-6,-6,-6,-6,-6,-6,-6,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,
    6,6,6,6,6,6,6,6,
    1,2,3,4,5,3,2,1
};

struct moves
{
    char moveWhite[10];
    char moveBlack[10];
}moves[1000];
int move = 1;

int castles = 0;
int turn = 1;
int nr_pieces = 32;
int is_check = 0;
void loadPosition(bool ok)
{
    int k = 0;
    for(int i=0; i<8; i++)
        for (int j = 0; j < 8; j++)
        {
            int n = board[i][j];
            if (!n)
            {
                continue;
            }
           
            int x = abs(n) - 1;
            int y = n > 0 ? 1 : 0;
            v[k].setTextureRect(IntRect(size * x, size * y, size, size));
            if(ok)
                v[k].scale(Vector2f(2, 2));
            v[k].setPosition(size * 2 * j , size * 2 * i );
            k++;
        }
    nr_pieces = k;
}
int sprite_piece_moved = -1;
int piece_moved ;
struct positions
{
    int x, y;
}old_pos, new_pos;

int legal_board[8][8] ;
void UpdateCheckBoard(int legal_in_board[8][8], int in_board[8][8])
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (in_board[i][j] > 0) ///white
            {
                switch (in_board[i][j])
                {
                case 1: ///rook
                {
                    if(j!=7)
                    for (int y = j + 1; y < 8; y++)
                    {
                        if(legal_in_board[i][y]!=2)
                        legal_in_board[i][y] = (legal_in_board[i][y] == 0 ? 1 : (legal_in_board[i][y] == -1 ? 2 : 1));
                        if (in_board[i][y] != 0)
                            break;
                    }
                    if(j!=0)
                    for (int y = j - 1; y >= 0; y--)
                    {
                        if (legal_in_board[i][y] != 2)
                        legal_in_board[i][y] = (legal_in_board[i][y] == 0 ? 1 : (legal_in_board[i][y] == -1 ? 2 : 1));
                        if (in_board[i][y] != 0)
                            break;
                    }
                    if(i!=7)
                    for (int y = i + 1; y < 8; y++)
                    {
                        if (legal_in_board[y][j] != 2)
                        legal_in_board[y][j] = (legal_in_board[y][j] == 0 ? 1 : (legal_in_board[y][j] == -1 ? 2 : 1));
                        if (in_board[y][j] != 0)
                            break;
                    }
                    if(i!=0)
                    for (int y = i - 1; y >= 0; y--)
                    {
                        if (legal_in_board[y][j] != 2)
                        legal_in_board[y][j] = (legal_in_board[y][i] == 0 ? 1 : (legal_in_board[y][j] == -1 ? 2 : 1));
                        if (in_board[y][j] != 0)
                            break;
                    }
                    break;
                }
                case 2: ///knight
                {
                    if(i+2<8 && j+1<8 && legal_in_board[i+2][j+1]!=2)
                    legal_in_board[i + 2][j + 1] = (legal_in_board[i + 2][j + 1] == 0 ? 1 : (legal_in_board[i + 2][j + 1] == -1 ? 2 : 1));
                    if (i + 2 < 8 && j - 1 >= 0 && legal_in_board[i + 2][j - 1] != 2)
                    legal_in_board[i + 2][j - 1] = (legal_in_board[i + 2][j - 1] == 0 ? 1 : (legal_in_board[i + 2][j - 1] == -1 ? 2 : 1));
                    if (i - 2 >= 0 && j + 1 < 8 && legal_in_board[i - 2][j + 1] != 2)
                    legal_in_board[i - 2][j + 1] = (legal_in_board[i - 2][j + 1] == 0 ? 1 : (legal_in_board[i - 2][j + 1] == -1 ? 2 : 1));
                    if (i - 2 >=0  && j - 1 >= 0 && legal_in_board[i - 2][j - 1] != 2)
                    legal_in_board[i - 2][j - 1] = (legal_in_board[i - 2][j - 1] == 0 ? 1 : (legal_in_board[i - 2][j - 1] == -1 ? 2 : 1));
                    if (i + 1 < 8 && j + 2 < 8 && legal_in_board[i + 1][j + 2] != 2)
                    legal_in_board[i + 1][j + 2] = (legal_in_board[i + 1][j + 2] == 0 ? 1 : (legal_in_board[i + 1][j + 2] == -1 ? 2 : 1));
                    if (i + 1 < 8 && j - 2 >= 0 && legal_in_board[i + 1][j - 2] != 2)
                    legal_in_board[i + 1][j - 2] = (legal_in_board[i + 1][j - 2] == 0 ? 1 : (legal_in_board[i + 1][j - 2] == -1 ? 2 : 1));
                    if (i - 1 >= 0 && j + 2 < 8 && legal_in_board[i - 1][j + 2] != 2)
                    legal_in_board[i - 1][j + 2] = (legal_in_board[i - 1][j + 2] == 0 ? 1 : (legal_in_board[i - 1][j + 2] == -1 ? 2 : 1));
                    if (i-1>=0 && j-2 >= 0 && legal_in_board[i - 1][j - 2] != 2)
                    legal_in_board[i - 1][j - 2] = (legal_in_board[i - 1][j - 2] == 0 ? 1 : (legal_in_board[i - 1][j - 2] == -1 ? 2 : 1));
                    break;
                }
                case 3: ///bishop
                {
                    if (i != 7)
                    {
                        if (j != 7)
                        {
                            int x = i + 1, y = j+1;
                            while (x < 8 && y < 8)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? 1 : (legal_in_board[x][y] == -1 ? 2 : 1));
                                if (in_board[x][y] != 0)
                                    break;
                                x++; 
                                y++;
                            }
                        }
                        if(j!=0)
                        {
                            int x = i + 1, y = j - 1;
                            while (x < 8 && y >=0)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? 1 : (legal_in_board[x][y] == -1 ? 2 : 1));
                                if (in_board[x][y] != 0)
                                    break;
                                x++;
                                y--;
                            }
                        }
                    }
                    if (i != 0)
                    {
                        if(j!=7)
                        {
                            int x = i - 1, y = j + 1;
                            while (x >=0 && y < 8)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? 1 : (legal_in_board[x][y] == -1 ? 2 : 1));
                                if (in_board[x][y] != 0)
                                    break;
                                x--;
                                y++;
                            }
                        }
                        if(j!=0)
                        {
                            int x = i - 1, y = j - 1;
                            while (x >=0 && y >=0)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? 1 : (legal_in_board[x][y] == -1 ? 2 : 1));
                                if (in_board[x][y] != 0)
                                    break;
                                x--;
                                y--;
                            }
                        }
                    }

                    break;
                }
                case 4: ///queen
                {
                    if (j != 7)
                        for (int y = j + 1; y < 8; y++)
                        {
                            if (legal_in_board[i][y] != 2)
                                legal_in_board[i][y] = (legal_in_board[i][y] == 0 ? 1 : (legal_in_board[i][y] == -1 ? 2 : 1));
                            if (in_board[i][y] != 0)
                                break;
                        }
                    if (j != 0)
                        for (int y = j - 1; y >= 0; y--)
                        {
                            if (legal_in_board[i][y] != 2)
                                legal_in_board[i][y] = (legal_in_board[i][y] == 0 ? 1 : (legal_in_board[i][y] == -1 ? 2 : 1));
                            if (in_board[i][y] != 0)
                                break;
                        }
                    if (i != 7)
                        for (int y = i + 1; y < 8; y++)
                        {
                            if (legal_in_board[y][j] != 2)
                                legal_in_board[y][j] = (legal_in_board[y][j] == 0 ? 1 : (legal_in_board[y][j] == -1 ? 2 : 1));
                            if (in_board[y][j] != 0)
                                break;
                        }
                    if (i != 0)
                        for (int y = i - 1; y >= 0; y--)
                        {
                            if (legal_in_board[y][j] != 2)
                                legal_in_board[y][j] = (legal_in_board[y][i] == 0 ? 1 : (legal_in_board[y][j] == -1 ? 2 : 1));
                            if (in_board[y][j] != 0)
                                break;
                        }

                    if (i != 7)
                    {
                        if (j != 7)
                        {
                            int x = i + 1, y = j + 1;
                            while (x < 8 && y < 8)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? 1 : (legal_in_board[x][y] == -1 ? 2 : 1));
                                if (in_board[x][y] != 0)
                                    break;
                                x++;
                                y++;
                            }
                        }
                        if (j != 0)
                        {
                            int x = i + 1, y = j - 1;
                            while (x < 8 && y >= 0)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? 1 : (legal_in_board[x][y] == -1 ? 2 : 1));
                                if (in_board[x][y] != 0)
                                    break;
                                x++;
                                y--;
                            }
                        }
                    }
                    if (i != 0)
                    {
                        if (j != 7)
                        {
                            int x = i - 1, y = j + 1;
                            while (x >= 0 && y < 8)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? 1 : (legal_in_board[x][y] == -1 ? 2 : 1));
                                if (in_board[x][y] != 0)
                                    break;
                                x--;
                                y++;
                            }
                        }
                        if (j != 0)
                        {
                            int x = i - 1, y = j - 1;
                            while (x >= 0 && y >= 0)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? 1 : (legal_in_board[x][y] == -1 ? 2 : 1));
                                if (in_board[x][y] != 0)
                                    break;
                                x--;
                                y--;
                            }
                        }
                    }
                    break;

                }
                case 5: ///king
                {
                    for (int y = j - 1; y <= j + 1; y++)
                    {
                        if(i-1>=0 && y>=0 && y<8 && legal_in_board[i-1][y]!=2)
                        legal_in_board[i - 1][y] = (legal_in_board[i - 1][y] == 0 ? 1 : (legal_in_board[i - 1][y] == -1 ? 2 : 1));
                        if (i + 1 < 8 && y >= 0 && y < 8 && legal_in_board[i + 1][y] != 2)
                        legal_in_board[i + 1][y] = (legal_in_board[i + 1][y] == 0 ? 1 : (legal_in_board[i + 1][y] == -1 ? 2 : 1));
                    }
                    if(j-1>=0 && legal_in_board[i][j-1] != 2)
                    legal_in_board[i][j - 1] = (legal_in_board[i][j - 1] == 0 ? 1 : (legal_in_board[i][j - 1] == -1 ? 2 : 1));
                    if(j+1<8 && legal_in_board[i][j + 1] != 2)
                    legal_in_board[i][j + 1] = (legal_in_board[i][j + 1] == 0 ? 1 : (legal_in_board[i][j + 1] == -1 ? 2 : 1));
                    break;
                }
                case 6: ///pawn
                {
                    if (i - 1 >= 0 && j + 1 < 8)
                    {
                        if(legal_in_board[i-1][j+1]!=2)
                        legal_in_board[i - 1][j + 1] = (legal_in_board[i - 1][j + 1] == 0 ? 1 : (legal_in_board[i - 1][j + 1] == -1 ? 2 : 1));
                      
                    }
                    if(i-1>=0 && j-1>=0 && legal_in_board[i-1][j-1]!=2)
                    legal_in_board[i - 1][j - 1] = (legal_in_board[i - 1][j - 1] == 0 ? 1 : (legal_in_board[i - 1][j - 1] == -1 ? 2 : 1));
                    break;
                }
                }
            }
            else if(in_board[i][j]<0) ///black pieces
            {
                switch (in_board[i][j])
                {
                case -1: ///rook
                {
                    if(j!=7)
                    for (int y = j + 1; y < 8; y++)
                    {
                        if(legal_in_board[i][y] != 2)
                        legal_in_board[i][y] = (legal_in_board[i][y] == 0 ? -1 : (legal_in_board[i][y] == 1 ? 2 : -1));
                        if (in_board[i][y] != 0)
                            break;
                    }
                    if(j!=0)
                    for (int y = j - 1; y >= 0; y--)
                    {
                        if (legal_in_board[i][y] != 2)
                        legal_in_board[i][y] = (legal_in_board[i][y] == 0 ? -1 : (legal_in_board[i][y] == 1 ? 2 : -1));
                        if (in_board[i][y] != 0)
                            break;
                    }
                    if(i!=7)
                    for (int y = i + 1; y < 8; y++)
                    {   
                        if (legal_in_board[y][j] != 2)
                        legal_in_board[y][j] = (legal_in_board[y][j] == 0 ? -1 : (legal_in_board[y][j] == 1 ? 2 : -1));
                        if (in_board[y][j] != 0)
                            break;
                    }
                    if(i!=0)
                    for (int y = i - 1; y >= 0; y--)
                    {
                        if (legal_in_board[y][j] != 2)
                        legal_in_board[y][j] = (legal_in_board[y][j] == 0 ? -1 : (legal_in_board[y][j] == 1 ? 2 : -1));
                        if (in_board[y][j] != 0)
                            break;
                    }
                    break;
                }
                case -2: ///knight
                {if (i + 2 < 8 && j + 1 < 8 && legal_in_board[i + 2][j + 1] !=2)
                    legal_in_board[i + 2][j + 1] = (legal_in_board[i + 2][j + 1] == 0 ? -1 : (legal_in_board[i + 2][j + 1] == 1 ? 2 : -1));
                if (i + 2 < 8 && j - 1 >= 0 && legal_in_board[i + 2][j - 1] !=2)
                    legal_in_board[i + 2][j - 1] = (legal_in_board[i + 2][j - 1] == 0 ? -1 : (legal_in_board[i + 2][j - 1] == 1 ? 2 : -1));
                if (i - 2 >= 0 && j + 1 < 8 && legal_in_board[i - 2][j + 1] !=2)
                    legal_in_board[i - 2][j + 1] = (legal_in_board[i - 2][j + 1] == 0 ? -1 : (legal_in_board[i - 2][j + 1] == 1 ? 2 : -1));
                if (i - 2 >= 0 && j - 1 >= 0 && legal_in_board[i - 2][j - 1]!=2)
                    legal_in_board[i - 2][j - 1] = (legal_in_board[i - 2][j - 1] == 0 ? -1 : (legal_in_board[i - 2][j - 1] == 1 ? 2 : -1));
                if (i + 1 < 8 && j + 2 < 8 && legal_in_board[i + 1][j + 2]!=2)
                    legal_in_board[i + 1][j + 2] = (legal_in_board[i + 1][j + 2] == 0 ? -1 : (legal_in_board[i + 1][j + 2] == 1 ? 2 : -1));
                if (i + 1 < 8 && j - 2 >= 0 && legal_in_board[i + 1][j - 2] !=2)
                    legal_in_board[i + 1][j - 2] = (legal_in_board[i + 1][j - 2] == 0 ? -1 : (legal_in_board[i + 1][j - 2] == 1 ? 2 : -1));
                if (i - 1 >= 0 && j + 2 < 8 && legal_in_board[i - 1][j + 2]!=2)
                    legal_in_board[i - 1][j + 2] = (legal_in_board[i - 1][j + 2] == 0 ? -1 : (legal_in_board[i - 1][j + 2] == 1 ? 2 : -1));
                if (i - 1 >= 0 && j - 2 >= 0 && legal_in_board[i - 1][j - 2]!=2)
                    legal_in_board[i - 1][j - 2] = (legal_in_board[i - 1][j - 2] == 0 ? -1 : (legal_in_board[i - 1][j - 2] == 1 ? 2 : -1));

                    break;
                }
                case -3: ///bishop
                {
                    if (i != 7) {
                        if (j != 7)
                        {
                            int x = i + 1, y = j + 1;
                            while (x < 8 && y < 8)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? -1 : (legal_in_board[x][y] == 1 ? 2 : -1));
                                if (in_board[x][y] != 0)
                                    break;
                                x++;
                                y++;
                            }
                        }
                        if(j!=0)
                        {
                            int x = i + 1, y = j - 1;
                            while (x < 8 && y >=0 )
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? -1 : (legal_in_board[x][y] == 1 ? 2 : -1));
                                if (in_board[x][y] != 0)
                                    break;
                                x++;
                                y--;
                            }
                        }
                    }
                    if (i != 0) {
                        if(j!=7)
                        {
                            int x = i - 1, y = j + 1;
                            while (x >= 0 && y < 8)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? -1 : (legal_in_board[x][y] == 1 ? 2 : -1));
                                if (in_board[x][y] != 0)
                                    break;
                                x--;
                                y++;
                            }
                        }
                        if(j!=0)
                        {
                            int x = i - 1, y = j - 1;
                            while (x >= 0 && y >= 0)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? -1 : (legal_in_board[x][y] == 1 ? 2 : -1));
                                if (in_board[x][y] != 0)
                                    break;
                                x--;
                                y--;
                            }
                        }
                    }


                    break;
                }
                case -4: ///queen
                {
                    if (j != 7)
                        for (int y = j + 1; y < 8; y++)
                        {
                            if (legal_in_board[i][y] != 2)
                                legal_in_board[i][y] = (legal_in_board[i][y] == 0 ? -1 : (legal_in_board[i][y] == 1 ? 2 : -1));
                            if (in_board[i][y] != 0)
                                break;
                        }
                    if (j != 0)
                        for (int y = j - 1; y >= 0; y--)
                        {
                            if (legal_in_board[i][y] != 2)
                                legal_in_board[i][y] = (legal_in_board[i][y] == 0 ? -1 : (legal_in_board[i][y] == 1 ? 2 : -1));
                            if (in_board[i][y] != 0)
                                break;
                        }
                    if (i != 7)
                        for (int y = i + 1; y < 8; y++)
                        {
                            if (legal_in_board[y][j] != 2)
                                legal_in_board[y][j] = (legal_in_board[y][j] == 0 ? -1 : (legal_in_board[y][j] == 1 ? 2 : -1));
                            if (in_board[y][j] != 0)
                                break;
                        }
                    if (i != 0)
                        for (int y = i - 1; y >= 0; y--)
                        {
                            if (legal_in_board[y][j] != 2)
                                legal_in_board[y][j] = (legal_in_board[y][j] == 0 ? -1 : (legal_in_board[y][j] == 1 ? 2 : -1));
                            if (in_board[y][j] != 0)
                                break;
                        }
                    if (i != 7) {
                        if (j != 7)
                        {
                            int x = i + 1, y = j + 1;
                            while (x < 8 && y < 8)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? -1 : (legal_in_board[x][y] == 1 ? 2 : -1));
                                if (in_board[x][y] != 0)
                                    break;
                                x++;
                                y++;
                            }
                        }
                        if (j != 0)
                        {
                            int x = i + 1, y = j - 1;
                            while (x < 8 && y >= 0)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? -1 : (legal_in_board[x][y] == 1 ? 2 : -1));
                                if (in_board[x][y] != 0)
                                    break;
                                x++;
                                y--;
                            }
                        }
                    }
                    if (i != 0) {
                        if (j != 7)
                        {
                            int x = i - 1, y = j + 1;
                            while (x >= 0 && y < 8)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? -1 : (legal_in_board[x][y] == 1 ? 2 : -1));
                                if (in_board[x][y] != 0)
                                    break;
                                x--;
                                y++;
                            }
                        }
                        if (j != 0)
                        {
                            int x = i - 1, y = j - 1;
                            while (x >= 0 && y >= 0)
                            {
                                if (legal_in_board[x][y] != 2)
                                    legal_in_board[x][y] = (legal_in_board[x][y] == 0 ? -1 : (legal_in_board[x][y] == 1 ? 2 : -1));
                                if (in_board[x][y] != 0)
                                    break;
                                x--;
                                y--;
                            }
                        }
                    }

                    break;

                }
                case -5: ///king
                {
                    for (int y = j - 1; y <= j + 1; y++)
                    {
                        if (i - 1 >= 0 && y >= 0 && y < 8 && legal_in_board[i - 1][y]!=2)
                            legal_in_board[i - 1][y] = (legal_in_board[i - 1][y] == 0 ? -1 : (legal_in_board[i - 1][y] == 1 ? 2 : -1));
                        if (i + 1 < 8 && y >= 0 && y < 8 && legal_in_board[i + 1][y]!=2)
                            legal_in_board[i + 1][y] = (legal_in_board[i + 1][y] == 0 ? -1 : (legal_in_board[i + 1][y] == 1 ? 2 : -1));
                    }
                    if (j - 1 >= 0 && legal_in_board[i][j - 1] !=2)
                        legal_in_board[i][j - 1] = (legal_in_board[i][j - 1] == 0 ? -1 : (legal_in_board[i][j - 1] == 1 ? 2 : -1));
                    if (j + 1 < 8 && legal_in_board[i][j + 1] != 2)
                        legal_in_board[i][j + 1] = (legal_in_board[i][j + 1] == 0 ? -1 : (legal_in_board[i][j + 1] == 1 ? 2 : -1));
                    break;
                }
                case -6: ///pawn
                {
                    if(i+1<8&&j+1<8 && legal_in_board[i + 1][j + 1]!=2)
                    legal_in_board[i + 1][j + 1] = (legal_in_board[i + 1][j + 1] == 0 ? -1 : (legal_in_board[i + 1][j + 1] == 1 ? 2 : -1));
                    if(i+1<8 && j-1>=0 && legal_in_board[i + 1][j - 1])
                    legal_in_board[i + 1][j - 1] = (legal_in_board[i + 1][j - 1] == 0 ? -1 : (legal_in_board[i + 1][j - 1] == 1 ? 2 : -1));
                    break;
                }
                }
            }
        }
     
    }
}

bool wk = 1, bk = 1, wrr = 1, wrl = 1, brr = 1, brl = 1;
bool isMoveLegal(int piece, int x_start, int y_start, int x, int y, int &castles)
{
    if (x_start == x && y_start == y)
        return 0;
    if (board[x][y] * piece > 0)
        return 0;
    if (piece * turn < 0)
        return 0;
    if (x < 0 || x > 7 || y < 0 || y > 7)
        return 0;
    int l_board[8][8] = { 0 };
    int temp_board[8][8] = { 0 };
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            temp_board[i][j] = board[i][j];
    temp_board[x_start][y_start] = 0;
    temp_board[x][y] = board[x_start][y_start];
    UpdateCheckBoard(l_board, temp_board);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
           // std::cout << temp_board[i][j] << " ";
            if (abs(temp_board[i][j]) == 5 && temp_board[i][j] * turn > 0)
            {
                if (l_board[i][j] == 0)
                {
                    i = 8;  break;
                }
                if (l_board[i][j] == 2 || l_board[i][j] * turn < 0)
                {
                    is_check = turn * -1;
                    return 0;
                }
            }
        }
        //std::cout << '\n';
    }
    //std::cout << '\n';
    if (abs(piece) == 5 && l_board[x_start][y_start]!=2 && l_board[x_start][y_start] *turn >=0 && piece * turn > 0) ///castling
    {
        if(piece == 5 && wk == 1) ///white king
        {
            if (x == 7 && y == 6 && wrr == 1 && l_board[7][5] != 2 && l_board[7][6] != 2 && l_board[7][5] * turn >= 0 && l_board[7][6] * turn >= 0)
            {
                castles = 1;
                
                return 1;
            }
            if (x == 7 && y == 2 && wrl == 1 && l_board[7][3] != 2 && l_board[7][2] != 2 && l_board[7][3] * turn >= 0 && l_board[7][2] * turn >= 0)
            {
                castles = 2;
                
                return 1;
            }
        }
        else if(piece == -5 && bk == 1) ///black king
        {
            if (x == 0 && y == 6 && brr == 1 && l_board[0][5] != 2 && l_board[0][6] != 2 && l_board[0][5] * turn >= 0 && l_board[0][6] * turn >= 0)
            {
                castles = 3;
                return 1;
            }
            if (x == 0 && y == 2 && brl == 1 && l_board[0][3] != 2 && l_board[0][2] != 2 && l_board[0][3] * turn >= 0 && l_board[0][2] * turn >= 0)
            {
                castles = 4;
                return 1;
            }
        }
    }
    if (piece > 0) ///white piece
    {
        switch (piece)
        {
        case 6: ///pawn
        {
            if (x == x_start - 1) ///capture diagonally
            {
                if ((y == y_start - 1 || y == y_start + 1) && board[x][y] != 0)
                    return 1;
            }
            if (y != y_start)
                return 0;
            if (x_start == 6)
            {
                if (x == x_start - 2 && board[x][y] == 0)
                    return 1;
            }
            if (x == x_start - 1 && board[x][y] == 0)
                return 1;
            return 0;
            break;
        }
        case 5: ///king
        {
            if (abs(x - x_start) > 1 || abs(y - y_start) > 1)
                return 0;
            wk = 0;
            return 1;
            break;
        }
        case 2: ///knight
        {
            if (x == x_start - 2 && y == y_start + 1)
                return 1;
            if (x == x_start - 2 && y == y_start - 1)
                return 1;
            if (x == x_start + 2 && y == y_start + 1)
                return 1;
            if (x == x_start + 2 && y == y_start - 1)
                return 1;
            if (y == y_start + 2 && x == x_start - 1)
                return 1;
            if (y == y_start + 2 && x == x_start + 1)
                return 1;
            if (y == y_start - 2 && x == x_start - 1)
                return 1;
            if (y == y_start - 2 && x == x_start + 1)
                return 1;
            return 0;
            break;
        }
        case 1: ///rook
        {
            if (x_start == x)
            {
                if (y_start < y)
                {
                    for (int i = y_start + 1; i < y; i++)
                        if (board[x][i] != 0)
                            return 0;
                }
                else
                {
                    for (int i = y + 1; i < y_start; i++)
                        if (board[x][i] != 0)
                            return 0;
                }
                if (wrr == 1 && x_start == 7 && y_start == 7)
                    wrr = 0;
                else if (wrl == 1 && x_start == 7 && y_start == 0)
                    wrl = 0;
                return 1;
            }
            else if (y_start == y)
            {
                if (x_start < x)
                {
                    for (int i = x_start + 1; i < x; i++)
                        if (board[i][y] != 0)
                            return 0;
                }
                else
                {
                    for (int i = x + 1; i < x_start; i++)
                        if (board[i][y] != 0)
                            return 0;
                }
                return 1;
            }
            else return 0;
            break;
        }
        case 3: ///bishop 
        {
            if (x_start == x || y_start == y)
                return 0;
            if (abs(x - x_start) != abs(y - y_start))
                return 0;
            if (x < x_start)
            {
                if (y < y_start)
                {
                    for (int i = x_start - 1, j = y_start - 1; i > x, j > y; i--, j--)
                        if (board[i][j] != 0)
                            return 0;
                }
                else
                {
                    for (int i = x_start - 1, j = y_start + 1; i > x, j < y; i--, j++)
                        if (board[i][j] != 0)
                            return 0;
                }
            }
            else
            {
                if (y < y_start)
                {
                    for (int i = x_start + 1, j = y_start - 1; i < x, j > y; i++, j--)
                        if (board[i][j] != 0)
                            return 0;
                }
                else
                {
                    for (int i = x_start + 1, j = y_start + 1; i < x, j < y; i++, j++)
                        if (board[i][j] != 0)
                            return 0;
                }
            }
            return 1;
            break;
        }
        case 4: ///queen
        {
            if (x_start == x)
            {
                if (y_start < y)
                {
                    for (int i = y_start + 1; i < y; i++)
                        if (board[x][i] != 0)
                            return 0;
                }
                else
                {
                    for (int i = y + 1; i < y_start; i++)
                        if (board[x][i] != 0)
                            return 0;
                }
                return 1;
            }
            else if (y_start == y)
            {
                if (x_start < x)
                {
                    for (int i = x_start + 1; i < x; i++)
                        if (board[i][y] != 0)
                            return 0;
                }
                else
                {
                    for (int i = x + 1; i < x_start; i++)
                        if (board[i][y] != 0)
                            return 0;
                }
                return 1;
            }
            else
            {
                if (abs(x - x_start) != abs(y - y_start))
                    return 0;
                if (x < x_start)
                {
                    if (y < y_start)
                    {
                        for (int i = x_start - 1, j = y_start - 1; i > x, j > y; i--, j--)
                            if (board[i][j] != 0)
                                return 0;
                    }
                    else
                    {
                        for (int i = x_start - 1, j = y_start + 1; i > x, j < y; i--, j++)
                            if (board[i][j] != 0)
                                return 0;
                    }
                }
                else
                {
                    if (y < y_start)
                    {
                        for (int i = x_start + 1, j = y_start - 1; i < x, j > y; i++, j--)
                            if (board[i][j] != 0)
                                return 0;
                    }
                    else
                    {
                        for (int i = x_start + 1, j = y_start + 1; i < x, j < y; i++, j++)
                            if (board[i][j] != 0)
                                return 0;
                    }
                }
                return 1;
            }
            break;
        }
        }
    }
    else if (piece < 0) ///black piece
    {
        switch (piece)
        {
        case -6: ///pawn
        {
            if (x == x_start + 1) ///capture diagonally
            {
                if ((y == y_start - 1 || y == y_start + 1) && board[x][y] != 0)
                    return 1;
            }
            if (y != y_start)
                return 0;
            if (x_start == 1)
            {
                if (x == x_start + 2 && board[x][y] == 0)
                    return 1;
            }
            if (x == x_start + 1 && board[x][y] == 0)
                return 1;
            return 0;
            break;
        }
        case -5: ///king
        {
            if (abs(x - x_start) > 1 || abs(y - y_start) > 1)
                return 0;
            bk = 0;
            return 1;
            break;
        }
        case -2: ///knight
        {
            if (x == x_start - 2 && y == y_start + 1)
                return 1;
            if (x == x_start - 2 && y == y_start - 1)
                return 1;
            if (x == x_start + 2 && y == y_start + 1)
                return 1;
            if (x == x_start + 2 && y == y_start - 1)
                return 1;
            if (y == y_start + 2 && x == x_start - 1)
                return 1;
            if (y == y_start + 2 && x == x_start + 1)
                return 1;
            if (y == y_start - 2 && x == x_start - 1)
                return 1;
            if (y == y_start - 2 && x == x_start + 1)
                return 1;
            return 0;
            break;
        }
        case -1: ///rook
        {
            if (x_start == x)
            {
                if (y_start < y)
                {
                    for (int i = y_start + 1; i < y; i++)
                        if (board[x][i] != 0)
                            return 0;
                }
                else
                {
                    for (int i = y + 1; i < y_start; i++)
                        if (board[x][i] != 0)
                            return 0;
                }
                if (brr == 1 && x_start == 0 && y_start == 7)
                    brr = 0;
                else if (brl == 1 && x_start == 0 && y_start == 0)
                    brl = 0;
                return 1;
            }
            else if (y_start == y)
            {
                if (x_start < x)
                {
                    for (int i = x_start + 1; i < x; i++)
                        if (board[i][y] != 0)
                            return 0;
                }
                else
                {
                    for (int i = x + 1; i < x_start; i++)
                        if (board[i][y] != 0)
                            return 0;
                }
                return 1;
            }
            else return 0;
            break;
        }
        case -3: ///bishop 
        {
            if (x_start == x || y_start == y)
                return 0;
            if (abs(x - x_start) != abs(y - y_start))
                return 0;
            if (x < x_start)
            {
                if (y < y_start)
                {
                    for (int i = x_start - 1, j = y_start - 1; i > x, j > y; i--, j--)
                        if (board[i][j] != 0)
                            return 0;
                }
                else
                {
                    for (int i = x_start - 1, j = y_start + 1; i > x, j < y; i--, j++)
                        if (board[i][j] != 0)
                            return 0;
                }
            }
            else
            {
                if (y < y_start)
                {
                    for (int i = x_start + 1, j = y_start - 1; i < x, j > y; i++, j--)
                        if (board[i][j] != 0)
                            return 0;
                }
                else
                {
                    for (int i = x_start + 1, j = y_start + 1; i < x, j < y; i++, j++)
                        if (board[i][j] != 0)
                            return 0;
                }
            }
            return 1;
            break;
        }
        case -4: ///queen
        {
            if (x_start == x)
            {
                if (y_start < y)
                {
                    for (int i = y_start + 1; i < y; i++)
                        if (board[x][i] != 0)
                            return 0;
                }
                else
                {
                    for (int i = y + 1; i < y_start; i++)
                        if (board[x][i] != 0)
                            return 0;
                }
                return 1;
            }
            else if (y_start == y)
            {
                if (x_start < x)
                {
                    for (int i = x_start + 1; i < x; i++)
                        if (board[i][y] != 0)
                            return 0;
                }
                else
                {
                    for (int i = x + 1; i < x_start; i++)
                        if (board[i][y] != 0)
                            return 0;
                }
                return 1;
            }
            else
            {
                if (abs(x - x_start) != abs(y - y_start))
                    return 0;
                if (x < x_start)
                {
                    if (y < y_start)
                    {
                        for (int i = x_start - 1, j = y_start - 1; i > x, j > y; i--, j--)
                            if (board[i][j] != 0)
                                return 0;
                    }
                    else
                    {
                        for (int i = x_start - 1, j = y_start + 1; i > x, j < y; i--, j++)
                            if (board[i][j] != 0)
                                return 0;
                    }
                }
                else
                {
                    if (y < y_start)
                    {
                        for (int i = x_start + 1, j = y_start - 1; i < x, j > y; i++, j--)
                            if (board[i][j] != 0)
                                return 0;
                    }
                    else
                    {
                        for (int i = x_start + 1, j = y_start + 1; i < x, j < y; i++, j++)
                            if (board[i][j] != 0)
                                return 0;
                    }
                }
                return 1;
            }
            break;
        }
        }
    }
}

int swapPieces()
{
    RenderWindow selection(VideoMode(490, 150), "Pieces");
    Texture pieces;
    Sprite s[4];
    pieces.loadFromFile("pieces.png");
    for (int i = 0; i < 4; i++)
    {
        int y = turn > 0 ? 1 : 0;
        s[i].setTexture(pieces);
        s[i].setTextureRect(IntRect(size * i, size * y , size, size));
        s[i].setPosition(size*2.1*i, 20);
        s[i].scale(Vector2f(2, 2));
    }
    int piece_chosen = 0;
    selection.clear();
    for (int i = 0; i < 4; i++)
        selection.draw(s[i]);
    selection.display();
    while (selection.isOpen())
    {
        Vector2i pos = Mouse::getPosition(selection);
        Event e;
        while (selection.pollEvent(e))
        {
            ////to do close
            if (e.type == Event::MouseButtonPressed)
            {
                if (e.key.code == Mouse::Left)
                {
                    for(int i=0; i<4; i++)
                        if (s[i].getGlobalBounds().contains(pos.x, pos.y))
                        {
                            piece_chosen = i*turn + turn;
                        }
                }
            }
            if (e.type == Event::MouseButtonReleased && piece_chosen!=0)
                if (e.key.code == Mouse::Left)
                {
                    selection.close();
                    return piece_chosen;
                }

            selection.clear();
            for (int i = 0; i < 4; i++)
                selection.draw(s[i]);
            selection.display();
        }
    }
}

bool isKingInCheck()
{
    int l_board[8][8] = { 0 };
    UpdateCheckBoard(l_board, board);
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (abs(board[i][j]) == 5 && (l_board[i][j] == 2 || l_board[i][j] * turn > 1))
                return 1;
        }
    }
    return 0;
}

bool areThereLegalMoves()
{
   
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            if (board[i][j] * turn > 0)
            {
                switch (abs(board[i][j]))
                {
                case 1:
                {
                    for (int k = i - 1; k >= 0; k--)
                        if (isMoveLegal(board[i][j], i, j, k, j, castles))
                            return 1;
                    for (int k = i + 1; k < 8; k++)
                        if (isMoveLegal(board[i][j], i, j, k, j, castles))
                            return 1;
                    for (int k = j - 1; k >= 0; k--)
                        if (isMoveLegal(board[i][j], i, j, i, k, castles))
                            return 1;
                    for (int k = j + 1; k < 8; k++)
                        if (isMoveLegal(board[i][j], i, j, i, k, castles))
                            return 1;
                    break;
                }
                case 2:
                {
                    if (isMoveLegal(board[i][j], i, j, i - 2, j - 1, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i - 2, j + 1, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i + 2, j - 1, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i + 2, j + 1, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i - 1, j - 2, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i - 1, j + 2, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i + 1, j - 2, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i + 1, j + 2, castles))
                        return 1;
                    break;
                }
                case 3:
                {
                    for (int i1 = i - 1, j1 = j - 1; i1 >= 0 && j1 >= 0; i1--, j1--)
                        if (isMoveLegal(board[i][j], i, j, i1, j1, castles))
                            return 1;
                    for (int i1 = i - 1, j1 = j + 1; i1 >= 0 && j1 < 8; i1--, j1++)
                        if (isMoveLegal(board[i][j], i, j, i1, j1, castles))
                            return 1;
                    for (int i1 = i + 1, j1 = j - 1; i1 < 8 && j1 >= 0; i1++, j1--)
                        if (isMoveLegal(board[i][j], i, j, i1, j1, castles))
                            return 1;
                    for (int i1 = i + 1, j1 = j + 1; i1 < 8 && j1 < 8; i1++, j1++)
                        if (isMoveLegal(board[i][j], i, j, i1, j1, castles))
                            return 1;
                    break;
                }
                case 4:
                {

                    for (int k = i - 1; k >= 0; k--)
                        if (isMoveLegal(board[i][j], i, j, k, j, castles))
                            return 1;
                    for (int k = i + 1; k < 8; k++)
                        if (isMoveLegal(board[i][j], i, j, k, j, castles))
                            return 1;
                    for (int k = j - 1; k >= 0; k--)
                        if (isMoveLegal(board[i][j], i, j, i, k, castles))
                            return 1;
                    for (int k = j + 1; k < 8; k++)
                        if (isMoveLegal(board[i][j], i, j, i, k, castles))
                            return 1;



                    for (int i1 = i - 1, j1 = j - 1; i1 >= 0 && j1 >= 0; i1--, j1--)
                        if (isMoveLegal(board[i][j], i, j, i1, j1, castles))
                            return 1;
                    for (int i1 = i - 1, j1 = j + 1; i1 >= 0 && j1 < 8; i1--, j1++)
                        if (isMoveLegal(board[i][j], i, j, i1, j1, castles))
                            return 1;
                    for (int i1 = i + 1, j1 = j - 1; i1 < 8 && j1 >= 0; i1++, j1--)
                        if (isMoveLegal(board[i][j], i, j, i1, j1, castles))
                            return 1;
                    for (int i1 = i + 1, j1 = j + 1; i1 < 8 && j1 < 8; i1++, j1++)
                        if (isMoveLegal(board[i][j], i, j, i1, j1, castles))
                            return 1;
                    break;

                }
                case 5:
                {
                    if (isMoveLegal(board[i][j], i, j, i + 1, j, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i + 1, j - 1, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i + 1, j + 1, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i - 1, j, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i - 1, j + 1, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i - 1, j - 1, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i , j - 1, castles))
                        return 1;
                    if (isMoveLegal(board[i][j], i, j, i , j + 1, castles))
                        return 1;
                    break;
                }
                case 6:
                {
                    if (board[i][j] == 6)
                    {
                        if (isMoveLegal(board[i][j], i, j, i - 1, j, castles))
                            return 1;
                        if (isMoveLegal(board[i][j], i, j, i - 2, j, castles))
                            return 1;
                        if (isMoveLegal(board[i][j], i, j, i - 1, j + 1, castles))
                            return 1;
                        if (isMoveLegal(board[i][j], i, j, i - 1, j - 1, castles))
                            return 1;
                    }
                    else
                    {
                        if (isMoveLegal(board[i][j], i, j, i + 1, j, castles))
                            return 1;
                        if (isMoveLegal(board[i][j], i, j, i + 2, j, castles))
                            return 1;
                        if (isMoveLegal(board[i][j], i, j, i + 1, j + 1, castles))
                            return 1;
                        if (isMoveLegal(board[i][j], i, j, i + 1, j - 1, castles))
                            return 1;
                    }
                    break;
                }
                }
            }
        }
    }
    return 0;
}

void update_Move_List(const char str[])
{
    
    if (turn == 1)
        strcpy_s(moves[move].moveWhite, str);
    else
    {
        strcpy_s(moves[move++].moveBlack, str);
    }
}

bool hasGameEnded()
{
    std::cout << "Aici: " <<  areThereLegalMoves() << '\n';
    if (!areThereLegalMoves())
    {
       
        if (isKingInCheck())
        {
            if (turn == 1)
                std::cout << "White wins!";
            else
                std::cout << "Black wins!";
            
        }
        else
            std::cout << "Draw!";
        return 1;
    }
    return 0;
}

int main()
{
    RenderWindow window(VideoMode(900, 900), "Chess");

    Texture t1, t2;
    t1.loadFromFile("board0.png");
    t2.loadFromFile("pieces.png");
    Sprite s(t2);
    Sprite sBoard(t1);
    sBoard.scale(Vector2f(2, 2));
    s.scale(Vector2f(2, 2));
    
    for (int i = 0; i < 32; i++)
        v[i].setTexture(t2);
     
    loadPosition(1);
    
    bool isMove = false;
    float dx = 0, dy = 0;
    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);
        Event e;
        
        while (window.pollEvent(e))
        {
            
            if (e.type == Event::Closed)
                window.close();
            if (e.type == Event::MouseButtonPressed)
                    if (e.key.code == Mouse::Left)
                        for(int i=0; i<32; i++)
                            if (v[i].getGlobalBounds().contains(pos.x, pos.y))
                            {
                                isMove = true;
                                dx = pos.x - v[i].getPosition().x;
                                dy = pos.y - v[i].getPosition().y;
                                old_pos.x = v[i].getPosition().x / (2 * size);
                                old_pos.y = v[i].getPosition().y / (2 * size);
                                piece_moved = board[old_pos.y][old_pos.x];
                                sprite_piece_moved = i;
                                break;
                            }
            if (e.type == Event::MouseButtonReleased)
                if (e.key.code == Mouse::Left)
                {
                    isMove = false;
                    int position_x = (pos.x - dx + size) / (2 * size);
                    int position_y = (pos.y - dy + size) / (2 * size);
                    //std::cout << isMoveLegal(piece_moved, old_pos.y, old_pos.x, position_y, position_x) << '\n';;
                    
                    if (isMoveLegal(piece_moved, old_pos.y, old_pos.x, position_y, position_x,castles)) {
                        bool takes = 0;
                        if (board[position_y][position_x] != 0)
                            takes = 1;
                        board[old_pos.y][old_pos.x] = 0;
                        board[position_y][position_x] = piece_moved;
                        //v[sprite_piece_moved].setPosition(position_x * 2 * size, position_y * 2 * size);
                        char s1[10];
                        int length1 = 0;
                        switch (castles)
                        {
                        case 1: ///white O-O
                        {
                            wk = 0;
                            wrr = 0;
                            board[7][7] = 0;
                            board[7][5] = 1;
                            strcpy_s(s1, "O-O");
                            length1 = 3;

                            break;
                        }

                        case 2: ///white O-O-O
                        {
                            wk = 0;
                            wrl = 0;
                            board[7][0] = 0;
                            board[7][3] = 1;
                            strcpy_s(s1, "O-O-O");
                            length1 = 5;
                            break;
                        }
                        case 3: ///black O-O
                        {
                            bk = 0;
                            brr = 0;
                            board[0][7] = 0;
                            board[0][5] = -1;
                            strcpy_s(s1, "O-O");
                            length1 = 3;
                            break;
                        }
                        case 4: ///black O-O-O
                        {
                            bk = 0;
                            brl = 0;
                            board[0][0] = 0;
                            board[0][3] = -1;
                            strcpy_s(s1, "O-O-O");
                            length1 = 5;
                            break; 
                        }
                     
                        };
                        
                        if (!castles) {
                            char s[10];
                            int length = 0;
                            switch (abs(piece_moved))
                            {
                            case 1:
                            {
                                s[length++] = 'R';
                                break;
                            }
                            case 2:
                            {
                                s[length++] = 'N';
                                break;
                            }
                            case 3:
                            {
                                s[length++] = 'B';
                                break;
                            }
                            case 4:
                            {
                                s[length++] = 'Q';
                                break;
                            }
                            case 5:
                            {
                                s[length++] = 'K';
                                break;
                            }
                            case 6:
                            {
                                if (takes)
                                    s[length++] = (char)(old_pos.x + 'a');
                            }
                            }
                           
                            if (takes)
                                s[length++] = 'x';
                            s[length++] = (char)(position_x + 'a');
                            s[length++] = (char)(8 - position_y  + '0');
                            
                            s[length] = '\0';
                            strcpy_s(s1, s);
                            length1 = length;
                        }

                        

                        if ((piece_moved == 6 && position_y == 0) || (piece_moved == -6 && position_y == 7))
                        {
                            s1[length1++] = '=';
                            piece_moved = swapPieces();
                            board[position_y][position_x] = piece_moved;
                            switch (abs(piece_moved))
                            {
                            case 1:
                            {
                                s1[length1++] = 'R';
                                break;
                            }
                            case 2:
                            {
                                s1[length1++] = 'N';
                                break;
                            }
                            case 3:
                            {
                                s1[length1++] = 'B';
                                break;
                            }
                            case 4:
                            {
                                s1[length1++] = 'Q';
                                break;
                            }
                            }
                        }
                        std::cout << isKingInCheck()<<'\n';
                        if (isKingInCheck())
                            s1[length1++] = '+';
                        s1[length1] = '\0';
                        update_Move_List(s1);
                        loadPosition(0);
                        
                        
                        if (turn == 1)
                        {
                            std::cout << move << ": " << moves[move].moveWhite;
                            for (int i = 11 - length1; i > 0; i--)
                                std::cout << " ";
                        }
                        else
                            std::cout << moves[move-1].moveBlack<< '\n';
                        sprite_piece_moved = -1;
                        if (hasGameEnded())
                            break;
                        turn *= -1;
                        
                        
                    }
                    else
                    {
                        window.clear();
                        loadPosition(0);
                    }
                    
                }
        }
        if (isMove)
        {
            v[sprite_piece_moved].setPosition(pos.x - dx, pos.y - dy);
        }
        window.clear();
        window.draw(sBoard);
        for (int k = 0; k < nr_pieces; k++)
            window.draw(v[k]);
        window.display();
    }
    return 0;
}