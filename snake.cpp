#include <iostream>
#include <termios.h>
using namespace std;

int height = 20, width = 30;
int x, y, foodx, foody, score;
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
int main()
{
    //struct termios newt;
    //newt.c_lflag &= ~(ICANON);
    y = height / 2;
    x = width / 2;
    foodx = rand() % width - 1;
    foody = rand() % height - 1;
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
            if (x <= 0 || x >= width || y <= 0 || y >= height)
            {
                gameover = true;
            }
            else if (i == y && j == x)
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
    fflush(stdout);
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
        foodx = rand() % width;
        foody = rand() % height;
    }
}