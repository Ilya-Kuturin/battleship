#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

const int SIZE = 10;
const char EMPTY = '~';
const char SHIP = 'O';
const char HIT = 'X';
const char MISS = '.';

void showBoard(char board[SIZE][SIZE], bool showShips) {
    cout << "  ";
    for (int i = 0; i < SIZE; i++) cout << i << " ";
    cout << endl;
    for (int i = 0; i < SIZE; i++) {
        cout << i << " ";
        for (int j = 0; j < SIZE; j++) {
            if (!showShips && board[i][j] == SHIP)
                cout << EMPTY << " ";
            else
                cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

void fillBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = EMPTY;
}

bool canPlaceShip(char board[SIZE][SIZE], int x, int y, int size, bool horizontal) {
    int dx = horizontal ? 1 : 0;
    int dy = horizontal ? 0 : 1;

    if (x + dx * (size - 1) >= SIZE || y + dy * (size - 1) >= SIZE)
        return false;

    for (int i = -1; i <= size; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int checkX = x + i * dx + j * dy;
            int checkY = y + i * dy + j * dx;

            if (checkX >= 0 && checkX < SIZE && checkY >= 0 && checkY < SIZE) {
                if (board[checkY][checkX] == SHIP)
                    return false;
            }
        }
    }
    return true;
}

bool placeShip(char board[SIZE][SIZE], int size) {
    int x = rand() % SIZE;
    int y = rand() % SIZE;
    bool horizontal = rand() % 2;

    if (!canPlaceShip(board, x, y, size, horizontal))
        return false;

    for (int i = 0; i < size; i++) {
        int nx = horizontal ? x + i : x;
        int ny = horizontal ? y : y + i;
        board[ny][nx] = SHIP;
    }
    return true;
}

void placeAllShips(char board[SIZE][SIZE], vector<int> ships) {
    for (int size : ships) {
        while (!placeShip(board, size));
    }
}

bool isValid(int x, int y) {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

bool isUntried(char board[SIZE][SIZE], int x, int y) {
    return board[y][x] != HIT && board[y][x] != MISS;
}

bool shoot(char board[SIZE][SIZE], int x, int y) {
    if (!isValid(x, y) || !isUntried(board, x, y)) {
        return false;
    }

    if (board[y][x] == SHIP) {
        board[y][x] = HIT;
        return true;
    }
    if (board[y][x] == EMPTY) {
        board[y][x] = MISS;
    }
    return false;
}

bool hasShips(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == SHIP)
                return true;
    return false;
}

int main() {
    srand(time(0));
    setlocale(LC_ALL, "RU");

    char player[SIZE][SIZE], enemy[SIZE][SIZE];
    vector<int> ships = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

    fillBoard(player);
    fillBoard(enemy);
    placeAllShips(player, ships);
    placeAllShips(enemy, ships);

    // Состояние AI компьютера
    int missesCount = 0;
    int shootsCount = 0;
    int computerState = 0; // 0 - случайные выстрелы, 1 - добивание корабля
    int lastHitX = -1, lastHitY = -1;
    int direction = -1; // 0-вправо, 1-влево, 2-вниз, 3-вверх
    int startX, startY; // Начальная точка корабля для добивания

    // Состояние AI компьютера2
    int missesCount2 = 0;
    int shootsCount2 = 0;
    int computerState2 = 0; // 0 - случайные выстрелы, 1 - добивание корабля
    int lastHitX2 = -1, lastHitY2 = -1;
    int direction2 = -1; // 0-вправо, 1-влево, 2-вниз, 3-вверх
    int startX2, startY2; // Начальная точка корабля для добивания

    while (hasShips(player) && hasShips(enemy)) {
        showBoard(player, true);
        cout << endl;
        showBoard(enemy, false);

        // Ход компьютера
        bool computerTurn2 = true;
        cout << "Ход компьютера2!" << endl;
        while (computerTurn2 && hasShips(enemy)) {
            int x, y;
            bool hit = false;

            if (computerState2 == 0) {
                // Режим случайных выстрелов
                do {
                    x = rand() % SIZE;
                    y = rand() % SIZE;
                } while (!isUntried(enemy, x, y));

                hit = shoot(enemy, x, y);

                if (hit) {
                    computerState2 = 1; // Переходим в режим добивания
                    lastHitX2 = x;
                    lastHitY2 = y;
                    startX2 = x;
                    startY2 = y;
                    direction2 = 0; // Начинаем с направления ВПРАВО (0-3)
                    shootsCount2++;
                    cout << "Компьютер2 попал в (" << x << ", " << y << ")!\n";
                }
                else {
                    cout << "Компьютер2 промахнулся в (" << x << ", " << y << ").\n";
                    computerTurn2 = false; // Конец хода после промаха
                }
            }
            else {
                // Режим добивания
                x = lastHitX2;
                y = lastHitY2;

                // Пробуем выстрелить в текущем направлении
                switch (direction2) {
                case 0: x++; break; // вправо
                case 1: x--; break; // влево
                case 2: y++; break; // вниз
                case 3: y--; break; // вверх
                }

                if (isValid(x, y) && isUntried(enemy, x, y)) {
                    hit = shoot(enemy, x, y);

                    if (hit) {
                        lastHitX2 = x;
                        lastHitY2 = y;
                        shootsCount2++;
                        cout << "Компьютер2 попал в (" << x << ", " << y << ")!\n";
                        cout << shootsCount2 << " " << missesCount2 << endl;
                    }
                    else {
                        cout << "Компьютер2 промахнулся в (" << x << ", " << y << ").\n";
                        cout << shootsCount2 << " " << missesCount2 << endl;
                        if (shootsCount2 >= 2) {
                            missesCount2++;
                        }
                        computerTurn2 = false; // Конец хода после промаха

                        if (missesCount2 >= 2) {
                            // Если 2 промаха подряд - возвращаемся к случайным выстрелам
                            computerState2 = 0;
                            shootsCount2 = 0;
                            missesCount2 = 0;
                        }
                        else {
                            // Пробуем следующее направление в следующий ход
                            direction2 = direction2 + 1;
                            if (direction == 4) {
                                computerState2 = 0;
                                shootsCount2 = 0;
                                missesCount2 = 0;
                            }
                            lastHitX2 = startX2;
                            lastHitY2 = startY2;
                        }
                    }
                }
                else {
                    // Если упёрлись в границу - пробуем следующее направление
                    direction2 = direction2 + 1;
                    if (direction2 == 4) {
                        computerState2 = 0;
                        shootsCount2 = 0;
                        missesCount2 = 0;
                    }
                    lastHitX2 = startX2;
                    lastHitY2 = startY2;
                    if (shootsCount2 >= 2) {
                        missesCount2++;
                    }
                    if (missesCount2 >= 2) {
                        // Если 2 промаха подряд - возвращаемся к случайным выстрелам
                        computerState2 = 0;
                        shootsCount2 = 0;
                        missesCount2 = 0;
                    }
                }
            }
        }

        if (!hasShips(enemy)) {
            cout << "ПК2 победил! " << endl;
            break;
        }

        // Ход компьютера
        bool computerTurn = true;
        cout << "Ход компьютера!" << endl;
        while (computerTurn && hasShips(player)) {
            int x, y;
            bool hit = false;

            if (computerState == 0) {
                // Режим случайных выстрелов
                do {
                    x = rand() % SIZE;
                    y = rand() % SIZE;
                } while (!isUntried(player, x, y));

                hit = shoot(player, x, y);

                if (hit) {
                    computerState = 1; // Переходим в режим добивания
                    lastHitX = x;
                    lastHitY = y;
                    startX = x;
                    startY = y;
                    direction = 0; // Начинаем с направления ВПРАВО (0-3)
                    shootsCount++;
                    cout << "Компьютер попал в (" << x << ", " << y << ")!\n";
                }
                else {
                    cout << "Компьютер промахнулся в (" << x << ", " << y << ").\n";
                    computerTurn = false; // Конец хода после промаха
                }
            }
            else {
                // Режим добивания
                x = lastHitX;
                y = lastHitY;

                // Пробуем выстрелить в текущем направлении
                switch (direction) {
                case 0: x++; break; // вправо
                case 1: x--; break; // влево
                case 2: y++; break; // вниз
                case 3: y--; break; // вверх
                }

                if (isValid(x, y) && isUntried(player, x, y)) {
                    hit = shoot(player, x, y);

                    if (hit) {
                        lastHitX = x;
                        lastHitY = y;
                        shootsCount++;
                        cout << "Компьютер попал в (" << x << ", " << y << ")!\n";
                        cout << shootsCount << " " << missesCount << endl;
                    }
                    else {
                        cout << "Компьютер промахнулся в (" << x << ", " << y << ").\n";
                        cout << shootsCount << " " << missesCount << endl;
                        if (shootsCount >= 2) {
                            missesCount++;
                        }
                        computerTurn = false; // Конец хода после промаха

                        if (missesCount >= 2) {
                            // Если 2 промаха подряд - возвращаемся к случайным выстрелам
                            computerState = 0;
                            shootsCount = 0;
                            missesCount = 0;
                        }
                        else {
                            // Пробуем следующее направление в следующий ход
                            direction = direction + 1;
                            if (direction == 4) {
                                computerState = 0;
                                shootsCount = 0;
                                missesCount = 0;
                            }
                            lastHitX = startX;
                            lastHitY = startY;
                        }
                    }
                }
                else {
                    // Если упёрлись в границу - пробуем следующее направление
                    direction = direction + 1;
                    if (direction == 4) {
                        computerState = 0;
                        shootsCount = 0;
                        missesCount = 0;
                    }
                    lastHitX = startX;
                    lastHitY = startY;
                    if (shootsCount >= 2) {
                        missesCount++;
                    }
                    if (missesCount >= 2) {
                        // Если 2 промаха подряд - возвращаемся к случайным выстрелам
                        computerState = 0;
                        shootsCount = 0;
                        missesCount = 0;
                    }
                }
            }
        }

        if (!hasShips(player)) {
            cout << "ПК1 победил!" << endl;
            break;
        }
    }

    showBoard(player, true);
    showBoard(enemy, true);

    return 0;
}