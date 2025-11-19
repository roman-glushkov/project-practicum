#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>

using namespace std;

const int FIELD_WIDTH = 40;
const int FIELD_HEIGHT = 20;
const int INITIAL_SNAKE_LENGTH = 3;
const int GAME_SPEED = 150;
const int SCORE_POSITION_X = 8;
const int INFO_START_Y = 23;
const int CURSOR_SIZE = 100;
const int FIELD_CENTER_X = 20;
const int FIELD_CENTER_Y = 10;
const int BORDER_OFFSET = 1;

const char BORDER_HORIZONTAL = '=';
const char BORDER_VERTICAL = '|';
const char BORDER_CORNER = '+';
const char EMPTY_CHAR = ' ';
const char SNAKE_HEAD_CHAR = 'O';
const char SNAKE_BODY_CHAR = 'o';
const char APPLE_CHAR = 'A';

const string MSG_LEGEND = "O - snake head, o - snake body, A - apple";
const string MSG_SIZE = "Field size: ";
const string MSG_CONTROLS = "Controls: W - up, A - left, S - down, D - right";
const string MSG_GAME_OVER = "Game Over!";
const string MSG_SCORE = "Score: ";

enum Direction 
{
  UP,
  DOWN,
  LEFT,
  RIGHT
};

void SetCursorPosition(int x, int y) 
{
  COORD coord = 
  {
    static_cast < SHORT > (x),
    static_cast < SHORT > (y)
  };
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void HideCursor() 
{
  CONSOLE_CURSOR_INFO cursorInfo;
  cursorInfo.dwSize = CURSOR_SIZE;
  cursorInfo.bVisible = FALSE;
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), & cursorInfo);
}

vector < pair < int, int >> GenerateSnake() 
{
  vector < pair < int, int >> snake;

  for (int i = 0; i < INITIAL_SNAKE_LENGTH; ++i)
    snake.push_back({
      FIELD_CENTER_X - i,
      FIELD_CENTER_Y
    });

  return snake;
}

pair < int, int > GenerateApple(const vector < pair < int, int >> & snake) 
{
  while (true) 
  {
    int appleX = rand() % FIELD_WIDTH;
    int appleY = rand() % FIELD_HEIGHT;
    pair < int, int > apple = 
    {
      appleX,
      appleY
    };

    bool onSnake = false;
    for (const auto & segment: snake) 
    {
      if (segment == apple)
      {
        onSnake = true;
        break;
      }
    }

    if (!onSnake) return apple;
  }
}

bool CheckCollision(const vector < pair < int, int >> & snake) 
{
  auto head = snake[0];

  if (head.first < 0 || head.first >= FIELD_WIDTH ||
    head.second < 0 || head.second >= FIELD_HEIGHT)
    return true;

  for (size_t i = 1; i < snake.size(); ++i) 
  {
    if (head == snake[i]) return true;
  }

  return false;
}

void DrawBorders()
 {
  SetCursorPosition(0, 0);
  cout << BORDER_CORNER;
  for (int i = 0; i < FIELD_WIDTH; ++i) cout << BORDER_HORIZONTAL;
  cout << BORDER_CORNER;

  for (int y = 0; y < FIELD_HEIGHT; ++y)
   {
    SetCursorPosition(0, y + BORDER_OFFSET);
    cout << BORDER_VERTICAL;
    SetCursorPosition(FIELD_WIDTH + BORDER_OFFSET, y + BORDER_OFFSET);
    cout << BORDER_VERTICAL;
  }

  SetCursorPosition(0, FIELD_HEIGHT + BORDER_OFFSET);
  cout << BORDER_CORNER;
  for (int i = 0; i < FIELD_WIDTH; ++i) cout << BORDER_HORIZONTAL;
  cout << BORDER_CORNER;
}

