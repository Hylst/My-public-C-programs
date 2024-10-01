// Text Pong
// v1
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
    #include <conio.h>
    #include <windows.h> // For Sleep, Beep, and setting UTF-8 encoding
#else
    #include <unistd.h>  // For usleep
    #include <termios.h> // For non-blocking input in Unix
    #include <fcntl.h>
    #include <locale.h>  // To fix extended ASCII characters on Unix
#endif

#define WIDTH 80
#define HEIGHT 24
#define PADDLE_CHAR '|'
#define BALL_CHAR 'O'
#define EMPTY ' '
#define OBSTACLE 'X'

typedef struct {
    int x, y;
} Position;

Position ball = {WIDTH / 2, HEIGHT / 2};
Position paddle1 = {1, HEIGHT / 2};
Position paddle2 = {WIDTH - 2, HEIGHT / 2};
Position obstacles[5]; // Obstacles on the field
int ballDirectionX = 1;
int ballDirectionY = 1;
int score1 = 0, score2 = 0;
int mode = 0;  // 0 = Solo, 1 = Two players
int isPaused = 0;

// Cross-platform clear screen
void clearScreen() {
    #ifdef _WIN32
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        COORD Position = {0, 0};
        SetConsoleCursorPosition(hOut, Position);
    #else
        printf("\033[H\033[J"); // ANSI escape code for clearing screen
    #endif
}

// Cross-platform wait (ms)
void wait(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

// Cross-platform sound effect
void playSound(int frequency, int duration) {
    #ifdef _WIN32
        Beep(frequency, duration);
    #else
        printf("\a");
    #endif
}

// Unix-specific non-blocking input
#ifndef _WIN32
int _kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    
    return 0;
}
#endif

// Double-buffered drawing
void drawBoard(char *buffer) {
    sprintf(buffer, "╔");
    for (int i = 0; i < WIDTH; i++) strcat(buffer, "═");
    strcat(buffer, "╗\n");

    for (int y = 0; y < HEIGHT; y++) {
        strcat(buffer, "║");
        for (int x = 0; x < WIDTH; x++) {
            if (x == ball.x && y == ball.y) {
                strcat(buffer, "O"); // Ball
            } else if (x == paddle1.x && y >= paddle1.y - 1 && y <= paddle1.y + 1) {
                strcat(buffer, "|"); // Player 1 paddle
            } else if (x == paddle2.x && y >= paddle2.y - 1 && y <= paddle2.y + 1) {
                strcat(buffer, "|"); // Player 2/AI paddle
            } else {
                int isObstacle = 0;
                for (int i = 0; i < 5; i++) {
                    if (x == obstacles[i].x && y == obstacles[i].y) {
                        strcat(buffer, "X"); // Obstacles
                        isObstacle = 1;
                        break;
                    }
                }
                if (!isObstacle) {
                    strcat(buffer, " "); // Empty space
                }
            }
        }
        strcat(buffer, "║\n");
    }

    strcat(buffer, "╚");
    for (int i = 0; i < WIDTH; i++) strcat(buffer, "═");
    strcat(buffer, "╝\n");
    char scoreBuffer[50];
    sprintf(scoreBuffer, "Score: Player 1: %d | Player 2: %d\n", score1, score2);
    strcat(buffer, scoreBuffer);
}

// Update ball position and handle collisions
void updateBall() {
    ball.x += ballDirectionX;
    ball.y += ballDirectionY;

    // Collision with top or bottom walls
    if (ball.y <= 0 || ball.y >= HEIGHT - 1) {
        ballDirectionY = -ballDirectionY;
        playSound(1000, 100); // Bounce sound
    }

    // Collision with paddles
    if (ball.x == paddle1.x + 1 && ball.y >= paddle1.y - 1 && ball.y <= paddle1.y + 1) {
        ballDirectionX = -ballDirectionX;
        playSound(2000, 100); // Paddle hit sound
    } else if (ball.x == paddle2.x - 1 && ball.y >= paddle2.y - 1 && ball.y <= paddle2.y + 1) {
        ballDirectionX = -ballDirectionX;
        playSound(2000, 100); // Paddle hit sound
    }

    // Check if ball hits an obstacle
    for (int i = 0; i < 5; i++) {
        if (ball.x == obstacles[i].x && ball.y == obstacles[i].y) {
            ballDirectionX = -ballDirectionX;
            ballDirectionY = -ballDirectionY;
            playSound(1500, 100); // Obstacle hit sound
        }
    }

    // Scoring
    if (ball.x <= 0) {
        score2++;
        ball.x = WIDTH / 2;
        ball.y = HEIGHT / 2;
        playSound(500, 300); // Score sound
    } else if (ball.x >= WIDTH - 1) {
        score1++;
        ball.x = WIDTH / 2;
        ball.y = HEIGHT / 2;
        playSound(500, 300); // Score sound
    }
}

