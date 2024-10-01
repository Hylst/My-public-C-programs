// TEXT SNAKE 
// v1.2
// by Hylst (1994 - 2024 (cleaning code / good practice / comments / cross platform) )
// It's far from perfect, but ... Hey ! It's done ^^

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

#define WIDTH 40  // Game area width (with 2 extra columns for the frame)
#define HEIGHT 20  // Game area height (with 2 extra rows for the frame)
#define SNAKE_BODY 'O'
#define SNAKE_HEAD '@'
#define FOOD '*'
#define EMPTY ' '
#define OBSTACLE '#'

// Structures for snake positions
typedef struct {
    int x, y;
} Position;

Position snake1[100];
Position snake2[100];
int snakeLength1 = 5, snakeLength2 = 5;
Position food;
Position obstacles[100];
int numObstacles = 5;
int directionX1 = 1, directionY1 = 0;  // Player 1: Initially moving right
int directionX2 = 0, directionY2 = 0;  // Player 2/AI: Initially not moving
int gameOver = 0;
int speed = 300;
int multiplayer = 0;  // 0 = Solo, 1 = VS Human, 2 = VS AI
int paused = 0;  // 0 = not paused, 1 = paused
int level = 1;  // Starting level
int aiDifficulty = 1;  // AI difficulty (1 = Easy, 2 = Medium, 3 = Hard)

// Cross-platform clear screen function
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Cross-platform wait function
void wait(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

// Cross-platform sound function for events
void playSound(int frequency, int duration) {
    #ifdef _WIN32
        Beep(frequency, duration);
    #else
        printf("\a");
    #endif
}

// Cross-platform input handling (non-blocking)
#ifdef _WIN32
int kbhit() {
    return _kbhit();
}

char getInput() {
    return _getch();
}
#else
int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

char getInput() {
    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

// Display game introduction
void showIntroduction() {
    clearScreen();
    printf("=== SNAKE GAME ===\n\n");
    printf("Welcome to the Snake game!\n");
    printf("Solo Mode:\n");
    printf("  - Control the snake with ZQSD keys, eat food to grow, and avoid obstacles.\n");
    printf("  - The game becomes more difficult with added obstacles and increased speed as you progress.\n\n");
    printf("Duo Mode:\n");
    printf("  - VS Human: Player 1 controls with ZQSD, Player 2 with arrow keys.\n");
    printf("  - VS AI: Play against an AI-controlled snake.\n");
    printf("\nPress any key to continue...");
    getInput();
}

// Main menu to choose the game mode
int displayMenu() {
    int choice;
    clearScreen();
    printf("=== SNAKE GAME ===\n");
    printf("1. Solo\n");
    printf("2. Duo (VS Human)\n");
    printf("3. Duo (VS AI)\n");
    printf("4. Quit\n");
    printf("Choose an option: ");
    scanf("%d", &choice);
    return choice;
}

// AI difficulty selection menu
int selectAIDifficulty() {
    int choice;
    printf("Select AI Difficulty:\n");
    printf("1. Easy\n");
    printf("2. Medium\n");
    printf("3. Hard\n");
    printf("Choose a difficulty: ");
    scanf("%d", &choice);
    return choice;
}

// Ask the player if they want to replay
int askReplay() {
    char choice;
    printf("Do you want to play again? (y/n): ");
    scanf(" %c", &choice);
    return (choice == 'y' || choice == 'Y');
}

// Initialize the game (snakes, food, obstacles)
void initGame() {
    snakeLength1 = 5;
    directionX1 = 1; directionY1 = 0;
    snakeLength2 = 5;
    directionX2 = 0; directionY2 = 0;

    gameOver = 0;
    speed = 300;
    paused = 0;
    level = 1;
    numObstacles = 5;

    // Initialize snake 1 position
    for (int i = 0; i < snakeLength1; i++) {
        snake1[i].x = 5 - i;
        snake1[i].y = 10;
    }
    
    // Initialize snake 2 position
    for (int i = 0; i < snakeLength2; i++) {
        snake2[i].x = WIDTH - 5 + i;
        snake2[i].y = 15;
    }

    // Initialize food and obstacles
    food.x = rand() % (WIDTH - 2) + 1;
    food.y = rand() % (HEIGHT - 2) + 1;

    for (int i = 0; i < numObstacles; i++) {
        obstacles[i].x = rand() % (WIDTH - 2) + 1;
        obstacles[i].y = rand() % (HEIGHT - 2) + 1;
    }
}

// Draw the game frame inside the game board
void drawFrame() {
    for (int x = 0; x < WIDTH; x++) {
        printf("-");
    }
    printf("\n");

    for (int y = 1; y < HEIGHT - 1; y++) {
        printf("|");
        for (int x = 1; x < WIDTH - 1; x++) {
            printf("%c", EMPTY);
        }
        printf("|\n");
    }

    for (int x = 0; x < WIDTH; x++) {
        printf("-");
    }
    printf("\n");
}

// Render the game board (snakes, food, obstacles)
void drawBoard() {
    // Ensure no "blinking" by first processing input, then drawing
    clearScreen();  // Clear the screen after input

    // Draw the frame
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1 || x == 0 || x == WIDTH - 1) {
                // Draw borders
                if (y == 0 || y == HEIGHT - 1) printf("-");
                else printf("|");
            } else {
                // Inside the game frame
                int isSnake1 = 0, isSnake2 = 0, isObstacle = 0;

                // Draw snake 1
                for (int i = 0; i < snakeLength1; i++) {
                    if (x == snake1[i].x && y == snake1[i].y) {
                        printf(i == 0 ? "%c" : "%c", SNAKE_HEAD, SNAKE_BODY);
                        isSnake1 = 1;
                        break;
                    }
                }

                // Draw snake 2
                if (!isSnake1) {
                    for (int i = 0; i < snakeLength2; i++) {
                        if (x == snake2[i].x && y == snake2[i].y) {
                            printf(i == 0 ? "%c" : "%c", SNAKE_HEAD, SNAKE_BODY);
                            isSnake2 = 1;
                            break;
                        }
                    }
                }

                // Draw food
                if (x == food.x && y == food.y) {
                    printf("%c", FOOD);
                } 
                // Draw obstacles
                else if (!isSnake1 && !isSnake2) {
                    for (int i = 0; i < numObstacles; i++) {
                        if (x == obstacles[i].x && y == obstacles[i].y) {
                            printf("%c", OBSTACLE);
                            isObstacle = 1;
                            break;
                        }
                    }

                    // If no snake or food or obstacle, leave it empty
                    if (!isObstacle) {
                        printf(" ");
                    }
                }
            }
        }
        printf("\n");
    }

    // Display the score
    printf("Player 1 Score: %d | Player 2 Score: %d | Level: %d\n", snakeLength1 - 5, snakeLength2 - 5, level);
}

