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

Position head;
Position point;
vector<Position> snake;

// Utility: Overloaded equality operator for Position struct
bool operator==(const Position& a, const Position& b) {
    return a.x == b.x && a.y == b.y;
}

void startGame(bool gameMode);

// Utility: Clear console screen
void ClearScreen() {
    COORD cursorPosition = { 0, 0 };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cursorPosition);
}

// Displays the main menu
void displayMenu() {
    ClearScreen();
    cout << "Use UP/DOWN arrows to navigate, RIGHT to select.\n";
    for (int q = 0; q < options.size(); q++) {
        string arrow = (option == q) ? " -> " : "    ";
        cout << arrow << options[q] << endl;
    }
}

// Displays the map during the game
void displayMap() {
    ClearScreen();
    cout << "\tScore: " << score << "\n";
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || i == HEIGHT - 1 || j == 0 || j == WIDTH - 1) {
                cout << "#";
            }
            else {
                bool isSnake = false;
                for (const auto& segment : snake) {
                    if (i == segment.y && j == segment.x) {
                        cout << snakeSymb;
                        isSnake = true;
                        break;
                    }
                }
                if (!isSnake) {
                    cout << ((i == point.y && j == point.x) ? fruit : ' ');
                }
            }
        }
        cout << endl;
    }
}

// Handles movement and key inputs during the game
void movement() {
    if (_kbhit()) {
        int key = _getch();
        if (key == 224) key = _getch(); // Handle arrow keys
        switch (key) {
        case 72: if (direction != down) direction = up; break;
        case 80: if (direction != up) direction = down; break;
        case 75: if (direction != dirRight) direction = dirLeft; break;
        case 77: if (direction != dirLeft) direction = dirRight; break;
        }
    }
}

// Spawns a new fruit point on the map
void spawnPoint() {
    bool validPoint = false;
    Position pre_point;

    while (!validPoint) {
        // Generate a random position
        pre_point = { 1 + rand() % (WIDTH - 2), 1 + rand() % (HEIGHT - 2) };

        // Assume it's valid, then check against the snake
        validPoint = true;
        for (const auto& segment : snake) {
            if (pre_point == segment) {
                validPoint = false; // Found a conflict, regenerate
                break;
            }
        }
    }

    // Once a valid point is found, assign it
    point = pre_point;
}


// Ends the game and provides options to restart or return to the menu
void gameOver() {
    system("cls");
    isPlay = false;
    cout << "YOUR SCORE: " << score << "\n";
    cout << "Back to Menu\t<- Left\nRestart\t\t-> Right\n";

    while (true) {
        int key = _getch();
        if (key == 224) key = _getch();
        switch (key) {
        case 75: system("cls"); displayMenu(); return; // Left arrow: Back to menu
        case 77: system("cls"); startGame(true); return; // Right arrow: Restart game
        }
    }
}

// Game logic for snake movement, collision, and scoring
void logic() {
    Position prev = head;
    Position prev2;

    for (auto& segment : snake) {
        prev2 = segment;
        segment = prev;
        prev = prev2;
    }

    if (direction == up) head.y--;
    else if (direction == down) head.y++;
    else if (direction == dirLeft) head.x--;
    else if (direction == dirRight) head.x++;

    // Wrap snake around edges
    if (head.x <= 0) head.x = WIDTH - 2;
    else if (head.x >= WIDTH - 1) head.x = 1;
    if (head.y <= 0) head.y = HEIGHT - 2;
    else if (head.y >= HEIGHT - 1) head.y = 1;

    // Check if snake eats itself
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i] == head) {
            gameOver();
            return;
        }
    }

    // Check if fruit is eaten
    if (head == point) {
        score++;
        spawnPoint();
        snake.push_back(snake.back()); // Extend snake
    }
}

// Starts the game loop
void startGame(bool gameMode) {
    isPlay = gameMode;
    direction = wait;
    head = { WIDTH / 2, HEIGHT / 2 };
    snake = { head };
    score = 0;
    spawnPoint();

    while (isPlay) {
        displayMap();
        movement();
        logic();
        Sleep(200);
    }
}

// Entry point
int main() {
    srand(static_cast<unsigned>(time(0)));
    displayMenu();

    while (true) {
        int key = _getch();
        if (key == 224) key = _getch();
        switch (key) {
        case 72: option = max(0, option - 1); break; // Arrow Up
        case 80: option = min(static_cast<int>(options.size() - 1), option + 1); break; // Arrow Down
        case 77: // Enter
            if (option == 0) 
            {
                system("cls");
                startGame(true);
            }// New Game
            else if (option == 1) { // Scores
                system("cls");
                cout << "Scores feature coming soon!\n";
                system("pause");
                displayMenu();
            }
            else if (option == 2) exit(0); // Exit
            break;
        }
        displayMenu();
    }
}
