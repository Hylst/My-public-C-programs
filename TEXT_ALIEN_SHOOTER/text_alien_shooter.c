// Text Alien Shooter
// v0.7
// by Hylst (1996 - 2024 (cleaning code / good practice / comments) )

#include <stdio.h>
#include <stdlib.h>
#ifdef _WIN32
    #include <conio.h>   // For _kbhit() and _getch() on Windows
    #include <windows.h> // For Sleep(), Beep(), and SetConsoleOutputCP on Windows
#else
    #include <unistd.h>  // For usleep() on Unix
    #include <termios.h> // For non-blocking input on Unix
    #include <fcntl.h>
#endif

#define WIDTH 40
#define HEIGHT 40  // Doubled height
#define PLAYER_CHAR '^'
#define ALIEN_CHAR 'X'
#define BULLET_CHAR '|'
#define EMPTY ' '
#define WALL_CHAR '#'

// Structure to handle positions
typedef struct {
    int x, y;
} Position;

// Global Variables
Position player = {WIDTH / 2, HEIGHT - 1};
Position aliens[50];
Position bullet;
int bulletActive = 0;
int alienCount = 10;
int score = 0;
int gameOver = 0;

// Cross-platform screen clear
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Cross-platform delay
void wait(int milliseconds) {
    #ifdef _WIN32
        Sleep(milliseconds);
    #else
        usleep(milliseconds * 1000);
    #endif
}

// Cross-platform sound
void playSound(int frequency, int duration) {
    #ifdef _WIN32
        Beep(frequency, duration);
    #else
        printf("\a");  // Unix beep
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

// Initialize aliens
void initAliens() {
    for (int i = 0; i < alienCount; i++) {
        aliens[i].x = rand() % WIDTH;
        aliens[i].y = rand() % 5;  // Initial aliens at the top
    }
}

// Display the introduction menu
void displayMenu() {
    clearScreen();
    printf("##################################################\n");
    printf("#                 ALIEN SHOOTER                  #\n");
    printf("##################################################\n");
    printf("#               Press 'z' to shoot!              #\n");
    printf("#            Move with 'q' and 'd' keys          #\n");
    printf("#              Shoot the alien ships!            #\n");
    printf("##################################################\n");
    printf("Press any key to start...\n");
    getchar();  // Wait for any key press
}

// Draw the game board
void drawBoard() {
    clearScreen();
    printf("╔");
    for (int i = 0; i < WIDTH; i++) printf("═");
    printf("╗\n");

    for (int y = 0; y < HEIGHT; y++) {
        printf("║");
        for (int x = 0; x < WIDTH; x++) {
            if (x == player.x && y == player.y) {
                printf("%c", PLAYER_CHAR);
            } else if (bulletActive && x == bullet.x && y == bullet.y) {
                printf("%c", BULLET_CHAR);
            } else {
                int isAlien = 0;
                for (int i = 0; i < alienCount; i++) {
                    if (aliens[i].x == x && aliens[i].y == y) {
                        printf("%c", ALIEN_CHAR);
                        isAlien = 1;
                        break;
                    }
                }
                if (!isAlien) printf("%c", EMPTY);
            }
        }
        printf("║\n");
    }

    printf("╚");
    for (int i = 0; i < WIDTH; i++) printf("═");
    printf("╝\n");
    printf("Score: %d\n", score);
}

// Move player based on input
void processInput() {
    if (_kbhit()) {
        char key = 
        #ifdef _WIN32
            _getch(); // No ENTER required
        #else
            getchar();
        #endif

        if (key == 'q' && player.x > 0) player.x--;     // Move left
        if (key == 'd' && player.x < WIDTH - 1) player.x++;  // Move right
        if (key == 'z' && !bulletActive) {  // Fire bullet
            bullet.x = player.x;
            bullet.y = player.y - 1;
            bulletActive = 1;
            playSound(1000, 100);
        }
    }
}

// Move aliens down
void moveAliens() {
    for (int i = 0; i < alienCount; i++) {
        aliens[i].y++;
        if (aliens[i].y >= HEIGHT) {
            gameOver = 1;  // Alien reaches bottom, game over
            playSound(200, 500);
        }
    }
}

// Move bullet and handle collision
void moveBullet() {
    if (bulletActive) {
        bullet.y--;
        if (bullet.y < 0) bulletActive = 0;

        for (int i = 0; i < alienCount; i++) {
            if (bullet.x == aliens[i].x && bullet.y == aliens[i].y) {
                score++;
                aliens[i].x = rand() % WIDTH;  // Respawn alien at top
                aliens[i].y = rand() % 5;
                bulletActive = 0;
                playSound(1500, 200);  // Hit sound
            }
        }
    }
}

// Main game loop
int main() {
    #ifdef _WIN32
        SetConsoleOutputCP(437);  // Set code page for extended ASCII on Windows
    #endif
    
    displayMenu();
    initAliens();
    
    while (!gameOver) {
        drawBoard();
        processInput();
        moveBullet();
        moveAliens();
        wait(100);  // Control game speed
    }

    printf("Game Over! Final Score: %d\n", score);
    return 0;
}
