// Text Pong
// v0.4
// by Hylst (1994 - 2024 (cleaning code / good practice / comments) )
// To Do :
// Enhance extended ascii characters display
// Several lives before game over
// Ask before quit if new game
// Explain keyboard controls in introduction
// Make the game board bigger


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

#define WIDTH 40
#define HEIGHT 20
#define PADDLE_CHAR '|'
#define BALL_CHAR 'O'
#define EMPTY ' '
#define OBSTACLE 'X'

// Position structure
typedef struct {
    int x, y;
} Position;

Position ball = {WIDTH / 2, HEIGHT / 2};
Position paddle1 = {1, HEIGHT / 2};
Position paddle2 = {WIDTH - 2, HEIGHT / 2};
Position obstacles[5]; // Obstacles
int ballDirectionX = 1;
int ballDirectionY = 1;
int score1 = 0, score2 = 0;
int mode = 0;  // 0 = Solo, 1 = Two players

// Cross-platform clear screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
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

// Draw the game field with ASCII borders and paddles
void drawBoard() {
    clearScreen();
    printf("╔");
    for (int i = 0; i < WIDTH; i++) printf("═");
    printf("╗\n");

    for (int y = 0; y < HEIGHT; y++) {
        printf("║");
        for (int x = 0; x < WIDTH; x++) {
            if (x == ball.x && y == ball.y) {
                printf("%c", BALL_CHAR); // Ball
            } else if (x == paddle1.x && y >= paddle1.y - 1 && y <= paddle1.y + 1) {
                printf("%c", PADDLE_CHAR); // Player 1 paddle
            } else if (x == paddle2.x && y >= paddle2.y - 1 && y <= paddle2.y + 1) {
                printf("%c", PADDLE_CHAR); // Player 2/AI paddle
            } else {
                int isObstacle = 0;
                for (int i = 0; i < 5; i++) {
                    if (x == obstacles[i].x && y == obstacles[i].y) {
                        printf("%c", OBSTACLE); // Obstacles
                        isObstacle = 1;
                        break;
                    }
                }
                if (!isObstacle) {
                    printf("%c", EMPTY); // Empty space
                }
            }
        }
        printf("║\n");
    }

    printf("╚");
    for (int i = 0; i < WIDTH; i++) printf("═");
    printf("╝\n");
    printf("Score: Player 1: %d | Player 2: %d\n", score1, score2);
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
        char key = getchar();
        // Player 1 controls (q = up, a = down)
        if (key == 'q' && paddle1.y > 1) paddle1.y--;
        if (key == 'a' && paddle1.y < HEIGHT - 2) paddle1.y++;
        // Player 2/AI controls (p = up, l = down) for two players
        if (mode == 1) {
            if (key == 'p' && paddle2.y > 1) paddle2.y--;
            if (key == 'l' && paddle2.y < HEIGHT - 2) paddle2.y++;
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
    int choice;
    printf("╔═══════════════╗\n");
    printf("║    PONG GAME  ║\n");
    printf("╠═══════════════╣\n");
    printf("║ 1. Solo       ║\n");
    printf("║ 2. Two Players║\n");
    printf("╚═══════════════╝\n");
    printf("Choose a mode: ");
    scanf("%d", &choice);
    mode = (choice == 2) ? 1 : 0;
}

// Generate random obstacles
void generateObstacles() {
    for (int i = 0; i < 5; i++) {
        obstacles[i].x = rand() % (WIDTH - 10) + 5; // Avoid near paddles
        obstacles[i].y = rand() % (HEIGHT - 2) + 1;
    }
}

// Update the high scores file
void updateScores(int score1, int score2) {
    FILE *file = fopen("highscores.txt", "a");
    fprintf(file, "Player 1: %d - Player 2: %d\n", score1, score2);
    fclose(file);
}

// Display the high scores
void showHighScores() {
    FILE *file = fopen("highscores.txt", "r");
    if (file == NULL) {
        printf("No scores available.\n");
        return;
    }
    char line[100];
    printf("High Scores:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

// Main game loop
int main() {
    showMenu();  // Choose solo or two players
    generateObstacles();  // Generate obstacles on the field

    while (1) {
        drawBoard();
        processInput();
        updateBall();
        if (mode == 0) moveAI();  // AI movement if solo
        wait(100);
    }

    // Update and show high scores after game ends
    updateScores(score1, score2);
    showHighScores();
}