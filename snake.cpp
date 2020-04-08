#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctime>
#include <stdlib.h>

using namespace std;

//global variable declarations
int height = 20, width = 30;
int y = height / 2, x = width / 2, foodx, foody, score;
bool gameover = false;
int tailx[100], taily[100], ntail = 0;
enum dir
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};
dir sdir;

//function declarations
void board();
void input();
void move();
void logic();
void foodgen();
void tailgen();
int kbhit(void);
void SetCursorPos(int XPos, int YPos);

int main()
{

    system("stty -icanon");
    board();
    foodgen();
    while (!gameover)
    {
        input();
        move();
        logic();
        board();
    }
    cout << "\nGame Over\n";
    return 0;
}

void board()
{

    void SetCursorPos();

    system("clear");
    cout << "\nscore: " << score << endl;
    for (int i = 0; i <= height; i++)
    {
        for (int j = 0; j <= width; j++)
        {

            if (i == y && j == x)
            {
                cout << "@";
            }
            else if (i == foody && j == foodx)
            {
                cout << "^";
            }

            else if (i == 0 || i == height)
            {
                cout << "#";
            }
            else if (j == 0 || j == width)
            {
                cout << "#";
            }
            else
            {
                bool tailp = false;
                for (int k = 0; k < ntail; k++)
                {
                    if (tailx[k] == j && taily[k] == i)
                    {
                        tailp = true;
                        cout << "*";
                    }
                }
                if (tailp == 0)
                {
                    cout << " ";
                }
            }
        }
        cout << endl;
    }
}

void input()
{
    usleep(300000);
    if (kbhit())
    {
        switch (getchar())
        {

        case 'w':
            if (sdir != DOWN)
                sdir = UP;
            break;
        case 's':
            if (sdir != UP)
                sdir = DOWN;
            break;
        case 'a':
            if (sdir != RIGHT)
                sdir = LEFT;
            break;
        case 'd':
            if (sdir != LEFT)
                sdir = RIGHT;
            break;
        case 27:
            gameover = true;
            break;
        }
    }
}

void move()
{
    tailgen();
    switch (sdir)
    {
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    }
}

void logic()
{
    if (x == foodx && y == foody)
    {
        score++;
        ntail++;
        foodgen();
    }
    if (x <= 0 || x >= width || y <= 0 || y >= height)
    {
        gameover = true;
    }
    for (int i = 0; i < ntail; i++)
    {
        if (x == tailx[i] && y == taily[i])
        {
            gameover = true;
        }
    }
}

void foodgen()
{
    foodx = rand() % width - 1;
    foody = rand() % height - 1;
    if (foodx == 0 || foody == 0)
    {
        foodgen();
    }
}

void tailgen()
{
    int prevx = tailx[0];
    int prevy = taily[0];
    int prev2x, prev2y;
    tailx[0] = x;
    taily[0] = y;
    for (int i = 1; i < ntail; i++)
    {
        prev2x = tailx[i];
        prev2y = taily[i];
        tailx[i] = prevx;
        taily[i] = prevy;
        prevx = prev2x;
        prevy = prev2y;
    }
}

int kbhit(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void SetCursorPos(int XPos, int YPos)
{
    printf("\033[%d;%dH", YPos + 1, XPos + 1);
}