void DrawInfo(int score) 
{
  SetCursorPosition(0, INFO_START_Y);
  cout << MSG_SCORE << score;
  SetCursorPosition(0, INFO_START_Y + 1);
  cout << MSG_LEGEND;
  SetCursorPosition(0, INFO_START_Y + 2);
  cout << MSG_SIZE << FIELD_WIDTH << "x" << FIELD_HEIGHT;
  SetCursorPosition(0, INFO_START_Y + 3);
  cout << MSG_CONTROLS;
}

void ClearOldPositions(const vector < pair < int, int >> & oldSnake,
  const vector < pair < int, int >> & newSnake,
    const pair < int, int > & apple)
    {
  for (const auto & segment: oldSnake) 
  {
    bool isInNewSnake = false;
    for (const auto & newSegment: newSnake) 
    {
      if (segment == newSegment) 
      {
        isInNewSnake = true;
        break;
      }
    }

    if (!isInNewSnake && segment != apple) {
      SetCursorPosition(segment.first + BORDER_OFFSET, segment.second + BORDER_OFFSET);
      cout << EMPTY_CHAR;
    }
  }
}

void DrawField(const vector < pair < int, int >> & snake,
  const vector < pair < int, int >> & oldSnake, pair < int, int > apple, int score, bool firstDraw) 
  {
  if (firstDraw) 
  {
    system("cls");
    HideCursor();
    DrawBorders();
    DrawInfo(score);
  }

  if (!firstDraw) 
  {
    ClearOldPositions(oldSnake, snake, apple);
  }

  SetCursorPosition(apple.first + BORDER_OFFSET, apple.second + BORDER_OFFSET);
  cout << APPLE_CHAR;

  for (size_t i = 0; i < snake.size(); ++i)
  {
    auto[x, y] = snake[i];
    SetCursorPosition(x + BORDER_OFFSET, y + BORDER_OFFSET);
    cout << (i == 0 ? SNAKE_HEAD_CHAR : SNAKE_BODY_CHAR);
  }

  SetCursorPosition(SCORE_POSITION_X, INFO_START_Y);
  cout << score;
}

void MoveSnake(vector < pair < int, int >> & snake, Direction dir, bool grow) 
{
  auto head = snake[0];

  switch (dir) 
  {
  case UP:
    head.second--;
    break;
  case DOWN:
    head.second++;
    break;
  case LEFT:
    head.first--;
    break;
  case RIGHT:
    head.first++;
    break;
  }

  snake.insert(snake.begin(), head);
  if (!grow) snake.pop_back();
}

Direction HandleInput(Direction currentDir) 
{
  if (_kbhit()) 
  {
    char key = _getch();
    switch (key) 
    {
    case 'w':
    case 'W':
      if (currentDir != DOWN) return UP;
      break;
    case 's':
    case 'S':
      if (currentDir != UP) return DOWN;
      break;
    case 'a':
    case 'A':
      if (currentDir != RIGHT) return LEFT;
      break;
    case 'd':
    case 'D':
      if (currentDir != LEFT) return RIGHT;
      break;
    }
  }
  return currentDir;
}

int main() {
  srand(static_cast < unsigned > (time(nullptr)));

  auto snake = GenerateSnake();
  auto oldSnake = snake;
  auto apple = GenerateApple(snake);
  Direction currentDir = RIGHT;
  int score = 0;
  bool gameOver = false;
  bool firstDraw = true;

  while (!gameOver) 
  {
    DrawField(snake, oldSnake, apple, score, firstDraw);
    firstDraw = false;

    oldSnake = snake;
    currentDir = HandleInput(currentDir);

    bool grow = false;
    if (snake[0] == apple) 
    {
      apple = GenerateApple(snake);
      score++;
      grow = true;
    }

    MoveSnake(snake, currentDir, grow);

    if (CheckCollision(snake)) {
      gameOver = true;
    }

    Sleep(GAME_SPEED);
  }

  SetCursorPosition(0, INFO_START_Y + 5);
  cout << MSG_GAME_OVER << '\n';

  return 0;
}