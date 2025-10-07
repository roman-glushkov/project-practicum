#include <iostream>
#include <vector>
#include <utility>
#include <string>

using namespace std;

const int FIELD_WIDTH  = 30;
const int FIELD_HEIGHT = 100;

const char BORDER_CHAR = '#';
const char EMPTY_CHAR  = ' ';
const char SNAKE_HEAD_CHAR = 'O';
const char SNAKE_BODY_CHAR = 'o';
const char APPLE_CHAR = 'X';

const string MSG_LEGEND = "O - snake head, o - snake body, X - apple";
const string MSG_SIZE   = "Field size: ";

const vector<string> BASE_GRID(FIELD_HEIGHT, string(FIELD_WIDTH, EMPTY_CHAR));

void DrawApple(vector<string>& grid, const pair<int, int>& applePosition)
{
    int x = applePosition.first;
    int y = applePosition.second;

    if (x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT)
        grid[y][x] = APPLE_CHAR;
}

void DrawSnake(vector<string>& grid, const vector<pair<int, int>>& snakeCoordinates)
{
    for (size_t i = 0; i < snakeCoordinates.size(); ++i)
    {
        int x = snakeCoordinates[i].first;
        int y = snakeCoordinates[i].second;

        if (x >= 0 && x < FIELD_WIDTH && y >= 0 && y < FIELD_HEIGHT)
            grid[y][x] = (i == 0 ? SNAKE_HEAD_CHAR : SNAKE_BODY_CHAR);
    }
}

void DrawBorders()
{
    for (int i = 0; i < FIELD_WIDTH + 2; ++i)
        cout << BORDER_CHAR;
    cout << '\n';
}

void DrawField(
    const vector<pair<int, int>>& snakeCoordinates,
    const pair<int, int>& applePosition
)
{
    vector<string> grid = BASE_GRID;

    DrawApple(grid, applePosition);
    DrawSnake(grid, snakeCoordinates);

    DrawBorders();

    for (int y = 0; y < FIELD_HEIGHT; ++y)
    {
        cout << BORDER_CHAR;
        cout << grid[y];
        cout << BORDER_CHAR << '\n';
    }

    DrawBorders();
}

int main()
{
    int centerX = FIELD_WIDTH / 2;
    int centerY = FIELD_HEIGHT / 2;

    pair<int, int> apple = { centerX + 5, centerY - 3 };

    vector<pair<int, int>> snake = {
        { centerX - 2, centerY },
        { centerX - 3, centerY },
        { centerX - 4, centerY },
        { centerX - 5, centerY }
    };

    DrawField(snake, apple);

    cout << MSG_LEGEND << '\n';
    cout << MSG_SIZE << FIELD_WIDTH << "x" << FIELD_HEIGHT << '\n';

    return 0;
}
