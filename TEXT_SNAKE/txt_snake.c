// TEXT SNAKE 
// v0.3
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Display a menu to ask if user want to play alon 
// Double board size
#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <termios.h>
    #include <fcntl.h>
#endif

#define WIDTH 30
#define HEIGHT 20
#define SNAKE_BODY 'O'
#define SNAKE_HEAD '@'
#define FOOD 'X'
#define EMPTY ' '
#define OBSTACLE '#'

// Position structure
typedef struct {
    int x, y;
} Position;

Position snake[100];
int snakeLength = 5;
Position food;
Position obstacles[5]; // Obstacles
int directionX = 1, directionY = 0;  // Initially moving right
int gameOver = 0;
int speed = 200;  // Initial speed of the game

// Cross-platform clear screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Cross-platform wait
void wait(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);  // Convert to microseconds for usleep
    #endif
}

// Cross-platform sound for events
void playSound(int frequency, int duration) {
    #ifdef _WIN32
        Beep(frequency, duration);
    #else
        printf("\a");  // Terminal beep for Unix
    #endif
}

// Cross-platform input handling
#ifdef _WIN32
int kbhit() {
    return _kbhit();
}
#else
int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}
#endif

// Initialize the snake, food, and obstacles
void initGame() {
    for (int i = 0; i < snakeLength; i++) {
        snake[i].x = 5 - i;
        snake[i].y = 10;
    }
    food.x = rand() % WIDTH;
    food.y = rand() % HEIGHT;
    
    // Initialize obstacles
    for (int i = 0; i < 5; i++) {
        obstacles[i].x = rand() % WIDTH;
        obstacles[i].y = rand() % HEIGHT;
    }
}

// Draw the game board
void drawBoard() {
    clearScreen();
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            int isSnake = 0, isObstacle = 0;
            
            // Draw the snake
            for (int i = 0; i < snakeLength; i++) {
                if (x == snake[i].x && y == snake[i].y) {
                    printf(i == 0 ? "%c" : "%c", SNAKE_HEAD, SNAKE_BODY);
                    isSnake = 1;
                    break;
                }
            }

            // Draw the food
            if (x == food.x && y == food.y) {
                printf("%c", FOOD);
            } 
            // Draw obstacles
            else {
                for (int i = 0; i < 5; i++) {
                    if (x == obstacles[i].x && y == obstacles[i].y) {
                        printf("%c", OBSTACLE);
                        isObstacle = 1;
                        break;
                    }
                }
                // If not snake or food, draw empty space
                if (!isSnake && !isObstacle) {
                    printf("%c", EMPTY);
                }
            }
        }
        printf("\n");
    }
    printf("Score: %d\n", snakeLength - 5);  // Display score
}

// Update snake's position and handle collisions
void updateSnake() {
    Position newHead = {snake[0].x + directionX, snake[0].y + directionY};

    // Check collisions with walls
    if (newHead.x < 0 || newHead.x >= WIDTH || newHead.y < 0 || newHead.y >= HEIGHT) {
        gameOver = 1;
        playSound(200, 500);  // Game over sound
        return;
    }

    // Check collisions with obstacles
    for (int i = 0; i < 5; i++) {
        if (newHead.x == obstacles[i].x && newHead.y == obstacles[i].y) {
            gameOver = 1;
            playSound(200, 500);  // Game over sound
            return;
        }
    }

    // Check if the snake eats the food
    if (newHead.x == food.x && newHead.y == food.y) {
        snakeLength++;
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;
        playSound(1000, 100);  // Food consumption sound
        speed -= 10;  // Increase speed as snake grows
    }

    // Update the snake's body
    for (int i = snakeLength - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = newHead;
}

// Handle user input for direction control
void processInput() {
    if (_kbhit()) {
        char key = getchar();
        switch (key) {
            case 'z': if (directionY == 0) { directionX = 0; directionY = -1; } break;  // Up
            case 's': if (directionY == 0) { directionX = 0; directionY = 1; } break;   // Down
            case 'q': if (directionX == 0) { directionX = -1; directionY = 0; } break;  // Left
            case 'd': if (directionX == 0) { directionX = 1; directionY = 0; } break;   // Right
        }
    }
}

// Main game loop
int main() {
    initGame();

    while (!gameOver) {
        drawBoard();
        processInput();
        updateSnake();
        wait(speed);  // Snake speed increases as the snake grows
    }

    printf("Game Over! Your score: %d\n", snakeLength - 5);
    return 0;
}