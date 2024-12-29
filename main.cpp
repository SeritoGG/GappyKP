#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <cctype>
#include <stdexcept>    

using namespace std;

class GappyPuzzle {
public:
    GappyPuzzle() : size(9), grid(size, vector<char>(size, ' ')) {}

    void startGame() 
    {
        displayMenu();
        int choice;
        while (true) 
        {
            choice = displayMainMenu();
            switch (choice) 
            {
            case 1:
                selectDifficulty();
                break;
            case 2:
                startNewGame();
                return;
            case 3:
                loadSavedGame();
                return;
            case 4:
                return;
            default:
                cout << "Неверный выбор. Пожалуйста, выберите еще раз." << endl;
                cin.clear();
                break;
            }
        }
    }

private:
    int size;
    vector<vector<char>> grid;

    int displayMainMenu() 
    {
        cout << "    Gappy Puzzle   " << endl;
        cout << "-------------------" << endl;
        cout << "1. Выбрать уровень сложности" << endl;
        cout << "2. Новая игра" << endl;
        cout << "3. Загрузить игру" << endl;
        cout << "4. Выход" << endl;
        cout << "Выберите пункт меню: ";

        int choice;
        cin >> choice;
        return choice;
    }

    void selectDifficulty() 
    {
        int choice = 0;

        cout << "    Выбор уровня сложности    " << endl;
        cout << "______________________________" << endl;
        cout << "1. Легкий (6x6)" << endl;
        cout << "2. Средний (9x9)" << endl;
        cout << "3. Сложный (12x12)" << endl;
        cout << "Выберите уровень сложности: ";
        cin >> choice;
        
        switch (choice) {
            case 1:
                size = 6;
                grid.assign(size, vector<char>(size, ' '));
                break;
            case 2:
                size = 9;
                grid.assign(size, vector<char>(size, ' '));
                break;
            case 3:
                size = 12;
                grid.assign(size, vector<char>(size, ' '));
                break;
            default:
                cout << "Неверный выбор. Пожалуйста, выберите еще раз." << endl;
                cin.clear();
                break;
         }
    }
    void startNewGame() 
    {
        printGrid();

        while (true) 
        {
            int row, col;
            cout << "Введите координаты черной клетки (row col) или -1 для выхода или -2 для сохранения: ";
            cin >> row;

            if (row == -2) 
            {
                if (saveGame()) 
                {
                    cout << "Игра сохранена в файл." << endl;
                } else {
                    cout << "Не удалось сохранить игру" << endl;
                }
                continue;
            }

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
            } else {
                cout << "Нельзя поставить черную клетку здесь! Попробуйте снова." << endl;
            }
        }
    }

    void loadSavedGame() 
    {
        try 
        {
            loadGame();
             cout << "Игра загружена из файла." << endl;
           startNewGame();
        } catch (const runtime_error& error) {
            cout << "Не удалось загрузить игру: " << error.what() << endl;
           startNewGame();
        }
    }
    void displayMenu() 
    {
            cout << "Добро пожаловать в игру Gappy!" << endl;
            cout << "Правила игры:" << endl;
            cout << "Ваша цель - разместить черные клетки в квадратной сетке." << endl;
            cout << "В каждом ряду и столбце должно быть ровно 2 черные клетки." << endl;
            cout << "Черные клетки не могут касаться друг друга, даже углами." << endl;
            cout << "Вводите координаты для размещения черных клеток в формате: row col" << endl;
            cout << "Введите -1 для выхода из игры. Введите -2 для сохранения игры." << endl;
            cout << endl;
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
        for (const auto& row : grid) 
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

    bool saveGame() 
    {
        ofstream file("gappy_save.txt");
        if (!file.is_open()) {return false;}

        file << size << endl;
        for (int i = 0; i < size; ++i) 
        {
            for (int j = 0; j < size; ++j) 
            {
                if (grid[i][j] == '#') 
                {
                    file << i << " " << j << endl;
                }
            }
        }
        file.close();
        return true;
    }

    void loadGame() 
    {
        ifstream file("gappy_save.txt");
        if (!file.is_open()) {throw runtime_error("Не удалось открыть файл сохранения.");}

        int loadedSize;

        if (!(file >> loadedSize)) 
        {
            file.close();
            throw runtime_error("Не удалось прочитать размер сетки из файла.");
        }
         if (loadedSize <= 0) 
         {
             file.close();
            throw runtime_error("Неверный размер сетки в файле сохранения.");
          }
        if (loadedSize != size) 
        {
            size = loadedSize;
            grid.assign(size, vector<char>(size, ' '));
        }
        file.ignore();

        int row, col;
        while (file >> row >> col) 
        {
            if (row < 0 || row >= size || col < 0 || col >= size) 
            {
                file.close();
               throw runtime_error("Некорректные координаты черных клеток в файле.");
            }
            grid[row][col] = '#';
        }
        if (!file.eof() && file.fail())
        {
            file.close();
            throw runtime_error("Ошибка при чтении координат черных клеток из файла.");
        }
        file.close();
    }
};

int main() {
    GappyPuzzle puzzle;
    puzzle.startGame();
    return 0;
}