// Update snake's position and handle collisions
void updateSnake(Position snake[], int *snakeLength, int directionX, int directionY) {
    Position newHead = {snake[0].x + directionX, snake[0].y + directionY};

    // Check collisions with walls
    if (newHead.x <= 0 || newHead.x >= WIDTH - 1 || newHead.y <= 0 || newHead.y >= HEIGHT - 1) {
        gameOver = 1;
        playSound(200, 500);
        return;
    }

    // Check collisions with obstacles
    for (int i = 0; i < numObstacles; i++) {
        if (newHead.x == obstacles[i].x && newHead.y == obstacles[i].y) {
            gameOver = 1;
            playSound(200, 500);
            return;
        }
    }

    // Check if the snake eats food
    if (newHead.x == food.x && newHead.y == food.y) {
        (*snakeLength)++;
        food.x = rand() % (WIDTH - 2) + 1;
        food.y = rand() % (HEIGHT - 2) + 1;
        playSound(1000, 100);
        speed -= 10;
        level++;
        numObstacles++;
        obstacles[numObstacles - 1].x = rand() % (WIDTH - 2) + 1;
        obstacles[numObstacles - 1].y = rand() % (HEIGHT - 2) + 1;
    }

    // Check collision with the other snake
    for (int i = 0; i < (*snakeLength); i++) {
        if (newHead.x == snake2[i].x && newHead.y == snake2[i].y) {
            gameOver = 1;
            playSound(200, 500);
            return;
        }
    }

    // Update the snake's body
    for (int i = (*snakeLength) - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = newHead;
}

// Simple AI control for Player 2
void controlAI(Position snake[], int *directionX, int *directionY) {
    if (aiDifficulty == 1) {  // Easy: Random movement with some food-seeking
        if (rand() % 4 == 0) {
            *directionX = (food.x > snake[0].x) ? 1 : (food.x < snake[0].x) ? -1 : *directionX;
            *directionY = (food.y > snake[0].y) ? 1 : (food.y < snake[0].y) ? -1 : *directionY;
        } else {
            *directionX = rand() % 3 - 1;
            *directionY = rand() % 3 - 1;
        }
    } else if (aiDifficulty == 2) {  // Medium: Follows food more accurately, avoids obstacles
        *directionX = (food.x > snake[0].x) ? 1 : (food.x < snake[0].x) ? -1 : 0;
        *directionY = (food.y > snake[0].y) ? 1 : (food.y < snake[0].y) ? -1 : 0;
        for (int i = 0; i < numObstacles; i++) {
            if (snake[0].x + *directionX == obstacles[i].x && snake[0].y + *directionY == obstacles[i].y) {
                *directionX = rand() % 3 - 1;
                *directionY = rand() % 3 - 1;
            }
        }
    } else {  // Hard: Actively seeks food and avoids obstacles and the player's snake
        *directionX = (food.x > snake[0].x) ? 1 : (food.x < snake[0].x) ? -1 : 0;
        *directionY = (food.y > snake[0].y) ? 1 : (food.y < snake[0].y) ? -1 : 0;
        for (int i = 0; i < numObstacles; i++) {
            if (snake[0].x + *directionX == obstacles[i].x && snake[0].y + *directionY == obstacles[i].y) {
                *directionX = rand() % 3 - 1;
                *directionY = rand() % 3 - 1;
            }
        }
        for (int i = 0; i < snakeLength1; i++) {
            if (snake[0].x + *directionX == snake1[i].x && snake[0].y + *directionY == snake1[i].y) {
                *directionX = rand() % 3 - 1;
                *directionY = rand() % 3 - 1;
            }
        }
    }
}

// Process user inputs
void processInput() {
    if (kbhit()) {
        char key = getInput();
        switch (key) {
            // Player 1 controls (ZQSD)
            case 'z': if (directionY1 == 0) { directionX1 = 0; directionY1 = -1; } break;
            case 's': if (directionY1 == 0) { directionX1 = 0; directionY1 = 1; } break;
            case 'q': if (directionX1 == 0) { directionX1 = -1; directionY1 = 0; } break;
            case 'd': if (directionX1 == 0) { directionX1 = 1; directionY1 = 0; } break;
            // Player 2 controls (Arrow keys)
            case 72: if (directionY2 == 0) { directionX2 = 0; directionY2 = -1; } break;  // Up arrow
            case 80: if (directionY2 == 0) { directionX2 = 0; directionY2 = 1; } break;   // Down arrow
            case 75: if (directionX2 == 0) { directionX2 = -1; directionY2 = 0; } break;  // Left arrow
            case 77: if (directionX2 == 0) { directionX2 = 1; directionY2 = 0; } break;   // Right arrow
            // Pause the game
            case 'p': paused = !paused; break;
        }
    }
}

// Main game loop
void gameLoop() {
    while (!gameOver) {
        if (!paused) {
            processInput();  // Process user input before clearing the screen
            drawBoard();  // Then draw the board after clearing
            updateSnake(snake1, &snakeLength1, directionX1, directionY1);
            if (multiplayer == 1) {  // VS Human
                updateSnake(snake2, &snakeLength2, directionX2, directionY2);
            } else if (multiplayer == 2) {  // VS AI
                controlAI(snake2, &directionX2, &directionY2);
                updateSnake(snake2, &snakeLength2, directionX2, directionY2);
            }
            wait(speed);
        } else {
            printf("PAUSE - Press 'P' to resume.\n");
            processInput();
        }
    }
    printf("Game Over! Player 1 Score: %d, Player 2 Score: %d\n", snakeLength1 - 5, snakeLength2 - 5);
}

// Main function
int main() {
    showIntroduction();

    while (1) {
        int choice = displayMenu();
        if (choice == 4) {
            break;  // Quit the game
        }

        multiplayer = (choice == 2) ? 1 : (choice == 3) ? 2 : 0;
        if (multiplayer == 2) {
            aiDifficulty = selectAIDifficulty();
        }
        initGame();
        gameLoop();

        if (!askReplay()) {
            break;
        }
    }

    return 0;
}
