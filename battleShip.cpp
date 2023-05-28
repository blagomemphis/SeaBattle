#define WINDOWS

#include <iostream>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

const int SIZE = 10; // размер игрового поля
const char SHIP = '#'; // символ корабля
const char HIT = 'X'; // символ попадания
const char KILL = '*'; // символ убитого корабля
const char MISS = '-'; // символ промаха
const char TAB = '\t';

// функция для очистки консоли
void clear() {
#ifdef WINDOWS
    std::system("cls");
#else
    // Assume POSIX
    std::system("clear");
#endif
}

// функция для заполнения игрового поля
void initialize(char board[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = '.';
        }
    }
}

// функция для вывода игрового поля пользователя
void mainDisplay(char board[][SIZE]) {
    cout << "  ";
    for (int i = 0; i < SIZE; i++) {
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < SIZE; i++) {
        cout << i << " ";
        for (int j = 0; j < SIZE; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

// функция для вывода игрового поля компьютера
void computerDisplay(char board[][SIZE]) {
    cout << "  ";
    for (int i = 0; i < SIZE; i++) {
        cout << i << " ";
    }
    cout << endl;

    for (int i = 0; i < SIZE; i++) {
        cout << i << " ";
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == HIT) {
                cout << "X ";
            }
            else if (board[i][j] == MISS) {
                cout << "- ";
            }
            else if (board[i][j] == KILL) {
                cout << "* ";
            }
            else {
                cout << "O ";
            }
        }
        cout << endl;
    }
}

// функция для размещения кораблей на игровом поле компьютера


// функция проверяет, что заданные координаты находятся в пределах поля
bool isValid(int row, int col) {
    return (row >= 0 && row < SIZE && col >= 0 && col < SIZE);
}

// функция для проверки, потоплен ли корабль
bool isSunk(char board[][SIZE], int row, int col) {
    for (int i = row; i < SIZE; i++) {
        if (board[i][col] == SHIP) return false;
        if (!isValid(i, col) || (board[i][col] != HIT && board[i][col] != KILL)) break;
    }
    for (int i = row; i > 0; i--) {
        if (board[i][col] == SHIP) return false;
        if (!isValid(i, col) || (board[i][col] != HIT && board[i][col] != KILL)) break;
    }
    for (int i = col; i < SIZE; i++) {
        if (board[row][i] == SHIP) return false;
        if (!isValid(row, i) || (board[row][i] != HIT && board[row][i] != KILL)) break;
    }
    for (int i = col; i > 0; i--) {
        if (board[row][i] == SHIP) return false;
        if (!isValid(row, i) || (board[row][i] != HIT && board[row][i] != KILL)) break;
    }

    return true;
}

bool replaceShipCells(char board[][SIZE], int row, int col) {
    if (!isSunk(board, row, col)) {
        return false;
    }

    for (int i = row; i < SIZE; i++) {
        if (board[i][col] == HIT) board[i][col] = KILL;
        if (!isValid(i, col) || board[i][col] != KILL) break;
    }
    for (int i = row; i > 0; i--) {
        if (board[i][col] == HIT) board[i][col] = KILL;
        if (!isValid(i, col) || board[i][col] != KILL) break;
    }
    for (int i = col; i < SIZE; i++) {
        if (board[row][i] == HIT) board[row][i] = KILL;
        if (!isValid(row, i) || board[row][i] != KILL) break;
    }
    for (int i = col; i > 0; i--) {
        if (board[row][i] == HIT) board[row][i] = KILL;
        if (!isValid(row, i) || board[row][i] != KILL) break;
    }

    return true;
}


// функция для проверки, есть ли корабль на заданных координатах
bool isHit(char board[][SIZE], int row, int col) {
    return (board[row][col] == SHIP || board[row][col] == HIT);

}

// функция проверяет, является ли введенная строка допустимым вводом для координат
bool isInputValid(const string& input) {
    for (char c : input) {
        if (!isdigit(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

// функция проверяет допустимость размещения корабля на игровом поле.
bool isPlacementValid(char board[][SIZE], int row, int col, int size, int direction) {
    for (int i = 0; i < size; i++) {
        int curRow = row + i * (1 - direction);
        int curCol = col + i * direction;

        if (!isValid(curRow, curCol) || board[curRow][curCol] != '.') {
            return false;
        }

        // Проверка на пересечение с другими кораблями
        if (direction == 1) {
            if ((curRow > 0 && board[curRow - 1][curCol] != '.') || (curRow < SIZE - 1 && board[curRow + 1][curCol] != '.')
                || (curCol > 0 && board[curRow][curCol - 1] != '.') || (curCol < SIZE - 1 && board[curRow][curCol + 1] != '.')
                || (curRow > 0 && curCol > 0 && board[curRow - 1][curCol - 1] != '.')
                || (curRow > 0 && curCol < SIZE - 1 && board[curRow - 1][curCol + 1] != '.')
                || (curRow < SIZE - 1 && curCol > 0 && board[curRow + 1][curCol - 1] != '.')
                || (curRow < SIZE - 1 && curCol < SIZE - 1 && board[curRow + 1][curCol + 1] != '.')) {
                return false;
            }
        }
        else { // direction == 0 (VERTICAL)
            if ((curCol > 0 && board[curRow][curCol - 1] != '.') || (curCol < SIZE - 1 && board[curRow][curCol + 1] != '.')
                || (curRow > 0 && curCol > 0 && board[curRow - 1][curCol - 1] != '.')
                || (curRow > 0 && curCol < SIZE - 1 && board[curRow - 1][curCol + 1] != '.')
                || (curRow < SIZE - 1 && curCol > 0 && board[curRow + 1][curCol - 1] != '.')
                || (curRow < SIZE - 1 && curCol < SIZE - 1 && board[curRow + 1][curCol + 1] != '.')) {
                return false;
            }
        }
    }

    return true;
}

void placeShips(char board[][SIZE]) {

    // размещение корабля длиной 4 клетки
    int row = rand() % SIZE;
    int col = rand() % (SIZE - 3);
    for (int i = 0; i < 4; i++) {
        board[row][col + i] = SHIP;
    }

    // размещение корабля длиной 3 клетки
    row = rand() % SIZE;
    col = rand() % (SIZE - 2);
    for (int i = 0; i < 3; i++) {
        // проверка, чтобы корабли не перекрывались
        while (board[row][col + i] == SHIP || !isPlacementValid(board, row, col + i, 3, 1)) {
            row = rand() % SIZE;
            col = rand() % (SIZE - 2);
        }
        board[row][col + i] = SHIP;
    }

    // размещение корабля длиной 3 клетки
    row = rand() % (SIZE - 2);
    col = rand() % SIZE;
    for (int i = 0; i < 3; i++) {
        // проверка, чтобы корабли не перекрывались
        while (board[row + i][col] == SHIP || !isPlacementValid(board, row + i, col, 3, 0)) {
            row = rand() % (SIZE - 2);
            col = rand() % SIZE;
        }
        board[row + i][col] = SHIP;
    }

    // размещение корабля длиной 2 клетки
    row = rand() % (SIZE - 1);
    col = rand() % SIZE;
    for (int i = 0; i < 2; i++) {
        // проверка, чтобы корабли не перекрывались
        while (board[row + i][col] == SHIP || !isPlacementValid(board, row + i, col, 2, 0)) {
            row = rand() % (SIZE - 1);
            col = rand() % SIZE;
        }
        board[row + i][col] = SHIP;
    }
}

//  позволяет пользователю размещать корабли на своем игровом поле.
//  позволяет пользователю размещать корабли на своем игровом поле.
void placeShipsUser(char board[][SIZE]) {
    int shipSizes[4] = { 4, 3, 3, 2 }; // размеры кораблей
    int row, col, size, direction;
    for (int i = 0; i < 4; i++) {
        size = shipSizes[i];
        cout << "Расставляем корабль длиной " << size << endl;

        // запрашиваем у пользователя координаты начальной точки
        string input;
        bool isValidInput = false;
        while (!isValidInput) {
            cout << "Введите координаты начальной точки (например, 0 0): ";
            getline(cin, input);
            istringstream iss(input);
            iss >> row >> col;

            // автоматическое определение направления корабля
            if (row + size > SIZE + 1) {
                direction = 1; // горизонтальное направление
            }
            else if (col + size > SIZE + 1) {
                cout << "col" << endl;
                direction = 0; // вертикальное направление
            }
            else {
                // запрашиваем у пользователя направление корабля
                cout << "Выберите направление корабля (0 - вертикальное, 1 - горизонтальное): ";
                getline(cin, input);
                istringstream iss2(input);
                iss2 >> direction;
            }

            isValidInput = isInputValid(input) && isValid(row, col) && isPlacementValid(board, row, col, size, direction);
            if (!isValidInput) {
                cout << "Некорректный ввод или неверные координаты. Попробуйте еще раз." << endl;
            }
        }

        // размещаем корабль на доске
        for (int j = 0; j < size; j++) {
            board[row + j * (1 - direction)][col + j * direction] = SHIP;
        }

        clear();
        mainDisplay(board);
    }
}


int main() {
    srand(time(NULL));
    setlocale(LC_ALL, "ru");

    char playerBoard[SIZE][SIZE]; // игровое поле игрока
    char computerBoard[SIZE][SIZE]; // игровое поле компьютера
    int playerHits = 12; // количество оставшихся кораблей у игрока
    int computerHits = 12; // количество оставшихся кораблей у компьютера

    initialize(playerBoard); // Инициализация игрового поля игрока
    initialize(computerBoard); // Инициализация игрового поля компьютера

    cout << "       Ваше поле " << endl;
    mainDisplay(playerBoard);
    cout << endl << "   Вражеское поле " << endl;
    computerDisplay(computerBoard);

    placeShipsUser(playerBoard);
    placeShips(computerBoard);

    while (playerHits > 0 && computerHits > 0) {
        cout << "       Ваше поле " << endl;
        mainDisplay(playerBoard);
        cout << "   Вражеское поле " << endl;
        computerDisplay(computerBoard);

        // ход игрока
        cout << "Введите координаты выстрела: ";
        int row, col;
        cin >> row >> col;
        while (!isValid(row, col) || computerBoard[row][col] == HIT || computerBoard[row][col] == KILL || computerBoard[row][col] == MISS) {
            cout << "Введите координаты выстрела: ";
            cin >> row >> col;
        }

        if (computerBoard[row][col] == SHIP) {
            computerBoard[row][col] = HIT;
            computerHits--;
            cout << "Вы попали в корабль!" << endl;
            if (replaceShipCells(computerBoard, row, col)) {
                cout << "Корабль полностью подбит!" << endl;
            }
        }
        else {
            computerBoard[row][col] = MISS;
            cout << "Вы промахнулись." << endl;
        }
        if (computerHits == 0) {
            break;
        }

        // ход компьютера
        row = rand() % SIZE;
        col = rand() % SIZE;
        while (playerBoard[row][col] == HIT || playerBoard[row][col] == MISS) {
            row = rand() % SIZE;
            col = rand() % SIZE;
        }

        if (playerBoard[row][col] == SHIP) {
            playerBoard[row][col] = HIT;
            playerHits--;
            cout << "Соперник попал в ваш корабль!" << endl;
        }
        else {
            playerBoard[row][col] = MISS;
            cout << "Соперник промахнулся." << endl;
        }

        if (playerHits == 0) {
            break;
        }

        // вывод текущего состояния игры
        cout << "Сопернику осталось нанести: " << playerHits << " выстрелов" << " | Вам осталось нанести: " << computerHits << " выстрелов" << endl;
        cout << "Нажмите ENTER для продолжения..." << endl;
        cin.ignore();
        cin.get();

        clear(); // Очищаем консоль после вывода текущего состояния игры
    }
    clear();
    cout << "       Ваше поле " << endl;
    mainDisplay(playerBoard);
    cout << "   Вражеское поле " << endl;
    computerDisplay(computerBoard);

    if (playerHits == 0) {
        cout << "Вы проиграли!" << endl;
    }
    else {
        cout << "Вы победили!" << endl;
    }

    return 0;
}
