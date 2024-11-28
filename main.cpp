#include <iostream>
#include <vector>
using namespace std;

class GappyPuzzle 
{
public:

    GappyPuzzle(int size) : size(size), grid(size, vector<char>(size, ' ')) {}

    void startGame() 
    {

        displayMenu();

        if (!confirmStart()) 
        {
            cout << "Игра не начата. Выход..." << endl;
            return;
        }

        printGrid();

        while (true) 
        {
            int row, col;
            cout << "Введите координаты черной клетки (row col) или -1 для выхода: "; cin >> row;

            
            if (row == -1) 
            {
                cout << "Выход из игры." << endl;
                break;
            }

            cin >> col;

            
            if (row < 0 || row >= size || col < 0 || col >= size) 
            {
                cout << "Координаты вне границ! Попробуйте снова." << endl;
                continue;
            }

            if (placeBlackCell(row, col)) 
            {
                printGrid();
                if (checkWin()) 
                {
                    cout << "Поздравляем! Вы выиграли!" << endl;
                    break;
                }
            } else cout << "Нельзя поставить черную клетку здесь! Попробуйте снова." << endl;
        }
    }

private:

    int size;
    vector<vector<char>> grid;

    void displayMenu() 
    {
        cout << "Добро пожаловать в игру Gappy!" << endl;
        cout << "Правила игры:" << endl;
        cout << "1. Ваша цель - разместить черные клетки в квадратной сетке." << endl;
        cout << "2. В каждом ряду и столбце должно быть ровно 2 черные клетки." << endl;
        cout << "3. Черные клетки не могут касаться друг друга, даже углами." << endl;
        cout << "4. Вводите координаты для размещения черных клеток в формате: row col" << endl;
        cout << "5. Введите -1 для выхода из игры." << endl;
        cout << endl;
    }

    bool confirmStart() 
    {
        char choice;
        cout << "Вы готовы начать игру? (y/n): "; cin >> choice;
        return (toupper(choice) == 'Y');
    }

    bool placeBlackCell(int row, int col) 
    {
        if (grid[row][col] != ' ') return false;
        if (!canPlaceBlackCell(row, col)) return false;

        grid[row][col] = '#';
        return true;
    }

    bool canPlaceBlackCell(int row, int col) 
    {
        for (int i = -1; i <= 1; ++i) 
        {
            for (int j = -1; j <= 1; ++j) 
            {
                if (i == 0 && j == 0) continue;
                int newRow = row + i, newCol = col + j;
                if (newRow >= 0 && newRow < size && newCol >= 0 && newCol < size && (grid[newRow][newCol] == '#')) return false;
            }
        }
        return true;
    }

    void printGrid() 
    {
        cout << "Текущая сетка:" << endl;

        for (auto &row : grid) 
        {
            for (char cell : row) cout << (cell == ' ' ? '-' : cell) << " ";
            cout << endl;
        }
    }

    bool checkWin() 
    {
        for (int i = 0; i < size; ++i) 
        {
            int rowCount = 0, colCount = 0;

            for (int j = 0; j < size; ++j) 
            {
                if (grid[i][j] == '#') rowCount++;
                if (grid[j][i] == '#') colCount++;
            }
            if (rowCount != 2 || colCount != 2) return false;
        }
        return true;
    }
};

int main() 
{
    int size = 9;
    
    GappyPuzzle puzzle(size);
    puzzle.startGame();
    return 0;
}
