#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Класс для представления ячейки
class Cell 
{
public:
    bool hasMine;
    bool revealed;
    int neighboringMines;

    Cell() : hasMine(false), revealed(false), neighboringMines(0) {}
};

// Класс для игры Сапер
class Minesweeper 
{
private:
    vector<vector<Cell>> grid;
    int width;
    int height;
    int mines;

    // Размещение мин на поле
    void placeMines() 
    {
        srand(static_cast<unsigned int>(time(0)));

        for (int i = 0; i < mines; ) 
        {
            int x = rand() % height;
            int y = rand() % width;

            if (!grid[x][y].hasMine) 
            {
                grid[x][y].hasMine = true;
                i++;
            }
        }
    }

    // Подсчет соседних мин
    void calculateNeighboringMines() 
    {
        for (int x = 0; x < height; x++) 
        {
            for (int y = 0; y < width; y++) 
            {
                if (grid[x][y].hasMine) 
                {
                    for (int dx = -1; dx <= 1; dx++) 
                    {
                        for (int dy = -1; dy <= 1; dy++) 
                        {
                            int nx = x + dx;
                            int ny = y + dy;

                            if (nx >= 0 && nx < height && ny >= 0 && ny < width) 
                            {
                                grid[nx][ny].neighboringMines++;
                            }
                        }
                    }
                }
            }
        }
    }

public:
    Minesweeper(int w, int h, int m) : width(w), height(h), mines(m) 
    {
        grid.resize(height, vector<Cell>(width));
        placeMines();
        calculateNeighboringMines();
    }

    // Раскрытие клетки 
    bool reveal(int x, int y) 
    {
        if (x < 0 || x >= height || y < 0 || y >= width || grid[x][y].revealed) 
        {
            return true;
        }

        grid[x][y].revealed = true;

        if (grid[x][y].neighboringMines == 0 && !grid[x][y].hasMine) 
        {
            for (int dx = -1; dx <= 1; dx++) 
            {
                for (int dy = -1; dy <= 1; dy++) 
                {
                    reveal(x + dx, y + dy); 
                }
            }
        }

        return !grid[x][y].hasMine;
    }


    // Отображение игрового поля
    void display() 
    {
        cout << "  ";

        for (int y = 0; y < width; y++) cout << y << " ";
        cout << endl;

        for (int x = 0; x < height; x++) 
        {
            cout << x << " ";

            for (int y = 0; y < width; y++) 
            {
                if (grid[x][y].revealed) 
                {
                    if (grid[x][y].hasMine) 
                    {
                        cout << "* ";
                    } else {
                        cout << grid[x][y].neighboringMines << " ";
                    }
                } else {
                    cout << ". ";
                }
            }
            cout << endl;
        }
    }
};

class Game 
{
private:
    Minesweeper* game;
public:
    Game(int width, int height, int mines) 
    {
        game = new Minesweeper(width, height, mines);
    }

    void play() 
    {
        int x, y;

        while (true) 
        {
            game->display();
            cout << "Введите координаты (x y) для раскрытия: "; cin >> x >> y;

            if (!(*game).reveal(x, y)) 
            {
                cout << "Игра окончена! Вы попали на мину." << endl;
                game->display();
                break;
            }
        }
    }
};

int main() 
{
    Game game(10, 10, 10);
    game.play();
    return 0;
}