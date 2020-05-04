#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

// global variable declarations
int height = 20, width = 30;
int y = height / 2, x = width / 2, foodx, foody, score;
bool gameover = false;
int tailx[100], taily[100], ntail = 0, diff, diffl;
char set[5];
enum dir
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};
dir sdir;

// function declarations
void board();
void input();
void move();
void logic();
void foodgen();
void tailgen();
int kbhit(void);
void SetCursorPos(int XPos, int YPos);
void settings();
void difficulty();

int main()
{
  srand(clock());

  system("stty -icanon");
  difficulty();
  settings();
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
        cout << "-";
      }
      else if (j == 0 || j == width)
      {
        cout << "|";
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
  usleep(diffl);
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
  if (set[0] == 'n' || set[0] == 'N')
  {
    if (x <= 0 || x >= width || y <= 0 || y >= height)
    {
      gameover = true;
    }
  }
  if (set[0] == 'y' || set[0] == 'Y')
  {
    if (x <= 0)
    {
      x = width - 1;
    }
    if (x >= width)
    {
      x = 0;
    }
    if (y <= 0)
    {
      y = height - 1;
    }
    if (y >= height)
    {
      y = 0;
    }
  }

  if (set[1] == 'y' || set[1] == 'Y')
  {
    for (int i = 0; i < ntail; i++)
    {
      if (x == tailx[i] && y == taily[i])
      {
        gameover = true;
      }
    }
  }
  if (set[1] == 'n' || set[1] == 'N')
  {
    for (int i = 0; i < ntail; i++)
    {
      if (x == tailx[i] && y == taily[i])
      {
        for (int j = i; j < ntail; j++)
        {
          tailx[j] = taily[j] = 0;
        }
        ntail = ntail - i;
        score = score - i;
      }
    }
  }
}

void foodgen()
{
  foodx = rand() % width - 1;
  foody = rand() % height - 1;
  if (foodx <= 0 || foody <= 0)
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
void difficulty()
{
  cout << "1)Easy\n2)Normal\n3)Hard\n";
  cin >> diff;
  if (diff == 1)
  {
    diffl = 300000;
  }
  else if (diff == 2)
  {
    diffl = 200000;
  }
  else if (diff == 3)
  {
    diffl = 100000;
  }
}
void settings()
{
  printf("do you want passthrough wall\n");
  cin >> set[0];
  printf("do you want to end the game when you cut the tail\n");
  cin >> set[1];
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
