#include <iostream>
#include <termios.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
//#include <Ncurses>
using namespace std;
int kbhit(void);

int height = 20, width = 30;
int y = height / 2, x = width / 2, foodx, foody, score;
bool gameover = false;
enum dir
{
    STOP,
    UP,
    DOWN,
    LEFT,
    RIGHT
};
dir sdir;
void board();
void input();
void move();
void logic();
void foodgen();
int main()
{

    foodgen();
    system("stty -icanon");
    board();
    while (!gameover)
    {
        input();
        move();
        logic();
        board();
    }
    cout << "\nGame Over";
    return 0;
}

void board()
{
    system("clear");
    for (int i = 0; i <= height; i++)
    {
        for (int j = 0; j <= width; j++)
        {

            if (i == y && j == x)
            {
                cout << "*";
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
                cout << " ";
            }
        }
        cout << endl;
    }
    cout << "\nscore: " << score;
}

void input()
{
    sleep(1);
    if (kbhit())
    {
        switch (getchar())
        {

        case 'w':
            sdir = UP;
            break;
        case 's':
            sdir = DOWN;
            break;
        case 'a':
            sdir = LEFT;
            break;
        case 'd':
            sdir = RIGHT;
            break;
        case 'x':
            gameover = true;
            break;
        }
    }
}

void move()
{
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
        foodgen();
    }
    if (x <= 0 || x >= width || y <= 0 || y >= height)
    {
        gameover = true;
    }
}

void foodgen()
{
    foodx = rand() % width - 1;
    foody = rand() % height - 1;
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