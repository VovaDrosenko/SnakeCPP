#include <iostream>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <ctime>

using namespace std;

const int WIDTH = 20;
const int HEIGHT = 10;

vector<string> options = { "New Game", "Scores", "Exit" };
int option = 0;
bool isPlay = false;
int score = 0;
char snakeSymb = '*';
char fruit = '@';

enum directionList { dirLeft, dirRight, up, down, wait };
directionList direction = wait;

struct Position {
    int x, y;
};

Position head = { WIDTH / 2, HEIGHT / 2 };
Position point;
vector<Position> snake;

bool operator==(const Position& a, const Position& b) {
    return a.x == b.x && a.y == b.y;
}

void displayMenu() {
    system("cls");
    for (int q = 0; q < options.size(); q++) {
        string arrow = (option == q) ? " -> " : "    ";
        cout << arrow << options[q] << endl;
    }
}

void displayMap() {
    system("cls");
    cout << "\n\tScore: " << score << "\n";
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                cout << "*";
            }
            else {
                bool isSnake = false;
                for (int s = 0; s < snake.size(); s++) {
                    if (i == snake[s].y && j == snake[s].x) {
                        cout << snakeSymb;
                        isSnake = true;
                        break;
                    }
                }
                if (i == point.y && j == point.x) {
                    cout << fruit;
                }
                else if (!isSnake) {
                    cout << " ";
                }
            }
        }
        cout << endl;
    }
}

void movement() {
    if (_kbhit()) {
        int key = _getch();
        if (key == 224) key = _getch();

        switch (key) {
        case 72: if (direction != down) direction = up; break;
        case 80: if (direction != up) direction = down; break;
        case 77: if (direction != dirLeft) direction = dirRight; break;
        case 75: if (direction != dirRight) direction = dirLeft; break;
        }
    }
}

void spawnPoint() {
    point = { 1 + rand() % (WIDTH - 2), 1 + rand() % (HEIGHT - 2) };
}

void logic() {
    Position prev = head;
    Position prev2;

    for (auto& s : snake) {
        prev2 = s;
        s = prev;
        prev = prev2;
    }

    if (direction == up) --head.y;
    if (direction == down) ++head.y;
    if (direction == dirLeft) --head.x;
    if (direction == dirRight) ++head.x;

    if (head.y == 0) head.y = HEIGHT - 2;
    if (head.y == HEIGHT - 1) head.y = 1;
    if (head.x == 0) head.x = WIDTH - 2;
    if (head.x == WIDTH - 1) head.x = 1;

    if (head == point) {
        score++;
        spawnPoint();
        snake.push_back(snake.back());
    }
}

void startGame(bool gameMode) {
    isPlay = gameMode;
    direction = wait;
    head = { WIDTH / 2, HEIGHT / 2 };
    snake.clear();
    snake.push_back(head);
    score = 0;
    spawnPoint();

    while (isPlay) {
        displayMap();
        movement();
        logic();
        Sleep(100);
    }
}

int main() {
    srand(time(0));
    displayMenu();

    while (true) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 224) key = _getch();

            switch (key) {
            case 72: // ARROW UP
                option = (option > 0) ? --option : option;
                displayMenu();
                break;

            case 80: // ARROW DOWN
                option = (option < options.size() - 1) ? ++option : option;
                displayMenu();
                break;

            case 13: // ENTER
                if (option == 0) {
                    startGame(true);
                    displayMenu();
                }
                else if (option == 1) {
                    system("cls");
                    cout << "Scores feature coming soon!\n";
                    system("pause");
                    displayMenu();
                }
                else if (option == 2) {
                    exit(0);
                }
                break;
            }
        }
    }
}