// Player and AI input
void processInput() {
    if (_kbhit()) {
        char key;
        #ifdef _WIN32
            key = _getch();  // Use _getch() under Windows
        #else
            key = getchar();  // Use getchar() under Unix
        #endif
        if (key == 'q') {
            printf("\nYou decided to quit. No hard feelings, right? Game over.\n");
            exit(0);  // Quit the game
        } else if (key == 'p') {
            isPaused = !isPaused;  // Toggle pause
        }
        if (!isPaused) {
            // Player 1 controls (w = up, s = down)
            if (key == 'z' && paddle1.y > 1) paddle1.y--;
            if (key == 's' && paddle1.y < HEIGHT - 2) paddle1.y++;
            // Player 2/AI controls (o = up, l = down) for two players
            if (mode == 1) {
                if (key == 'o' && paddle2.y > 1) paddle2.y--;
                if (key == 'l' && paddle2.y < HEIGHT - 2) paddle2.y++;
            }
        }
    }
}

// AI logic for solo mode
void moveAI() {
    if (ball.y < paddle2.y && paddle2.y > 1) {
        paddle2.y--;
    } else if (ball.y > paddle2.y && paddle2.y < HEIGHT - 2) {
        paddle2.y++;
    }
}

// Menu for game mode selection
void showMenu() {
    clearScreen();
    printf("╔════════════════════════════════════╗\n");
    printf("║          WELCOME TO PONG!          ║\n");
    printf("╠════════════════════════════════════╣\n");
    printf("║ 1. Solo Mode                       ║\n");
    printf("║ 2. Two-Player Mode                 ║\n");
    printf("║ 3. Quit (I'll miss you)            ║\n");
    printf("╠════════════════════════════════════╣\n");
    printf("║ Controls:                          ║\n");
    printf("║  Player 1: z (up), s (down)        ║\n");
    printf("║  Player 2: s (up), l (down)        ║\n");
    printf("║  p: Pause, q: Quit anytime         ║\n");
    printf("╚════════════════════════════════════╝\n");
    printf("Choose an option: ");
    int choice;
    scanf("%d", &choice);
    if (choice == 3) {
        printf("You chose to leave. We'll miss you!\n");
        exit(0);  // Quit game
    }
    mode = (choice == 2) ? 1 : 0;
}

// Generate random obstacles
void generateObstacles() {
    for (int i = 0; i < 5; i++) {
        obstacles[i].x = rand() % (WIDTH - 10) + 5; // Avoid near paddles
        obstacles[i].y = rand() % (HEIGHT - 2) + 1;
    }
}

// Main game loop
int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);  // Set UTF-8 for extended characters
    #else
        setlocale(LC_ALL, "");  // Fix extended ASCII characters on Unix
    #endif

    showMenu();  // Show the menu with humor
    generateObstacles();  // Generate obstacles on the field
    char buffer[4096];  // Double buffer for screen rendering

    while (1) {
        if (!isPaused) {
            memset(buffer, 0, sizeof(buffer));  // Clear buffer
            drawBoard(buffer);
            clearScreen();
            printf("%s", buffer);  // Render buffer
            processInput();
            updateBall();
            if (mode == 0) moveAI();  // AI movement if solo
        } else {
            printf("\nGame is paused. Press 'p' to resume.\n");
        }
        wait(100);  // Frame delay
    }

    return 0;
}
