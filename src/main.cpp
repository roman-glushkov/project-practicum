#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
using namespace std;

// Блок констант для настройки игры
const int FIELD_WIDTH = 40;          // Ширина игрового поля
const int FIELD_HEIGHT = 20;         // Высота игрового поля
const int SNAKE_LENGTH = 3;          // Начальная длина змейки
const int GAME_SPEED = 150;          // Скорость игры (задержка в миллисекундах)

// Символы для отрисовки игрового поля
const char BORDER_HORIZONTAL = '=';  // Горизонтальная граница
const char BORDER_VERTICAL = '|';    // Вертикальная граница
const char BORDER_CORNER = '+';      // Угол границы
const char EMPTY_CHAR = ' ';         // Пустая клетка
const char SNAKE_HEAD_CHAR = 'O';    // Голова змейки
const char SNAKE_BODY_CHAR = 'o';    // Тело змейки
const char APPLE_CHAR = 'A';         // Яблоко

// Текстовые сообщения для интерфейса
const string MSG_LEGEND = "O - snake head, o - snake body, A - apple";
const string MSG_SIZE = "Field size: ";
const string MSG_CONTROLS = "Controls: W - up, A - left, S - down, D - right";
const string MSG_GAME_OVER = "Game Over!";
const string MSG_SCORE = "Score: ";

// Базовый шаблон игрового поля (пустое поле)
const vector < string > BASE_GRID(FIELD_HEIGHT, string(FIELD_WIDTH, EMPTY_CHAR));

// Перечисление для направлений движения змейки
enum Direction 
{
  UP,     // Вверх
  DOWN,   // Вниз
  LEFT,   // Влево
  RIGHT   // Вправо
};

// Функция для установки позиции курсора в консоли
void SetCursorPosition(int x, int y) 
{
  COORD coord;          // Структура для координат
  coord.X = x;          // Установка X координаты
  coord.Y = y;          // Установка Y координаты
  // Установка позиции курсора в консоли
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Функция для скрытия курсора в консоли
void HideCursor() 
{
  CONSOLE_CURSOR_INFO cursorInfo;  // Структура информации о курсоре
  cursorInfo.dwSize = 100;         // Размер курсора
  cursorInfo.bVisible = FALSE;     // Сделать курсор невидимым
  // Применение настроек курсора
  SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), & cursorInfo);
}

// Функция генерации начальной змейки
vector < pair < int, int >> GenerateSnake() 
{
  int headX = FIELD_WIDTH / 2;  // Стартовая X координата головы (центр)
  int headY = FIELD_HEIGHT / 2; // Стартовая Y координата головы (центр)
  vector < pair < int, int >> snake;  // Вектор для хранения сегментов змейки
  
  // Создание змейки заданной длины
  for (int i = 0; i < SNAKE_LENGTH; ++i)
    snake.push_back({
      headX - i,  // Каждый следующий сегмент смещен влево
      headY       // Y координата одинакова для всех сегментов
    });
  return snake;
}

// Функция генерации яблока в случайной позиции
pair < int, int > GenerateApple(const vector < pair < int, int >> & snake) 
{
  while (true) 
  {
    // Генерация случайных координат для яблока
    int appleX = rand() % FIELD_WIDTH;
    int appleY = rand() % FIELD_HEIGHT;
    pair < int, int > apple = 
    {
      appleX,
      appleY
    };

    // Проверка, не попадает ли яблоко на змейку
    bool onSnake = false;
    for (const auto & segment: snake) 
    {
      if (segment == apple) 
      {
        onSnake = true;
        break;
      }
    }

    // Если яблоко не на змейке - возвращаем его позицию
    if (!onSnake) return apple;
  }
}

// Функция проверки столкновений
bool CheckCollision(const vector < pair < int, int >> & snake) 
{
  auto head = snake[0];  // Получаем голову змейки

  // Проверка столкновения с границами поля
  if (head.first < 0 || head.first >= FIELD_WIDTH ||
    head.second < 0 || head.second >= FIELD_HEIGHT)
    return true;  // Столкновение с границей

  // Проверка столкновения головы с телом змейки
  for (size_t i = 1; i < snake.size(); ++i) 
  {
    if (head == snake[i]) return true;  // Столкновение с самим собой
  }

  return false;  // Столкновений нет
}

// Функция отрисовки игрового поля
void DrawField(const vector < pair < int, int >> & snake, pair < int, int > apple, int score, bool firstDraw) 
{
  // Первоначальная отрисовка (только при первом вызове)
  if (firstDraw) 
  {
    system("cls");  // Очистка консоли
    HideCursor();   // Скрытие курсора

    // Отрисовка верхней границы
    SetCursorPosition(0, 0);
    cout << BORDER_CORNER;
    for (int i = 0; i < FIELD_WIDTH; ++i) cout << BORDER_HORIZONTAL;
    cout << BORDER_CORNER;

    // Отрисовка боковых границ
    for (int y = 0; y < FIELD_HEIGHT; ++y) 
    {
      SetCursorPosition(0, y + 1);
      cout << BORDER_VERTICAL;
      SetCursorPosition(FIELD_WIDTH + 1, y + 1);
      cout << BORDER_VERTICAL;
    }

    // Отрисовка нижней границы
    SetCursorPosition(0, FIELD_HEIGHT + 1);
    cout << BORDER_CORNER;
    for (int i = 0; i < FIELD_WIDTH; ++i) cout << BORDER_HORIZONTAL;
    cout << BORDER_CORNER;

    // Отрисовка информационных сообщений
    SetCursorPosition(0, FIELD_HEIGHT + 3);
    cout << MSG_SCORE << score;
    SetCursorPosition(0, FIELD_HEIGHT + 4);
    cout << MSG_LEGEND;
    SetCursorPosition(0, FIELD_HEIGHT + 5);
    cout << MSG_SIZE << FIELD_WIDTH << "x" << FIELD_HEIGHT;
    SetCursorPosition(0, FIELD_HEIGHT + 6);
    cout << MSG_CONTROLS;
  }

  // Очистка только тех клеток, где нет змейки или яблока
  for (int y = 0; y < FIELD_HEIGHT; ++y) 
  {
    for (int x = 0; x < FIELD_WIDTH; ++x) 
    {
      bool isSnakeOrApple = false;

      // Проверка, находится ли в этой клетке сегмент змейки
      for (const auto & segment: snake) 
      {
        if (segment.first == x && segment.second == y) 
        {
          isSnakeOrApple = true;
          break;
        }
      }

      // Проверка, находится ли в этой клетке яблоко
      if (apple.first == x && apple.second == y) 
      {
        isSnakeOrApple = true;
      }

      // Если клетка пустая - очищаем ее
      if (!isSnakeOrApple) 
      {
        SetCursorPosition(x + 1, y + 1);
        cout << EMPTY_CHAR;
      }
    }
  }

  // Отрисовка яблока
  SetCursorPosition(apple.first + 1, apple.second + 1);
  cout << APPLE_CHAR;

  // Отрисовка змейки
  for (size_t i = 0; i < snake.size(); ++i) 
  {
    auto[x, y] = snake[i];  // Получаем координаты сегмента
    SetCursorPosition(x + 1, y + 1);
    // Голова отрисовывается одним символом, тело - другим
    cout << (i == 0 ? SNAKE_HEAD_CHAR : SNAKE_BODY_CHAR);
  }

  // Обновление счета
  SetCursorPosition(MSG_SCORE.length() + 1, FIELD_HEIGHT + 3);
  cout << score;
}

// Функция движения змейки
void MoveSnake(vector < pair < int, int >> & snake, Direction dir, bool grow) 
{
  auto head = snake[0];  // Текущая позиция головы

  // Изменение координат головы в зависимости от направления
  switch (dir) 
  {
  case UP:
    head.second--;  // Движение вверх (уменьшение Y)
    break;
  case DOWN:
    head.second++;  // Движение вниз (увеличение Y)
    break;
  case LEFT:
    head.first--;   // Движение влево (уменьшение X)
    break;
  case RIGHT:
    head.first++;   // Движение вправо (увеличение X)
    break;
  }

  // Добавление новой головы в начало вектора
  snake.insert(snake.begin(), head);
  // Если змейка не растет - удаляем хвост
  if (!grow) snake.pop_back();
}

// Главная функция игры
int main() 
{
  // Инициализация генератора случайных чисел
  srand(static_cast < unsigned > (time(nullptr)));

  // Инициализация игровых объектов
  auto snake = GenerateSnake();  // Создание змейки
  auto apple = GenerateApple(snake);  // Создание первого яблока
  Direction currentDir = RIGHT;  // Начальное направление движения
  int score = 0;                 // Начальный счет
  bool gameOver = false;         // Флаг окончания игры
  bool firstDraw = true;         // Флаг первой отрисовки

  // Главный игровой цикл
  while (!gameOver) 
  {
    // Отрисовка игрового поля
    DrawField(snake, apple, score, firstDraw);
    firstDraw = false;  // После первой отрисовки сбрасываем флаг

    // Обработка пользовательского ввода
    if (_kbhit())  // Проверка нажатия клавиши
    {
      char key = _getch();  // Получение символа нажатой клавиши
      // Изменение направления в зависимости от нажатой клавиши
      switch (key) 
      {
      case 'w':
      case 'W':
        if (currentDir != DOWN) currentDir = UP;     // Вверх (если не движемся вниз)
        break;
      case 's':
      case 'S':
        if (currentDir != UP) currentDir = DOWN;     // Вниз (если не движемся вверх)
        break;
      case 'a':
      case 'A':
        if (currentDir != RIGHT) currentDir = LEFT;  // Влево (если не движемся вправо)
        break;
      case 'd':
      case 'D':
        if (currentDir != LEFT) currentDir = RIGHT;  // Вправо (если не движемся влево)
        break;
      }
    }

    // Проверка съедания яблока
    bool grow = false;
    if (snake[0] == apple)  // Если голова на яблоке
    {
      apple = GenerateApple(snake);  // Генерация нового яблока
      score++;                       // Увеличение счета
      grow = true;                   // Установка флага роста змейки
    }

    // Движение змейки
    MoveSnake(snake, currentDir, grow);

    // Проверка столкновений
    if (CheckCollision(snake)) 
    {
      gameOver = true;  // Завершение игры при столкновении
    }

    // Задержка для контроля скорости игры
    Sleep(GAME_SPEED);
  }

  // Вывод сообщения о конце игры
  SetCursorPosition(0, FIELD_HEIGHT + 8);
  cout << MSG_GAME_OVER << '\n';

  return 0;
}